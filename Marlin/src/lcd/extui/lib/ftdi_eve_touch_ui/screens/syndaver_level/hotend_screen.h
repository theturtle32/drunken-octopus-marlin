/**********************************
 * syndaver_level/hotend_screen.h *
 **********************************/

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

#pragma once

#define SYNDAVER_LEVEL_HOTEND_SCREEN
#define SYNDAVER_LEVEL_HOTEND_SCREEN_CLASS HotendScreen

struct HotendScreenData {
  uint8_t e_tag, t_tag, repeat_tag;
  ExtUI::extruder_t saved_extruder;
  #if FILAMENT_UNLOAD_PURGE_LENGTH > 0
    bool need_purge;
  #endif
};

class HotendScreen : public BaseScreen, public CachedScreen<HOTEND_SCREEN_CACHE> {
  private:
    static uint8_t getSoftenTemp();
    static ExtUI::extruder_t getExtruder();
    static void drawTempGradient(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
    static uint32_t getTempColor(uint32_t temp);
    static void doPurge();
  public:
    static void onEntry();
    static void onExit();
    static void onRedraw(draw_mode_t);
    static bool onTouchStart(uint8_t tag);
    static bool onTouchEnd(uint8_t tag);
    static bool onTouchHeld(uint8_t tag);
    static void onIdle();
};
