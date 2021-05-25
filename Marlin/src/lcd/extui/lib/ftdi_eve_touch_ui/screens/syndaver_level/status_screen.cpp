/************************************
 * syndaver_level/status_screen.cpp *
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

#include "../../config.h"
#include "../screens.h"
#include "../screen_data.h"

#ifdef SYNDAVER_LEVEL_STATUS_SCREEN

#include "../../archim2-flash/flash_storage.h"

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

#include "status_screen_layout.h"
#include "png/status_screen.h"

#define FILE_POS         BTN1_POS
#define PRINT_POS        BTN2_POS
#define TOOLS_POS        BTN3_POS
#define SETTINGS_POS     BTN4_POS

void StatusScreen::onStartup() {
  UIFlashStorage::initialize();
}

void StatusScreen::onEntry() {
  SynLevelBase::load_background(status_screen, sizeof(status_screen));
}

void StatusScreen::onRedraw(draw_mode_t what) {
  if (what & FOREGROUND) {
    const bool has_media = isMediaInserted() && !isPrintingFromMedia();

    CommandProcessor cmd;
    cmd.colors(normal_btn)
       .font(Theme::font_medium)
       .enabled(has_media && !isPrinting())
       .tag(1).button(FILE_POS,     F("File Select"))
       .tag(2).button(PRINT_POS,    F("Print"))
       .tag(3).button(TOOLS_POS,    F("Tools"))
       .tag(4).button(SETTINGS_POS, F("Settings"));
    SynLevelBase::draw_temperatures(cmd, FOREGROUND);
    SynLevelBase::draw_fan(cmd, FOREGROUND);
    SynLevelBase::draw_progress(cmd, FOREGROUND);
  }
}

void StatusScreen::setStatusMessage(progmem_str message) {
  char buff[strlen_P((const char * const)message)+1];
  strcpy_P(buff, (const char * const) message);
  setStatusMessage((const char *) buff);
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

  SynLevelBase::draw_background(cmd);
  cmd.cmd(COLOR_RGB(bg_text_enabled));
  SynLevelBase::draw_title(cmd, message);
  SynLevelBase::draw_progress(cmd, BACKGROUND);
  SynLevelBase::draw_fan(cmd, BACKGROUND);
  SynLevelBase::draw_temperatures(cmd, BACKGROUND);
  SynLevelBase::restore_bitmaps(cmd);

  storeBackground();

  #if ENABLED(TOUCH_UI_DEBUG)
    SERIAL_ECHO_MSG("New status message: ", message);
  #endif

  if (AT_SCREEN(StatusScreen)) {
    current_screen.onRefresh();
  }
}

void StatusScreen::onIdle() {
  if (refresh_timer.elapsed(STATUS_UPDATE_INTERVAL)) {
    onRefresh();
    refresh_timer.start();
  }
  BaseScreen::onIdle();
}

bool StatusScreen::onTouchEnd(uint8_t tag) {
  switch (tag) {
    #if ENABLED(SDSUPPORT)
      case 1: GOTO_SCREEN(FilesScreen); break;
    #endif
    case 2: GOTO_SCREEN(TuneMenu); break;
    case 3:
    case 4: GOTO_SCREEN(MainMenu); break;
    case 5: GOTO_SCREEN(TemperatureScreen); break;
    default:
      return true;
  }
  return true;
}

#endif // SYNDAVER_LEVEL_STATUS_SCREEN
