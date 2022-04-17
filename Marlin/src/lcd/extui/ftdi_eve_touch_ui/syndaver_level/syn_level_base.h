/***********************************
 * syndaver_level/syn_level_base.h *
 ***********************************/

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

#pragma once

#define SYNDAVER_LEVEL_BASE

#define POLY(A) PolyUI::poly_reader_t(A, sizeof(A)/sizeof(A[0]))

class SynLevelUI : public PolyUI {
  private:
    static void _format_time(char *outstr, uint32_t time);
    static void send_buffer(CommandProcessor &cmd, const void *data, uint16_t len);
  public:
    SynLevelUI(CommandProcessor &cmd, draw_mode_t what = BOTH) : PolyUI(cmd, what) {}

    static void load_background(const void *data, uint16_t len);
    static bool isFileSelected();
    static void getTempColor(uint16_t temp, uint32_t &fg_col, uint32_t &rgb_col);

    void draw_start();
    void draw_bkgnd();
    void draw_time(poly_reader_t);
    void draw_prog(poly_reader_t);
    void draw_fan(poly_reader_t);
    void draw_noz(poly_reader_t, uint32_t color = -1u, uint8_t tag = 7);
    void draw_bed(poly_reader_t, uint32_t color = -1u, uint8_t tag = 7);
    void draw_encl(poly_reader_t, uint32_t color = -1u, uint8_t tag = 7);
    void draw_lamp(poly_reader_t, uint32_t color = -1u, uint8_t tag = 8);
    void draw_title(poly_reader_t, const char * const);
    void draw_title(poly_reader_t, FSTR_P message);
    void draw_file(poly_reader_t);
    void draw_back(poly_reader_t);
    void draw_button(poly_reader_t, FSTR_P label);
    void draw_tile(poly_reader_t, uint8_t tag, FSTR_P label, bool enabled = true);
    void restore_bitmaps();
};

class SynLevelBase : public BaseScreen {
  private:
    static void _format_time(char *outstr, uint32_t time);
  public:
    static void loadBitmaps();
    static void onIdle();
    static bool onTouchEnd(uint8_t tag);
};
