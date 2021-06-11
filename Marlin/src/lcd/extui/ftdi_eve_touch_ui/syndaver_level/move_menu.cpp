/********************************
 * syndaver_level/move_menu.cpp *
 ********************************/

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

#ifdef SYNDAVER_LEVEL_MOVE_MENU

#include "png/move_menu.h"

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

void MoveMenu::onEntry() {
  SynLevelBase::load_background(move_menu, sizeof(move_menu));
}

void MoveMenu::onRedraw(draw_mode_t what) {
  CommandProcessor cmd;
  SynLevelBase::draw_start( cmd, what);
  SynLevelBase::draw_bkgnd( cmd, what);
  SynLevelBase::draw_title( cmd, what, F("Move Menu"));
  SynLevelBase::draw_tile(  cmd, what, 1, F("Home (All)"));
  SynLevelBase::draw_tile(  cmd, what, 2, F("Top"));
  SynLevelBase::draw_tile(  cmd, what, 3, F("Bottom"));
  SynLevelBase::draw_tile(  cmd, what, 4, F("Maintenance Position"));
  SynLevelBase::draw_temp(  cmd, what);
  SynLevelBase::draw_back(  cmd, what);    
  SynLevelBase::restore_bitmaps(cmd);
}

bool MoveMenu::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1: SpinnerDialogBox::enqueueAndWait_P(F("G28")); break;
    case 2: raiseToTop(); break;
    case 3: lowerToBottom(); break;
    case 4: SpinnerDialogBox::enqueueAndWait_P(F("M125")); break;
    default: return SynLevelBase::onTouchEnd(tag);
  }
  return true;
}

void MoveMenu::lowerToBottom() {
  constexpr xyze_feedrate_t homing_feedrate = HOMING_FEEDRATE_MM_M;
  setAxisPosition_mm(Z_MAX_POS - 5, Z, homing_feedrate[Z_AXIS]);
}

void MoveMenu::raiseToTop() {
  constexpr xyze_feedrate_t homing_feedrate = HOMING_FEEDRATE_MM_M;
  setAxisPosition_mm(Z_MIN_POS + 5, Z, homing_feedrate[Z_AXIS]);
}

#endif // SYNDAVER_LEVEL_SETTINGS_MENU
