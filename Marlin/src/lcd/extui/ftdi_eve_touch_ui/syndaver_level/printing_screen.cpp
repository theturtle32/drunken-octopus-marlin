/**************************************
 * syndaver_level/printing_screen.cpp *
 **************************************/

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

#ifdef SYNDAVER_LEVEL_PRINTING_SCREEN

#include "../../../../feature/host_actions.h"
#include "autogen/printing_screen.h"
#include "autogen/layout_4_icons.h"

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

void PrintingScreen::onEntry() {
  SynLevelUI::load_background(printing_screen, sizeof(printing_screen));
  SaveSettingsDialogBox::promptToSaveAndStay();
}

void PrintingScreen::onRedraw(draw_mode_t what) {
  if (what & FOREGROUND) {
    const bool sdOrHostPrinting = ExtUI::isPrinting();
    const bool sdOrHostPaused   = ExtUI::isPrintingPaused();
    
    CommandProcessor cmd;
    SynLevelUI ui(cmd, what);
    ui.draw_start();
    ui.draw_bkgnd();
    ui.draw_tile(  POLY(icon_1), 1, F(""), sdOrHostPrinting); // Pause/Resume
    ui.draw_tile(  POLY(icon_2), 2, F(""), sdOrHostPrinting); // Cancel
    ui.draw_tile(  POLY(icon_3), 3, F(""), ENABLED(HAS_BED_PROBE)); // Z Offset
    ui.draw_tile(  POLY(icon_4), 4, F(""), !sdOrHostPrinting || sdOrHostPaused); // Filament Change
    ui.draw_fan(  POLY(fan_percent));
    ui.draw_encl( POLY(done_btn));
    ui.draw_prog( POLY(file_name));
    ui.draw_time( POLY(print_time));
    ui.draw_noz(  POLY(nozzle_temp));
    ui.draw_bed(  POLY(bed_temp));
    ui.draw_file( POLY(file_name));
  }
}

void PrintingScreen::setStatusMessage(FSTR_P message) {
  char buff[strlen_P((const char * const)message)+1];
  strcpy_P(buff, (const char * const) message);
  setStatusMessage((const char *) buff);
}

void PrintingScreen::setStatusMessage(const char *message) {
  if (CommandProcessor::is_processing()) {
    #if ENABLED(TOUCH_UI_DEBUG)
      SERIAL_ECHO_MSG("Cannot update status message, command processor busy");
    #endif
    return;
  }

  CommandProcessor cmd;
  cmd.cmd(CMD_DLSTART)
     .cmd(CLEAR_COLOR_RGB(bg_color))
     .cmd(CLEAR(true,true,true));

  SynLevelUI ui(cmd, BACKGROUND);
  ui.draw_bkgnd();
  ui.draw_title( POLY(status_text), message);
  ui.draw_prog( POLY(file_name));
  ui.draw_time( POLY(print_time));
  ui.draw_fan( POLY(fan_percent));
  ui.draw_encl( POLY(done_btn));
  ui.draw_noz( POLY(nozzle_temp));
  ui.draw_bed( POLY(bed_temp));
  ui.draw_file( POLY(file_name));
  ui.draw_lamp( POLY(lamp_toggle));
  ui.restore_bitmaps();

  storeBackground();

  #if ENABLED(TOUCH_UI_DEBUG)
    SERIAL_ECHO_MSG("New status message: ", message);
  #endif

  if (AT_SCREEN(PrintingScreen)) {
    current_screen.onRefresh();
  }
}

bool PrintingScreen::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 1: pauseResumePrint(); break;
    case 2: stopPrint(); break;
    case 3: bedHeight(); break;
    case 4: GOTO_SCREEN(HotendScreen); break;
    default: return SynLevelBase::onTouchEnd(tag);
  }
  return true;
}

void PrintingScreen::onIdle() {
  SynLevelBase::onIdle();
  if(!(print_job_timer.isRunning() || print_job_timer.isPaused())) CooldownScreen::showIfHot();
}

void PrintingScreen::pauseResumePrint() {
  if(ExtUI::isPrintingPaused())
    resumePrint();
  else
    pausePrint();
}

void PrintingScreen::pausePrint() {
  sound.play(twinkle, PLAY_ASYNCHRONOUS);
  if (ExtUI::isPrintingFromMedia())
    ExtUI::pausePrint();
  #ifdef ACTION_ON_PAUSE
    else hostui.pause();
  #endif
}

void PrintingScreen::resumePrint() {
  sound.play(twinkle, PLAY_ASYNCHRONOUS);
  if (ExtUI::awaitingUserConfirm())
    ExtUI::setUserConfirmed();
  else if (ExtUI::isPrintingFromMedia())
    ExtUI::resumePrint();
  #ifdef ACTION_ON_RESUME
    else hostui.resume();
  #endif
}

void PrintingScreen::stopPrint() {
  GOTO_SCREEN(ConfirmAbortPrintDialogBox);
  current_screen.forget();
  PUSH_SCREEN(StatusScreen);
}

void PrintingScreen::bedHeight() {
  #if ENABLED(HAS_BED_PROBE)
    GOTO_SCREEN(ZOffsetScreen);
  #endif
}

#endif // SYNDAVER_LEVEL_PRINTING_SCREEN
