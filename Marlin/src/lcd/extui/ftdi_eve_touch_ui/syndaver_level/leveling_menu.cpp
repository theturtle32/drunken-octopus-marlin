/************************************
 * syndaver_level/leveling_menu.cpp *
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

#ifdef SYNDAVER_LEVEL_LEVELING_MENU

#if ENABLED(TOUCH_UI_SYNDAVER_LEVELUP)
  #include "autogen/leveling_with_auto_menu.h"
  #include "autogen/layout_5_icons.h"
  #define BACKGROUND leveling_with_auto_menu
#else
  #include "autogen/leveling_wo_auto_menu.h"
  #include "autogen/layout_4_icons.h"
  #define BACKGROUND leveling_wo_auto_menu
#endif

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

void LevelingMenu::onEntry() {
  SynLevelUI::load_background(BACKGROUND, sizeof(BACKGROUND));
}

void LevelingMenu::onRedraw(draw_mode_t what) {
  CommandProcessor cmd;
  SynLevelUI ui(cmd, what);
  ui.draw_start();
  ui.draw_bkgnd();
  ui.draw_title( POLY(status_text), F("Leveling Menu"));
  ui.draw_tile(  POLY(icon_1), 1, F("")); // Manual Leveling
  #if ENABLED(TOUCH_UI_SYNDAVER_LEVELUP)
    ui.draw_tile(  POLY(icon_2), 2, F("")); // Probe Bed
    ui.draw_tile(  POLY(icon_3), 3, F("")); // Show Mesh
    ui.draw_tile(  POLY(icon_4), 4, F("")); // Edit Mesh
    ui.draw_tile(  POLY(icon_5), 5, F("")); // Print Test
  #else
    ui.draw_tile(  POLY(icon_2), 3, F("")); // Show Mesh
    ui.draw_tile(  POLY(icon_3), 4, F("")); // Edit Mesh
    ui.draw_tile(  POLY(icon_4), 5, F("")); // Print Test
  #endif
  ui.draw_noz(   POLY(nozzle_temp));
  ui.draw_bed(   POLY(bed_temp));
  ui.draw_fan(   POLY(fan_percent));
  ui.draw_lamp(  POLY(lamp_toggle));
  ui.draw_back(  POLY(done_btn));
  ui.restore_bitmaps();
}

bool LevelingMenu::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1: GOTO_SCREEN(ConfirmManualLevelDialogBox); break;
    #if ENABLED(AUTO_BED_LEVELING_UBL)
    case 2: BedMeshViewScreen::doProbe(); break;
    case 3: BedMeshViewScreen::show(); break;
    case 4: BedMeshEditScreen::show(); break;
    #endif
    #if ENABLED(G26_MESH_VALIDATION)
    case 5:
      GOTO_SCREEN(StatusScreen);
      injectCommands_P(PSTR("G28\nM117 Heating...\nG26 R25 B75 H210 L0.5 O10 Q6\nG27"));
      break;
    #endif
    default: return SynLevelBase::onTouchEnd(tag);
  }
  return true;
}

#endif // SYNDAVER_LEVEL_LEVELING_MENU
