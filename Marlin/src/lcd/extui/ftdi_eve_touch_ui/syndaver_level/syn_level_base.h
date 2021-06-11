/***********************************
 * syndaver_level/syn_level_base.h *
 ***********************************/

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

#define SYNDAVER_LEVEL_BASE

class SynLevelBase : public BaseScreen {
  private:
    static void _format_time(char *outstr, uint32_t time);
  protected:
    static void send_buffer(CommandProcessor &cmd, const void *data, uint16_t len);
    static void load_background(const void *data, uint16_t len);

    static void draw_prog(  CommandProcessor &, draw_mode_t);
    static void draw_fan(   CommandProcessor &, draw_mode_t);
    static void draw_temp(  CommandProcessor &, draw_mode_t);
    static void draw_start( CommandProcessor &, draw_mode_t);
    static void draw_title( CommandProcessor &, draw_mode_t, const char * const);
    static void draw_title( CommandProcessor &, draw_mode_t, progmem_str message);
    static void draw_bkgnd( CommandProcessor &, draw_mode_t);
    static void draw_back(  CommandProcessor &, draw_mode_t);
    static void draw_tile(  CommandProcessor &, draw_mode_t, uint8_t tag, progmem_str label, bool enabled = true);
    static void restore_bitmaps(CommandProcessor &);
  public:
    static void loadBitmaps();
    static void onIdle();
    static bool onTouchEnd(uint8_t tag);
};
