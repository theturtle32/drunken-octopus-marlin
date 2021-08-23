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

float CooldownScreen::getTemp() {
  const float eTemp  = getActualTemp_celsius(H0);
  const float bTemp  = getActualTemp_celsius(BED);
  return max(eTemp,bTemp);
}

void CooldownScreen::onRedraw(draw_mode_t what) {
  const float temp = getTemp();

  uint32_t fg_col, rgb_col;
  SynLevelUI::getTempColor(temp, fg_col, rgb_col);

  CommandProcessor cmd;
  cmd.cmd(CLEAR_COLOR_RGB(fg_col))
     .cmd(CLEAR(true,true,true))
     .tag(0)
     .cmd(COLOR_RGB(rgb_col));

  draw_text_box(cmd, BTN_POS(1,1), BTN_SIZE(1,2), isCool(temp) ? F("Cooling Complete") : F("Cooling..."), OPT_CENTER, font_large);
  
  SynLevelUI ui(cmd, what);
  ui.draw_noz(POLY(nozzle_temp), rgb_col, 0);
  ui.draw_bed(POLY(bed_temp), rgb_col, 0);
  ui.draw_lamp(POLY(lamp_toggle), rgb_col);
  if (isCool(temp))
    ui.draw_back(POLY(done_btn));
  else
    ui.draw_button(POLY(done_btn), F("Skip"));
}

bool CooldownScreen::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 6: GOTO_SCREEN(StatusScreen); break;
    default: SynLevelBase::onTouchEnd(tag);
  }
  return true;
}

void CooldownScreen::showIfHot() {
  if(isCool(getTemp()))
    GOTO_SCREEN(StatusScreen);
  else
    GOTO_SCREEN(CooldownScreen);
}

#endif // SYNDAVER_LEVEL_COOLDOWN_SCREEN

