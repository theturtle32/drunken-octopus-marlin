/************************************
 * syndaver_level/hotend_screen.cpp *
 ************************************/

/****************************************************************************
 *   Written By Marcio Teixeira 2021 - SynDaver 3D                  *
 *                                                                          *
 *   This program is free software: you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation, either version 3 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   To view a copy of the GNU General Public License, go to the following  *
 *   location: <https://www.gnu.org/licenses/>.                             *
 ****************************************************************************/

#include "../config.h"
#include "../screens.h"
#include "../screen_data.h"

#ifdef SYNDAVER_LEVEL_HOTEND_SCREEN

using namespace ExtUI;
using namespace FTDI;
using namespace Theme;

constexpr static HotendScreenData &mydata = screen_data.HotendScreen;

#define GRID_COLS 4
#define GRID_ROWS 4

#define CURRENT_TEMP_LBL_POS BTN_POS(1,1),  BTN_SIZE(2,1)
#define CURRENT_TEMP_POS     BOX_POS(1,2),  BOX_SIZE(2,1)
#define TARGET_TEMP_ADJ_POS  BOX_POS(1,3),  BOX_SIZE(2,1)
#define INCREMENTS_POS       BTN_POS(1,4),  BTN_SIZE(2,1)

#define FILAMENT_LABL_POS    BTN_POS(3,1),  BTN_SIZE(2,1)
#define UNLD_BTN_POS         BTN_POS(3,2),  BTN_SIZE(1,1)
#define LOAD_BTN_POS         BTN_POS(4,2),  BTN_SIZE(1,1)
#define UNLD_CONT_POS        BTN_POS(3,3),  BTN_SIZE(1,1)
#define LOAD_CONT_POS        BTN_POS(4,3),  BTN_SIZE(1,1)
#define BACK_POS             BTN_POS(3,4),  BTN_SIZE(2,1)

void HotendScreen::onEntry() {
  BaseScreen::onEntry();
  mydata.repeat_tag = 0;
  mydata.inc_tag    = 6;
}

void HotendScreen::draw_adjuster(draw_mode_t what, uint8_t tag, float value, int16_t x, int16_t y, int16_t w, int16_t h) {
    #define SUB_COLS 9
    #define SUB_ROWS 1

    CommandProcessor cmd;
    cmd.tag(0)
       .font(font_medium);
    if (what & BACKGROUND) {
        cmd.colors(normal_btn).button(SUB_POS(1,1), SUB_SIZE(5,1), F(""), OPT_FLAT);
    }

    if (what & FOREGROUND) {
        char str[32];
        dtostrf(value, 3, 0, str);
        strcat_P(str, PSTR(" "));
        strcat_P(str, (const char*) GET_TEXT_F(MSG_UNITS_C));

        cmd.colors(normal_btn)
           .text(SUB_POS(1,1), SUB_SIZE(5,1), str)
           .tag(tag  ).button(SUB_POS(6,1), SUB_SIZE(2,1), F("-"))
           .tag(tag+1).button(SUB_POS(8,1), SUB_SIZE(2,1), F("+"));
    }
}

void HotendScreen::draw_increments(draw_mode_t what, int16_t x, int16_t y, int16_t w, int16_t h) {
  #undef  SUB_COLS
  #undef  SUB_ROWS
  #define SUB_COLS 3
  #define SUB_ROWS 1

  if (what & FOREGROUND) {
    CommandProcessor cmd;
    cmd.font(font_small)
       .tag(4).colors(4 == mydata.inc_tag ? action_btn : normal_btn).button(SUB_POS(1,1), SUB_SIZE(1,1), F("1"))
       .tag(5).colors(5 == mydata.inc_tag ? action_btn : normal_btn).button(SUB_POS(2,1), SUB_SIZE(1,1), F("10"))
       .tag(6).colors(6 == mydata.inc_tag ? action_btn : normal_btn).button(SUB_POS(3,1), SUB_SIZE(1,1), F("100"));
  }
}

float HotendScreen::get_increment() {
    switch(mydata.inc_tag) {
        case 4: return 1;
        case 5: return 10;
        default: return 100;
    }
}

void HotendScreen::draw_interaction_buttons(draw_mode_t what) {
  CommandProcessor cmd;
  if (what & BACKGROUND) {
    cmd.tag(0)
       .font(font_medium)
       .cmd(COLOR_RGB(bg_text_enabled))
       .text(FILAMENT_LABL_POS, F("Filament"));
  }
  if (what & FOREGROUND) {
    const bool t_ok = getActualTemp_celsius(E0) > EXTRUDE_MINTEMP &&
                      getTargetTemp_celsius(E0) > EXTRUDE_MINTEMP;

    #define TOG_STYLE(A) colors(A ? action_btn : normal_btn)

    if (!t_ok) reset_menu_timeout();

    const bool tog7 = mydata.repeat_tag == 7;
    const bool tog8 = mydata.repeat_tag == 8;

    cmd.colors(normal_btn)
       .font(font_medium)
       .tag(11)               .enabled(t_ok).button (UNLD_BTN_POS,  F("Unload"))
       .tag(12)               .enabled(t_ok).button (LOAD_BTN_POS,  F("Load"))
       .tag(7).TOG_STYLE(tog7).enabled(t_ok).button (UNLD_CONT_POS, F("Retract"))
       .tag(8).TOG_STYLE(tog8).enabled(t_ok).button (LOAD_CONT_POS, F("Extrude"))
       .tag(1).colors(action_btn)           .button (BACK_POS,      GET_TEXT_F(MSG_BUTTON_DONE));
  }
}

