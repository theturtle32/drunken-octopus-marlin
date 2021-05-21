/******************************
 * syndaver_level/tune_menu.h *
 ******************************/

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

#include "../../config.h"
#include "../screens.h"

#ifdef SYNDAVER_LEVEL_TUNE_MENU

#include "status_screen_layout.h"

#define PAUSE_POS          BTN1_POS
#define STOP_POS           BTN2_POS
#define FIL_CHANGE_POS     BTN3_POS
#define ZPROBE_ZOFFSET_POS BTN4_POS

#include "../../../../../../feature/host_actions.h"

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

void TuneMenu::onRedraw(draw_mode_t what) {
  CommandProcessor cmd;
  if (what & BACKGROUND) {
    cmd.cmd(CLEAR_COLOR_RGB(bg_color))
       .cmd(CLEAR(true,true,true));
    SynLevelBase::draw_title(cmd, F("Print Menu"));
  }
  SynLevelBase::draw_temperatures(cmd, what);
  SynLevelBase::restore_bitmaps(cmd);
  draw_buttons(cmd, what);
}

void TuneMenu::draw_buttons(CommandProcessor &cmd, draw_mode_t what) {
  if (what & FOREGROUND) {
    const bool sdOrHostPrinting = ExtUI::isPrinting();
    const bool sdOrHostPaused   = ExtUI::isPrintingPaused();

    cmd.colors(normal_btn)
       .font(font_medium)
       .enabled(sdOrHostPrinting)
       .tag(sdOrHostPaused ? 3 : 2)
       .button(PAUSE_POS, sdOrHostPaused ? GET_TEXT_F(MSG_RESUME_PRINT) : GET_TEXT_F(MSG_PAUSE_PRINT))
       .enabled(sdOrHostPrinting)
       .tag(4).button(STOP_POS, GET_TEXT_F(MSG_STOP_PRINT))
       .enabled(!sdOrHostPrinting || sdOrHostPaused)
       .tag(5).button(FIL_CHANGE_POS, GET_TEXT_F(MSG_FILAMENTCHANGE))
       .enabled(ENABLED(HAS_BED_PROBE))
       .tag(6).button(ZPROBE_ZOFFSET_POS, GET_TEXT_F(MSG_ZPROBE_ZOFFSET))
       .tag(1).colors(action_btn).button(BACK_POS, GET_TEXT_F(MSG_BACK));
  }
}

bool TuneMenu::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case  1: GOTO_PREVIOUS(); break;
    case  2: pausePrint(); break;
    case  3: resumePrint(); break;
    case  4:
      GOTO_SCREEN(ConfirmAbortPrintDialogBox);
      current_screen.forget();
      PUSH_SCREEN(StatusScreen);
      break;
    case  5: GOTO_SCREEN(ChangeFilamentScreen);  break;
    case  6:
      #if ENABLED(HAS_BED_PROBE)
        GOTO_SCREEN(ZOffsetScreen);
      #endif
      break;
    default:
      return false;
  }
  return true;
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

#endif // SYNDAVER_LEVEL_TUNE_MENU
