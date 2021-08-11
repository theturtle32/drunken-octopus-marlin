/************************************
 * syndaver_level/settings_menu.cpp *
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

#ifdef SYNDAVER_LEVEL_SETTINGS_MENU

#include "autogen/settings_menu.h"
#include "autogen/layout_5_icons.h"

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

void SettingsMenu::onEntry() {
  SynLevelUI::load_background(settings_menu, sizeof(settings_menu));
}

void SettingsMenu::onRedraw(draw_mode_t what) {
  CommandProcessor cmd;
  SynLevelUI ui(cmd, what);
  ui.draw_start( );
  ui.draw_bkgnd( );
  ui.draw_title( POLY(status_text), F("Settings Menu"));
  ui.draw_tile(  POLY(icon_1), 1,   GET_TEXT_F(MSG_INFO_MENU));
  ui.draw_tile(  POLY(icon_2), 2,   GET_TEXT_F(MSG_ZPROBE_ZOFFSET), ENABLED(HAS_BED_PROBE));
  ui.draw_tile(  POLY(icon_3), 3,   GET_TEXT_F(MSG_INTERFACE));
  ui.draw_tile(  POLY(icon_4), 4,   F("Wireless Status"));
  ui.draw_tile(  POLY(icon_5), 5,   F("Advanced"));
  ui.draw_noz(   POLY(nozzle_temp));
  ui.draw_bed(   POLY(bed_temp));
  ui.draw_fan(   POLY(fan_percent));
  ui.draw_back(  POLY(done_btn));
  ui.restore_bitmaps();
}

bool SettingsMenu::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1: GOTO_SCREEN(AboutScreen); break;
    #if HAS_BED_PROBE
      case 2: GOTO_SCREEN(ZOffsetScreen); break;
    #endif
    case 3: GOTO_SCREEN(InterfaceSettingsScreen); break;
    case 4: injectCommands_P(PSTR("M118 P0 wifi_report")); break;
    case 5: GOTO_SCREEN(AdvSettingsScreen); break;
    case 6: SaveSettingsDialogBox::promptToSaveSettings(); break;
    default: return SynLevelBase::onTouchEnd(tag);
  }
  return true;
}

#endif // SYNDAVER_LEVEL_SETTINGS_MENU
