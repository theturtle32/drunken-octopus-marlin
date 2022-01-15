/********************************
 * syndaver_level/move_screen.h *
 ********************************/

/****************************************************************************
 *   Written By Mark Pelletier  2017 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2018 - Aleph Objects, Inc.                  *
 *   Written By Marcio Teixeira 2019 - Cocoa Press                          *
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

#define SYNDAVER_LEVEL_MOVE_SCREEN
#define SYNDAVER_LEVEL_MOVE_SCREEN_CLASS MoveScreen

struct MoveScreenData {
  uint8_t inc_tag;
  float   jog_inc;
};

class MoveScreen : public BaseScreen, public CachedScreen<MOVE_SCREEN_CACHE> {
  private:
    static void draw_arrows(draw_mode_t);
    static void draw_buttons(draw_mode_t);
    static void draw_overlay_icons(draw_mode_t);
    static void draw_adjuster(draw_mode_t, uint8_t tag, FSTR_P label, float value, int16_t x, int16_t y, int16_t w, int16_t h);
    static void draw_disabled(draw_mode_t, int16_t x, int16_t y, int16_t w, int16_t h) ;
    static void draw_increments(draw_mode_t, int16_t x, int16_t y, int16_t w, int16_t h);
    static float get_increment();
    static float getManualFeedrate(uint8_t axis, float increment_mm);
    static void setManualFeedrate(ExtUI::axis_t, float increment_mm);
  public:
    static void onEntry();
    static void onRedraw(draw_mode_t);
    static bool onTouchStart(uint8_t tag);
    static bool onTouchHeld(uint8_t tag);
    static bool onTouchEnd(uint8_t tag);
    static void onIdle();
};