void HotendScreen::draw_temperature(draw_mode_t what, int16_t x, int16_t y, int16_t w, int16_t h) {
  #undef  SUB_COLS
  #undef  SUB_ROWS
  #define SUB_COLS 9
  #define SUB_ROWS 1
  
  if (what & FOREGROUND) {
    const float temp = getActualTemp_celsius(E0);
    char str[15];
    format_temp(str, temp);

    uint32_t fg_col, rgb_col;
    SynLevelUI::getTempColor(temp, fg_col, rgb_col);

    CommandProcessor cmd;
    cmd.font(font_medium)
       .fgcolor(fg_col)
       .cmd(COLOR_RGB(rgb_col))
       .tag(15).button(SUB_POS(1,1), SUB_SIZE(5,1), str, OPT_FLAT).colors(normal_btn)
       .tag(10).button(SUB_POS(6,1), SUB_SIZE(4,0.7), GET_TEXT_F(MSG_COOLDOWN));
  }
}

void HotendScreen::onRedraw(draw_mode_t what) {
  CommandProcessor cmd;

  if (what & BACKGROUND) {
    cmd.cmd(CLEAR_COLOR_RGB(bg_color))
       .cmd(CLEAR(true,true,true))
       .cmd(COLOR_RGB(bg_text_enabled))
       .tag(0)
       .font(font_medium)
       .text(CURRENT_TEMP_LBL_POS, GET_TEXT_F(MSG_TEMPERATURE));
  }

  draw_temperature(what, CURRENT_TEMP_POS);
  draw_adjuster(what, 2, getTargetTemp_celsius(E0), TARGET_TEMP_ADJ_POS);
  draw_increments(what, INCREMENTS_POS);
  draw_interaction_buttons(what);
}

bool HotendScreen::onTouchStart(uint8_t tag) {
  // Make the Momentary and Continuous buttons slightly more responsive
  switch (tag) {
    case 7: case 8:
      return HotendScreen::onTouchHeld(tag);
    default:
      return false;
  }
}

bool HotendScreen::onTouchEnd(uint8_t tag) {
  const float increment = get_increment();
  using namespace ExtUI;
  switch (tag) {
    case 1: GOTO_PREVIOUS(); break;
    case 2: UI_DECREMENT(TargetTemp_celsius, E0); break;
    case 3: UI_INCREMENT(TargetTemp_celsius, E0); break;
    case 4:
    case 5:
    case 6:
      mydata.inc_tag = tag;
      break;
    case 7:
      mydata.repeat_tag = (mydata.repeat_tag == 7) ? 0 : 7;
      break;
    case 8:
      mydata.repeat_tag = (mydata.repeat_tag == 8) ? 0 : 8;
      break;
    case 10: coolDown(); break;
    case 11: unloadFilament(); break;
    case 12: loadFilament(); break;
    case 15: GOTO_SCREEN(TemperatureScreen); break;
  }
  return true;
}

bool HotendScreen::onTouchHeld(uint8_t tag) {
  if (ExtUI::isMoving()) return false; // Don't allow moves to accumulate
  constexpr float increment = 1;
  #define UI_INCREMENT_AXIS(axis) UI_INCREMENT(AxisPosition_mm, axis);
  #define UI_DECREMENT_AXIS(axis) UI_DECREMENT(AxisPosition_mm, axis);
  switch (tag) {
    case 5: case 7: UI_DECREMENT_AXIS(E0); break;
    case 6: case 8: UI_INCREMENT_AXIS(E0); break;
    default: return false;
  }
  #undef UI_DECREMENT_AXIS
  #undef UI_INCREMENT_AXIS
  return false;
}

void HotendScreen::onIdle() {
  reset_menu_timeout();
  if (mydata.repeat_tag) onTouchHeld(mydata.repeat_tag);
  if (refresh_timer.elapsed(STATUS_UPDATE_INTERVAL)) {
    onRefresh();
    refresh_timer.start();
  }
  BaseScreen::onIdle();
}

void HotendScreen::loadFilament() {
  SpinnerDialogBox::show(GET_TEXT_F(MSG_PLEASE_WAIT));
  #ifdef MOVE_TO_FIL_CHG_COMMANDS
    gcode.process_subcommands_now(F(MOVE_TO_FIL_CHG_COMMANDS));
  #endif
  const float oldFr = getAxisMaxFeedrate_mm_s(E0);
  setAxisMaxFeedrate_mm_s(MMM_TO_MMS(1000), E0);
  unscaled_e_move(700, MMM_TO_MMS(1000)); // move E axis forward 800mm at 1000mm/m
  unscaled_e_move(70, MMM_TO_MMS(40));    // move E axis forward 70mm at 40mm/m
  setAxisMaxFeedrate_mm_s(oldFr, E0);
  SpinnerDialogBox::hide();
}

void HotendScreen::unloadFilament() {
  SpinnerDialogBox::show(GET_TEXT_F(MSG_PLEASE_WAIT));
  #ifdef MOVE_TO_FIL_CHG_COMMANDS
    gcode.process_subcommands_now(F(MOVE_TO_FIL_CHG_COMMANDS));
  #endif
  const float oldFr = getAxisMaxFeedrate_mm_s(E0);
  setAxisMaxFeedrate_mm_s(MMM_TO_MMS(1000), E0);
  unscaled_e_move(-800, MMM_TO_MMS(1000)); // move E axis backwards 800mm at 1000mm/m
  setAxisMaxFeedrate_mm_s(oldFr, E0);
  SpinnerDialogBox::hide();
}

#endif // SYNDAVER_LEVEL_HOTEND_SCREEN
