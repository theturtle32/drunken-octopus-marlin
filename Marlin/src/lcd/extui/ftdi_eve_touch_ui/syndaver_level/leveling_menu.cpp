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

#include "autogen/leveling_menu.h"
#include "autogen/layout_5_icons.h"

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

void LevelingMenu::onEntry() {
  SynLevelUI::load_background(leveling_menu, sizeof(leveling_menu));
}

void LevelingMenu::onRedraw(draw_mode_t what) {
  CommandProcessor cmd;
  SynLevelUI ui(cmd, what);
  ui.draw_start();
  ui.draw_bkgnd();
  ui.draw_title( POLY(status_text), F("Leveling Menu"));
  ui.draw_tile(  POLY(icon_1), 1, F("Manual Leveling"));
  ui.draw_tile(  POLY(icon_2), 2, GET_TEXT_F(MSG_PROBE_BED));
  ui.draw_tile(  POLY(icon_3), 3, GET_TEXT_F(MSG_SHOW_MESH));
  ui.draw_tile(  POLY(icon_4), 4, GET_TEXT_F(MSG_EDIT_MESH));
  ui.draw_tile(  POLY(icon_5), 5, GET_TEXT_F(MSG_PRINT_TEST));
  ui.draw_noz(   POLY(nozzle_temp));
  ui.draw_bed(   POLY(bed_temp));
  ui.draw_fan(   POLY(fan_percent));
  ui.draw_back(  POLY(done_btn));
  ui.restore_bitmaps();
}

bool LevelingMenu::onTouchEnd(uint8_t tag) {
  switch (tag) {
    #if ENABLED(AUTO_BED_LEVELING_UBL)
    case 2: BedMeshViewScreen::doProbe(); break;
    case 3: BedMeshViewScreen::show(); break;
    case 4: BedMeshEditScreen::show(); break;
    #endif
    #if ENABLED(G26_MESH_VALIDATION)
    case 5: BedMeshViewScreen::doMeshValidation(); break;
    #endif
    default: return SynLevelBase::onTouchEnd(tag);
  }
  return true;
}

#endif // SYNDAVER_LEVEL_LEVELING_MENU
