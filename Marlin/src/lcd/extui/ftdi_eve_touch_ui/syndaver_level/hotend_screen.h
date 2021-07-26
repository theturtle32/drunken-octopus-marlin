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
  uint8_t repeat_tag, inc_tag;
};

class HotendScreen : public BaseScreen, public CachedScreen<HOTEND_SCREEN_CACHE> {
  private:
    static uint32_t getWarmColor(uint16_t temp, uint16_t cool, uint16_t low, uint16_t med, uint16_t high);
    static void draw_temperature(draw_mode_t, int16_t x, int16_t y, int16_t w, int16_t h);
    static void draw_increments(draw_mode_t, int16_t x, int16_t y, int16_t w, int16_t h);
    static void draw_interaction_buttons(draw_mode_t);
    static void draw_adjuster(draw_mode_t, uint8_t tag, float value, int16_t x, int16_t y, int16_t w, int16_t h);
    static float get_increment();
    static void loadFilament();
    static void unloadFilament();
  public:
    static void onEntry();
    static void onRedraw(draw_mode_t);
    static bool onTouchStart(uint8_t tag);
    static bool onTouchEnd(uint8_t tag);
    static bool onTouchHeld(uint8_t tag);
    static void onIdle();
};
