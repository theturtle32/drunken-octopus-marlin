/*******************************************
 * syndaver_level/endstop_state_screen.cpp *
 *******************************************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
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

#ifdef SYNDAVER_LEVEL_ENDSTOP_STATE_SCREEN

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

void EndstopStatesScreen::onEntry() {
  BaseScreen::onEntry();
  #if ENABLED(EMI_MITIGATION)
    enable_emi_pins(true);
  #endif
}

void EndstopStatesScreen::onExit() {
  BaseScreen::onExit();
  #if ENABLED(EMI_MITIGATION)
    enable_emi_pins(false);
  #endif
}

void EndstopStatesScreen::onRedraw(draw_mode_t) {
  CommandProcessor cmd;
  cmd.cmd(CLEAR_COLOR_RGB(bg_color))
     .cmd(COLOR_RGB(bg_text_enabled))
     .cmd(CLEAR(true,true,true))
     .tag(0);

  #define GRID_ROWS 4
  #define GRID_COLS 3

  #define PIN_BTN(X,Y,PIN,LABEL)          button(BTN_POS(X,Y), BTN_SIZE(1,1), LABEL)
  #define PIN_ENABLED(X,Y,LABEL,PIN,INV)  cmd.enabled(1).colors(READ(PIN##_PIN) != INV ? action_btn : normal_btn).PIN_BTN(X,Y,PIN,LABEL);

  cmd.font(
    #if ENABLED(TOUCH_UI_PORTRAIT)
      font_large
    #else
      font_medium
    #endif
  )
  .text(BTN_POS(1,1), BTN_SIZE(3,1), GET_TEXT_F(MSG_LCD_ENDSTOPS))
  .font(font_tiny);
  #if HAS_X_MIN
    PIN_ENABLED (1, 2, PSTR(STR_X_MIN), X_MIN, X_MIN_ENDSTOP_INVERTING)
  #endif
  #if HAS_Y_MAX
    PIN_ENABLED (2, 2, PSTR(STR_Y_MAX), Y_MAX, Y_MAX_ENDSTOP_INVERTING)
  #endif
  #if ENABLED(FILAMENT_RUNOUT_SENSOR) && PIN_EXISTS(FIL_RUNOUT)
    PIN_ENABLED (3, 2, GET_TEXT_F(MSG_RUNOUT_1), FIL_RUNOUT, FIL_RUNOUT1_STATE)
  #endif
  #if HAS_Z_MIN
    PIN_ENABLED (1, 3, PSTR(STR_Z_MIN), Z_MIN, Z_MIN_ENDSTOP_INVERTING)
  #endif
  #if HAS_Z_MAX
    PIN_ENABLED (2, 3, PSTR(STR_Z_MAX), Z_MAX, Z_MAX_ENDSTOP_INVERTING)
  #endif

  cmd.font(font_medium)
     .colors(action_btn)
     .tag(1).button(BTN_POS(1,4), BTN_SIZE(3,1), GET_TEXT_F(MSG_BUTTON_DONE));
  #undef GRID_COLS
  #undef GRID_ROWS
}

bool EndstopStatesScreen::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1: GOTO_PREVIOUS(); break;
    default:
      return false;
  }
  return true;
}

void EndstopStatesScreen::onIdle() {
  constexpr uint32_t DIAGNOSTICS_UPDATE_INTERVAL = 100;

  if (refresh_timer.elapsed(DIAGNOSTICS_UPDATE_INTERVAL)) {
    onRefresh();
    refresh_timer.start();
    reset_menu_timeout();
  }
  BaseScreen::onIdle();
}

#endif // SYNDAVER_LEVEL_ENDSTOP_STATE_SCREEN
