/***************************************
 * confirm_manual_level_dialog_box.cpp *
 ***************************************/

/****************************************************************************
 *   Written By Marcio Teixeira 2021 - SynDaver 3D                          *
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

#ifdef SYNDAVER_LEVEL_CONFIRM_MANUAL_LEVEL

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

void ConfirmManualLevelDialogBox::onRedraw(draw_mode_t) {
  drawMessage(F("Make sure the nozzle is clean before starting this process. Do you want to proceed?\n\nWARNING: Saying yes will clear the bed mesh map"));
  drawYesNoButtons();
}

bool ConfirmManualLevelDialogBox::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1:
      GOTO_PREVIOUS();
      #ifdef MANUAL_BED_LEVELING_COMMANDS
      SpinnerDialogBox::enqueueAndWait(F(MANUAL_BED_LEVELING_COMMANDS));
      #endif
      return true;
    case 2: GOTO_PREVIOUS(); return true;
    default:                 return false;
  }
}

#endif // SYNDAVER_LEVEL_CONFIRM_MANUAL_LEVEL
