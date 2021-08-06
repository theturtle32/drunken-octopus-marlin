/***********************
 * cooldown_screen.cpp *
 ***********************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
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

#ifdef SYNDAVER_LEVEL_COOLDOWN_SCREEN

#include "autogen/layout_4_icons.h"

#define GRID_COLS 1
#define GRID_ROWS 3

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

void CooldownScreen::onRedraw(draw_mode_t what) {
  const float temp  = getActualTemp_celsius(H0);
  const rgb_t tcol  = SynLevelUI::getTempColor(temp);
  const rgb_t ccol  = tcol.luminance() > 128 ? 0x000000 : 0xFFFFFF;
  const bool isCool = temp < 40;

  CommandProcessor cmd;
  cmd.cmd(CLEAR_COLOR_RGB(tcol))
     .cmd(CLEAR(true,true,true))
     .tag(0)
     .cmd(COLOR_RGB(ccol));

  draw_text_box(cmd, BTN_POS(1,1), BTN_SIZE(1,2), isCool ? F("Cooling Complete") : F("Cooling..."), OPT_CENTER, font_large);
  
  SynLevelUI ui(cmd, what);
  ui.draw_noz(POLY(nozzle_temp), ccol, 0);
  ui.draw_bed(POLY(bed_temp), ccol, 0);
  if (isCool)
    ui.draw_back(POLY(done_btn));
  else
    ui.draw_button(POLY(done_btn), F("Skip"));
}

bool CooldownScreen::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 6: GOTO_SCREEN(StatusScreen); break;
    default: return false;
  }
  return true;
}

#endif // SYNDAVER_LEVEL_COOLDOWN_SCREEN

