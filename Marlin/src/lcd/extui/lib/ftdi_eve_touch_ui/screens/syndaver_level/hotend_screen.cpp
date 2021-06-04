/************************************
 * syndaver_level/hotend_screen.cpp *
 ************************************/

/****************************************************************************
 *   Written By Marcio Teixeira 2021 - SynDaver Labs, Inc.                  *
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

#include "../../config.h"
#include "../screens.h"
#include "../screen_data.h"

#ifdef SYNDAVER_LEVEL_HOTEND_SCREEN

using namespace ExtUI;
using namespace FTDI;
using namespace Theme;

constexpr static ChangeFilamentScreenData &mydata = screen_data.ChangeFilamentScreen;

#define GRID_COLS 4
#define GRID_ROWS 6
#define FILAMENT_LABL_POS    BTN_POS(3,1),  BTN_SIZE(2,1)
#define UNLD_BTN_POS         BTN_POS(3,2),  BTN_SIZE(1,1)
#define LOAD_BTN_POS         BTN_POS(4,2),  BTN_SIZE(1,1)
#define UNLD_LABL_POS        BTN_POS(3,3),  BTN_SIZE(1,1)
#define LOAD_LABL_POS        BTN_POS(4,3),  BTN_SIZE(1,1)
#define UNLD_MOMN_POS        BTN_POS(3,4),  BTN_SIZE(1,1)
#define LOAD_MOMN_POS        BTN_POS(4,4),  BTN_SIZE(1,1)
#define UNLD_CONT_POS        BTN_POS(3,5),  BTN_SIZE(1,1)
#define LOAD_CONT_POS        BTN_POS(4,5),  BTN_SIZE(1,1)
#define COOLDOWN_POS         BTN_POS(1,6),  BTN_SIZE(2,1)
#define BACK_POS             BTN_POS(3,6),  BTN_SIZE(2,1)

#define E_TEMP_LBL_POS       BTN_POS(1,1),  BTN_SIZE(2,1)
#define E_TEMP_POS           BTN_POS(1,2),  BTN_SIZE(2,1)
#define GRADIENT_POS         BTN_POS(1,3),  BTN_SIZE(1,3)
#define LOW_TEMP_POS         BTN_POS(2,5),  BTN_SIZE(1,1)
#define MED_TEMP_POS         BTN_POS(2,4),  BTN_SIZE(1,1)
#define HIG_TEMP_POS         BTN_POS(2,3),  BTN_SIZE(1,1)
#define HEATING_LBL_POS      BTN_POS(1,5),  BTN_SIZE(1,1)
#define CAUTION_LBL_POS      BTN_POS(1,3),  BTN_SIZE(1,1)
#define HOT_LBL_POS          BTN_POS(1,5),  BTN_SIZE(1,1)

#define COOL_TEMP  40
#define LOW_TEMP  180
#define MED_TEMP  200
#define HIGH_TEMP 220

extern uint32_t getWarmColor(uint16_t temp, uint16_t cool, uint16_t low, uint16_t med, uint16_t high);

void HotendScreen::drawTempGradient(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
  CommandProcessor cmd;
  cmd.cmd(SCISSOR_XY   (x, y))
     .cmd(SCISSOR_SIZE (w, h/2))
     .gradient         (x, y,     high_rgb, x, y+h/2, med_rgb)
     .cmd(SCISSOR_XY   (x, y+h/2))
     .cmd(SCISSOR_SIZE (w, h/2))
     .gradient         (x, y+h/2, med_rgb,  x, y+h, low_rgb)
     .cmd(SCISSOR_XY   ())
     .cmd(SCISSOR_SIZE ());
}

void HotendScreen::onEntry() {
  BaseScreen::onEntry();
  mydata.e_tag = ExtUI::getActiveTool() + 10;
  mydata.t_tag = 0;
  mydata.repeat_tag = 0;
  mydata.saved_extruder = getActiveTool();
  #if FILAMENT_UNLOAD_PURGE_LENGTH > 0
    mydata.need_purge = true;
  #endif
}

void HotendScreen::onExit() {
  setActiveTool(mydata.saved_extruder, true);
}

void HotendScreen::onRedraw(draw_mode_t what) {
  CommandProcessor cmd;

  if (what & BACKGROUND) {
    cmd.cmd(CLEAR_COLOR_RGB(bg_color))
       .cmd(CLEAR(true,true,true))
       .cmd(COLOR_RGB(bg_text_enabled))
       .tag(0)
       .font(TERN(TOUCH_UI_PORTRAIT, font_large, font_medium))
       .text(E_TEMP_LBL_POS, GET_TEXT_F(MSG_TEMPERATURE));
    drawTempGradient(GRADIENT_POS);
  }

  if (what & FOREGROUND) {
    char str[15];
    const extruder_t e = getExtruder();

    if (isHeaterIdle(e))
      format_temp_and_idle(str, getActualTemp_celsius(e));
    else
      format_temp_and_temp(str, getActualTemp_celsius(e), getTargetTemp_celsius(e));

    const rgb_t tcol = getWarmColor(getActualTemp_celsius(e), COOL_TEMP, LOW_TEMP, MED_TEMP, HIGH_TEMP);
    cmd.cmd(COLOR_RGB(tcol))
       .tag(15)
       .rectangle(E_TEMP_POS)
       .cmd(COLOR_RGB(tcol.luminance() > 128 ? 0x000000 : 0xFFFFFF))
       .font(font_medium)
       .text(E_TEMP_POS, str)
       .colors(normal_btn);

    const bool t_ok = getActualTemp_celsius(e) > getSoftenTemp() - 10;

    if (mydata.t_tag && !t_ok) {
      cmd.text(HEATING_LBL_POS, GET_TEXT_F(MSG_HEATING));
    } else if (getActualTemp_celsius(e) > 100) {
      cmd.cmd(COLOR_RGB(0xFF0000))
         .text(CAUTION_LBL_POS, GET_TEXT_F(MSG_CAUTION))
         .colors(normal_btn)
         .text(HOT_LBL_POS, GET_TEXT_F(MSG_HOT));
    }

    #define TOG_STYLE(A) colors(A ? action_btn : normal_btn)

    const bool tog2  = mydata.t_tag == 2;
    const bool tog3  = mydata.t_tag == 3;
    const bool tog4  = mydata.t_tag == 4;

    if (!t_ok) reset_menu_timeout();

    const bool tog7 = mydata.repeat_tag == 7;
    const bool tog8 = mydata.repeat_tag == 8;

    {
      char str[30];
      format_temp(str, LOW_TEMP);
      cmd.tag(2) .TOG_STYLE(tog2).button (LOW_TEMP_POS, str);

      format_temp(str, MED_TEMP);
      cmd.tag(3) .TOG_STYLE(tog3).button (MED_TEMP_POS, str);

      format_temp(str, HIGH_TEMP);
      cmd.tag(4) .TOG_STYLE(tog4).button (HIG_TEMP_POS, str);
    }

    cmd.cmd(COLOR_RGB(t_ok ? bg_text_enabled : bg_text_disabled))
       .tag(0)                              .text   (FILAMENT_LABL_POS, F("Filament"))
                                            .text   (UNLD_LABL_POS,     F("Retract"))
                                            .text   (LOAD_LABL_POS,     F("Extrude"))
       .colors(normal_btn)
       .tag(10)                             .button (COOLDOWN_POS,  GET_TEXT_F(MSG_COOLDOWN))
       .tag(11)               .enabled(t_ok).button (UNLD_BTN_POS,           F("Unload"))
       .tag(12)               .enabled(t_ok).button (LOAD_BTN_POS,           F("Load"))
       .tag(5)                .enabled(t_ok).button (UNLD_MOMN_POS, GET_TEXT_F(MSG_MOMENTARY))
       .tag(6)                .enabled(t_ok).button (LOAD_MOMN_POS, GET_TEXT_F(MSG_MOMENTARY))
       .tag(7).TOG_STYLE(tog7).enabled(t_ok).button (UNLD_CONT_POS, GET_TEXT_F(MSG_CONTINUOUS))
       .tag(8).TOG_STYLE(tog8).enabled(t_ok).button (LOAD_CONT_POS, GET_TEXT_F(MSG_CONTINUOUS))
       .tag(1).colors(action_btn)           .button (BACK_POS, GET_TEXT_F(MSG_BACK));
  }
}

uint8_t HotendScreen::getSoftenTemp() {
  switch (mydata.t_tag) {
    case 2:  return LOW_TEMP;
    case 3:  return MED_TEMP;
    case 4:  return HIGH_TEMP;
    default: return EXTRUDE_MINTEMP;
  }
}

ExtUI::extruder_t HotendScreen::getExtruder() {
  return ExtUI::E0;
}

void HotendScreen::doPurge() {
  #if FILAMENT_UNLOAD_PURGE_LENGTH > 0
    constexpr float purge_distance_mm = FILAMENT_UNLOAD_PURGE_LENGTH;
    if (mydata.need_purge) {
      mydata.need_purge = false;
      MoveAxisScreen::setManualFeedrate(getExtruder(), purge_distance_mm);
      ExtUI::setAxisPosition_mm(ExtUI::getAxisPosition_mm(getExtruder()) + purge_distance_mm, getExtruder());
    }
  #endif
}

bool HotendScreen::onTouchStart(uint8_t tag) {
  // Make the Momentary and Continuous buttons slightly more responsive
  switch (tag) {
    case 5: case 6: case 7: case 8:
      #if FILAMENT_UNLOAD_PURGE_LENGTH > 0
        if (tag == 5 || tag == 7) doPurge();
      #endif
      return HotendScreen::onTouchHeld(tag);
    default:
      return false;
  }
}

bool HotendScreen::onTouchEnd(uint8_t tag) {
  using namespace ExtUI;
  switch (tag) {
    case 1: GOTO_PREVIOUS(); break;
    case 2:
    case 3:
    case 4:
      // Change temperature
      mydata.t_tag = tag;
      setTargetTemp_celsius(getSoftenTemp(), getExtruder());
      break;
    case 7:
      mydata.repeat_tag = (mydata.repeat_tag == 7) ? 0 : 7;
      break;
    case 8:
      mydata.repeat_tag = (mydata.repeat_tag == 8) ? 0 : 8;
      break;
    case 10:
      coolDown();
      TERN_(HAS_HEATED_CHAMBER, setTargetTemp_celsius(0, CHAMBER));
      break;
    case 11: SpinnerDialogBox::enqueueAndWait_P(F("M702")); break;
    case 12: SpinnerDialogBox::enqueueAndWait_P(F("M701")); break;
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
    case 5: case 7: UI_DECREMENT_AXIS(getExtruder()); break;
    case 6: case 8: UI_INCREMENT_AXIS(getExtruder()); break;
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

#endif // SYNDAVER_LEVEL_HOTEND_SCREEN
