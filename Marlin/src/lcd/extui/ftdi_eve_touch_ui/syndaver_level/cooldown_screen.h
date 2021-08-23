/*********************
 * cooldown_screen.h *
 *********************/

/****************************************************************************
 *   Written By Marcio Teixeira 2021 - SynDaver 3D                          *
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

#define SYNDAVER_LEVEL_COOLDOWN_SCREEN
#define SYNDAVER_LEVEL_COOLDOWN_SCREEN_CLASS CooldownScreen

class CooldownScreen : public SynLevelBase, public UncachedScreen {
    static bool isCool(float temp) {return temp < 40;}
    static float getTemp();
  public:
    static void showIfHot();
    static void onRedraw(draw_mode_t);
    static bool onTouchEnd(uint8_t tag);
};
