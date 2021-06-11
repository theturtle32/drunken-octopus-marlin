/********************************
 * syndaver_level/tune_menu.cpp *
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

#ifdef SYNDAVER_LEVEL_TUNE_MENU

#include "../../../../feature/host_actions.h"
#include "png/tune_menu.h"

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

void TuneMenu::onEntry() {
  SynLevelBase::load_background(tune_menu, sizeof(tune_menu));
}

void TuneMenu::onRedraw(draw_mode_t what) {
  const bool sdOrHostPrinting = ExtUI::isPrinting();
  const bool sdOrHostPaused   = ExtUI::isPrintingPaused();

  CommandProcessor cmd;
  SynLevelBase::draw_start( cmd, what);
  SynLevelBase::draw_bkgnd( cmd, what);
  SynLevelBase::draw_title( cmd, what, F("Print Menu"));
  SynLevelBase::draw_tile(  cmd, what, 1, sdOrHostPaused ? GET_TEXT_F(MSG_RESUME_PRINT) : GET_TEXT_F(MSG_PAUSE_PRINT), sdOrHostPrinting);
  SynLevelBase::draw_tile(  cmd, what, 2, GET_TEXT_F(MSG_STOP_PRINT), sdOrHostPrinting);
  SynLevelBase::draw_tile(  cmd, what, 3, GET_TEXT_F(MSG_ZPROBE_ZOFFSET), ENABLED(HAS_BED_PROBE));
  SynLevelBase::draw_tile(  cmd, what, 4, GET_TEXT_F(MSG_FILAMENTCHANGE), !sdOrHostPrinting || sdOrHostPaused);
  SynLevelBase::draw_temp(  cmd, what);
  SynLevelBase::draw_back(  cmd, what);
  SynLevelBase::restore_bitmaps(cmd);
}

bool TuneMenu::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1: pauseResumePrint(); break;
    case 2: stopPrint(); break;
    case 3: bedHeight(); break;
    case 4: GOTO_SCREEN(HotendScreen); break;
    default: return SynLevelBase::onTouchEnd(tag);
  }
  return true;
}

void TuneMenu::pauseResumePrint() {
  if(ExtUI::isPrintingPaused())
    resumePrint();
  else
    pausePrint();
}

void TuneMenu::pausePrint() {
  sound.play(twinkle, PLAY_ASYNCHRONOUS);
  if (ExtUI::isPrintingFromMedia())
    ExtUI::pausePrint();
  #ifdef ACTION_ON_PAUSE
    else host_action_pause();
  #endif
  GOTO_SCREEN(StatusScreen);
}

void TuneMenu::resumePrint() {
  sound.play(twinkle, PLAY_ASYNCHRONOUS);
  if (ExtUI::awaitingUserConfirm())
    ExtUI::setUserConfirmed();
  else if (ExtUI::isPrintingFromMedia())
    ExtUI::resumePrint();
  #ifdef ACTION_ON_RESUME
    else host_action_resume();
  #endif
  GOTO_SCREEN(StatusScreen);
}

void TuneMenu::stopPrint() {
  GOTO_SCREEN(ConfirmAbortPrintDialogBox);
  current_screen.forget();
  PUSH_SCREEN(StatusScreen);
}

void TuneMenu::bedHeight() {
  #if ENABLED(HAS_BED_PROBE)
    GOTO_SCREEN(ZOffsetScreen);
  #endif
}

#endif // SYNDAVER_LEVEL_TUNE_MENU
