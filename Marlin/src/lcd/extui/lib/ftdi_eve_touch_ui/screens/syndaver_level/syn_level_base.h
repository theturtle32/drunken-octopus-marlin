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
    static void draw_progress(CommandProcessor &, draw_mode_t);
    static void draw_fan(CommandProcessor &, draw_mode_t);
    static void draw_temperatures(CommandProcessor &, draw_mode_t);
    static void draw_title(CommandProcessor &, const char * const);
    static void draw_title(CommandProcessor &, progmem_str message);
    static void draw_background(CommandProcessor &cmd, const void *data, uint16_t len);
    static void restore_bitmaps(CommandProcessor &);
  public:
    static void loadBitmaps();
};
