/************************************
 * syndaver_level/leveling_menu.cpp *
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

#ifdef SYNDAVER_LEVEL_LEVELING_MENU

#include "png/leveling_menu.h"

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

void LevelingMenu::onEntry() {
  SynLevelBase::load_background(leveling_menu, sizeof(leveling_menu));
}

void LevelingMenu::onRedraw(draw_mode_t what) {
  CommandProcessor cmd;
  SynLevelBase::draw_start( cmd, what);
  SynLevelBase::draw_bkgnd( cmd, what);
  SynLevelBase::draw_title( cmd, what, F("Leveling Menu"));
  SynLevelBase::draw_tile(  cmd, what, 1, GET_TEXT_F(MSG_PROBE_BED));
  SynLevelBase::draw_tile(  cmd, what, 2, GET_TEXT_F(MSG_SHOW_MESH));
  SynLevelBase::draw_tile(  cmd, what, 3, GET_TEXT_F(MSG_EDIT_MESH));
  SynLevelBase::draw_tile(  cmd, what, 4, GET_TEXT_F(MSG_PRINT_TEST));
  SynLevelBase::draw_temp(  cmd, what);
  SynLevelBase::draw_back(  cmd, what);
  SynLevelBase::restore_bitmaps(cmd);
}

bool LevelingMenu::onTouchEnd(uint8_t tag) {
  switch (tag) {
    #if ENABLED(AUTO_BED_LEVELING_UBL)
    case 1: BedMeshViewScreen::doProbe(); break;
    case 2: BedMeshViewScreen::show(); break;
    case 3: BedMeshEditScreen::show(); break;
    #endif
    #if ENABLED(G26_MESH_VALIDATION)
    case 4: BedMeshViewScreen::doMeshValidation(); break;
    #endif
    default: return SynLevelBase::onTouchEnd(tag);
  }
  return true;
}

#endif // SYNDAVER_LEVEL_LEVELING_MENU
