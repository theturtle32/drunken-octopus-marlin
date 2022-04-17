/************************************
 * syndaver_level/status_screen.cpp *
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
#include "../screen_data.h"

#ifdef SYNDAVER_LEVEL_STATUS_SCREEN

#include "../archim2-flash/flash_storage.h"
#include "autogen/status_screen.h"
#include "autogen/layout_4_icons.h"

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

void StatusScreen::onStartup() {
  UIFlashStorage::initialize();
}

void StatusScreen::onEntry() {
  SynLevelUI::load_background(status_screen, sizeof(status_screen));
}

void StatusScreen::onRedraw(draw_mode_t what) {
  if (what & FOREGROUND) {
    CommandProcessor cmd;
    SynLevelUI ui(cmd, what);
    ui.draw_start();
    ui.draw_tile( POLY(icon_1), 1, F(""), !isPrinting() && isMediaInserted());  // File
    ui.draw_tile( POLY(icon_2), 2, F(""), !isPrinting() && ui.isFileSelected()); // Print
    ui.draw_tile( POLY(icon_3), 3, F("")); // Tools
    ui.draw_tile( POLY(icon_4), 4, F("")); // Settings
    ui.draw_fan(  POLY(fan_percent));
    ui.draw_encl( POLY(done_btn));
    ui.draw_time( POLY(print_time));
    ui.draw_noz(  POLY(nozzle_temp));
    ui.draw_bed(  POLY(bed_temp));
    ui.draw_prog( POLY(file_name));
    ui.draw_file( POLY(file_name));
  }
}

void StatusScreen::setStatusMessage(FSTR_P message) {
  char buff[strlen_P((const char * const)message)+1];
  strcpy_P(buff, (const char * const) message);
  setStatusMessage((const char *) buff);

  PrintingScreen::setStatusMessage(message);
}

void StatusScreen::setStatusMessage(const char *message) {
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
  ui.draw_prog(  POLY(file_name));
  ui.draw_time(  POLY(print_time));
  ui.draw_fan(   POLY(fan_percent));
  ui.draw_encl(  POLY(done_btn));
  ui.draw_noz(   POLY(nozzle_temp));
  ui.draw_bed(   POLY(bed_temp));
  ui.draw_file(  POLY(file_name));
  ui.draw_lamp(  POLY(lamp_toggle));
  ui.restore_bitmaps();

  storeBackground();

  #if ENABLED(TOUCH_UI_DEBUG)
    SERIAL_ECHO_MSG("New status message: ", message);
  #endif

  if (AT_SCREEN(StatusScreen)) {
    current_screen.onRefresh();
  }

  PrintingScreen::setStatusMessage(message);
}

bool StatusScreen::onTouchEnd(uint8_t tag) {
  switch (tag) {
    #if ENABLED(SDSUPPORT)
      case 1: GOTO_SCREEN(FilesScreen); break;
    #endif
    case 2: GOTO_SCREEN(ConfirmStartPrintDialogBox); break;
    case 3: GOTO_SCREEN(ToolsMenu); break;
    case 4: GOTO_SCREEN(SettingsMenu); break;
    case 6: GOTO_SCREEN(TemperatureScreen); break;
    default: return SynLevelBase::onTouchEnd(tag);
  }
  return true;
}

void StatusScreen::onIdle() {
  SynLevelBase::onIdle();
  if(print_job_timer.isRunning() || print_job_timer.isPaused()) GOTO_SCREEN(PrintingScreen);
}

void StatusScreen::onMediaInserted() {
  FileList list;
  list.seek(0);
  if (AT_SCREEN(StatusScreen)) current_screen.onRefresh();
}

void StatusScreen::onMediaRemoved() {
  if (AT_SCREEN(StatusScreen)) current_screen.onRefresh();
}
#endif // SYNDAVER_LEVEL_STATUS_SCREEN
