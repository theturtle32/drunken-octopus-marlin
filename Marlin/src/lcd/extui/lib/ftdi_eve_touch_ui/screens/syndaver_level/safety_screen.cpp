/************************************
 * syndaver_level/safety_screen.cpp *
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

#ifdef SYNDAVER_LEVEL_SAFETY_SCREEN

using namespace FTDI;
using namespace ExtUI;
using namespace Theme;

void SafetyScreen::onRedraw(draw_mode_t what) {
  widgets_t w(what);
  w.heading( F("Safety"));
  w.toggle( 2, GET_TEXT_F(MSG_RUNOUT_SENSOR), getFilamentRunoutEnabled());
  w.toggle( 3, F("Door Switch:"), false, false);
}

bool SafetyScreen::onTouchHeld(uint8_t tag) {
  switch (tag) {
    case 2: setFilamentRunoutEnabled(!getFilamentRunoutEnabled()); break;
    default:
      return false;
  }

  SaveSettingsDialogBox::settingsChanged();
  return true;
}

#endif // SYNDAVER_LEVEL_SAFETY_SCREEN
