/*********************************
 * syndaver_level/tools_menu.cpp *
 *********************************/

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

#ifdef SYNDAVER_LEVEL_TOOLS_MENU

#include "autogen/tools_menu.h"
#include "autogen/layout_3_icons.h"

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

void ToolsMenu::onEntry() {
  SynLevelUI::load_background(tools_menu, sizeof(tools_menu));
}

void ToolsMenu::onRedraw(draw_mode_t what) {
  CommandProcessor cmd;
  SynLevelUI ui(cmd, what);
  ui.draw_start();
  ui.draw_bkgnd();
  ui.draw_title( POLY(status_text), F("Tools Menu"));
  ui.draw_tile(  POLY(icon_1), 1,   F(""), !isPrinting()); // Move Axis
  ui.draw_tile(  POLY(icon_2), 2,   F(""), !isPrinting()); // Leveling
  ui.draw_tile(  POLY(icon_3), 3,   F(""), !isPrinting()); // Hotend
  ui.draw_noz(   POLY(nozzle_temp));
  ui.draw_bed(   POLY(bed_temp));
  ui.draw_fan(   POLY(fan_percent));
  ui.draw_lamp(  POLY(lamp_toggle));
  ui.draw_back(  POLY(done_btn));
  ui.restore_bitmaps();
}

bool ToolsMenu::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1: GOTO_SCREEN(MoveMenu); break;
    #if HAS_LEVELING
      case 2: GOTO_SCREEN(LevelingMenu); break;
    #endif
    case 3: GOTO_SCREEN(HotendScreen); break;
    default: return SynLevelBase::onTouchEnd(tag);
  }
  return true;
}

#endif // SYNDAVER_LEVEL_TOOLS_MENU
