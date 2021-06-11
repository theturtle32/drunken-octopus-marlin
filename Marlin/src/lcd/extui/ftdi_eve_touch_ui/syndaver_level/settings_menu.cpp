/************************************
 * syndaver_level/settings_menu.cpp *
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

#include "../config.h"
#include "../screens.h"

#ifdef SYNDAVER_LEVEL_SETTINGS_MENU

#include "png/settings_menu.h"

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

void SettingsMenu::onEntry() {
  SynLevelBase::load_background(settings_menu, sizeof(settings_menu));
}

void SettingsMenu::onRedraw(draw_mode_t what) {
  CommandProcessor cmd;
  SynLevelBase::draw_start( cmd, what);
  SynLevelBase::draw_bkgnd( cmd, what);
  SynLevelBase::draw_title( cmd, what, F("Settings Menu"));
  SynLevelBase::draw_tile(  cmd, what, 1, GET_TEXT_F(MSG_INFO_MENU));
  SynLevelBase::draw_tile(  cmd, what, 2, GET_TEXT_F(MSG_INTERFACE));
  SynLevelBase::draw_tile(  cmd, what, 3, F("Safety"));
  SynLevelBase::draw_tile(  cmd, what, 4, F("Wireless"));
  SynLevelBase::draw_temp(  cmd, what);
  SynLevelBase::draw_back(  cmd, what);
  SynLevelBase::restore_bitmaps(cmd);
}

bool SettingsMenu::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1: GOTO_SCREEN(AboutScreen); break;
    case 2: GOTO_SCREEN(InterfaceSettingsScreen); LockScreen::check_passcode(); break;
    case 3: GOTO_SCREEN(SafetyScreen); break;
    case 4: GOTO_SCREEN(MainMenu); break;
    default: return SynLevelBase::onTouchEnd(tag);
  }
  return true;
}

#endif // SYNDAVER_LEVEL_SETTINGS_MENU
