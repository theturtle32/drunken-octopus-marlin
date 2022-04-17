/********************************
 * syndaver_level/move_menu.cpp *
 ********************************/

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

#ifdef SYNDAVER_LEVEL_MOVE_MENU

#include "autogen/move_menu.h"
#include "autogen/layout_5_icons.h"

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

void MoveMenu::onEntry() {
  SynLevelUI::load_background(move_menu, sizeof(move_menu));
}

void MoveMenu::onRedraw(draw_mode_t what) {
  CommandProcessor cmd;
  SynLevelUI ui(cmd, what);
  ui.draw_start();
  ui.draw_bkgnd();
  ui.draw_title( POLY(status_text), F("Move Menu"));
  ui.draw_tile(  POLY(icon_1), 1,   F("")); // Home (All)
  ui.draw_tile(  POLY(icon_2), 2,   F("")); // Top
  ui.draw_tile(  POLY(icon_3), 3,   F("")); // Bottom
  ui.draw_tile(  POLY(icon_4), 4,   F("")); // Maint. Position
  ui.draw_tile(  POLY(icon_5), 5,   F("")); // Custom
  ui.draw_noz(   POLY(nozzle_temp));
  ui.draw_bed(   POLY(bed_temp));
  ui.draw_fan(   POLY(fan_percent));
  ui.draw_lamp(  POLY(lamp_toggle));
  ui.draw_back(  POLY(done_btn)); 
  ui.restore_bitmaps();
}

bool MoveMenu::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1: SpinnerDialogBox::enqueueAndWait(F("G28")); break;
    #ifdef MOVE_TO_Z_MIN_COMMANDS
        case 2: SpinnerDialogBox::enqueueAndWait(F(MOVE_TO_Z_MIN_COMMANDS)); break;
    #endif
    #ifdef MOVE_TO_Z_MAX_COMMANDS
        case 3: SpinnerDialogBox::enqueueAndWait(F(MOVE_TO_Z_MAX_COMMANDS)); break;
    #endif
    #ifdef MOVE_TO_MAINT_COMMANDS
        case 4: SpinnerDialogBox::enqueueAndWait(F(MOVE_TO_MAINT_COMMANDS)); break;
    #endif
    case 5: GOTO_SCREEN(MoveScreen); break;
    default: return SynLevelBase::onTouchEnd(tag);
  }
  return true;
}

#endif // SYNDAVER_LEVEL_SETTINGS_MENU
