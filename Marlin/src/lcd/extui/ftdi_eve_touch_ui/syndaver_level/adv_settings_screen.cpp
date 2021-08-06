/******************************************
 * syndaver_level/adv_settings_screen.cpp *
 ******************************************/

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

#ifdef SYNDAVER_LEVEL_ADV_SETTINGS_SCREEN

using namespace FTDI;
using namespace ExtUI;
using namespace Theme;

void AdvSettingsScreen::onRedraw(draw_mode_t what) {
  widgets_t w(what);
  w.heading( F("Advanced Settings"));
  w.heading( F(""));
  w.toggle( 2, GET_TEXT_F(MSG_RUNOUT_SENSOR), getFilamentRunoutEnabled());
  w.heading( F(""));
  w.two_buttons( 3, GET_TEXT_F(MSG_LCD_ENDSTOPS), 4, GET_TEXT_F(MSG_CASE_LIGHT));
  w.heading( F(""));
  w.button( 5, GET_TEXT_F(MSG_RESTORE_DEFAULTS));
}

bool AdvSettingsScreen::onTouchHeld(uint8_t tag) {
  switch (tag) {
    case 2: setFilamentRunoutEnabled(!getFilamentRunoutEnabled()); break;
    case 3: GOTO_SCREEN(EndstopStatesScreen); break;
    #if ENABLED(CASE_LIGHT_ENABLE)
      case 4: GOTO_SCREEN(CaseLightScreen); break;
    #endif
    case 5: GOTO_SCREEN(RestoreFailsafeDialogBox); break;
    default:
      return false;
  }

  SaveSettingsDialogBox::settingsChanged();
  return true;
}

#endif // SYNDAVER_LEVEL_ADV_SETTINGS_SCREEN
