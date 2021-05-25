/*************************************
 * syndaver_level/syn_level_base.cpp *
 *************************************/

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

#include "status_screen_layout.h"

#ifdef SYNDAVER_LEVEL_BASE

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

// Format for background image

constexpr uint8_t  format   = RGB332;
constexpr uint16_t bitmap_w = 480;
constexpr uint16_t bitmap_h = 272;

#define _ICON_POS(x,y,w,h) x, y, w/4, h
#define _TEXT_POS(x,y,w,h) x + w/4, y, w - w/4, h
#define ICON_POS(pos) _ICON_POS(pos)
#define TEXT_POS(pos) _TEXT_POS(pos)

void SynLevelBase::draw_fan(CommandProcessor &cmd, draw_mode_t what) {
  if (what & BACKGROUND) {
    // Draw Fan Percent Bitmap
    cmd.tag(5)
       .cmd (BITMAP_SOURCE(Fan_Icon_Info))
       .cmd (BITMAP_LAYOUT(Fan_Icon_Info))
       .cmd (BITMAP_SIZE  (Fan_Icon_Info))
       .icon(ICON_POS(FAN_POS), Fan_Icon_Info, icon_scale);
  }

  if (what & FOREGROUND) {
    char fan_str[20];
    sprintf_P(fan_str, PSTR("%-3d %%"), int8_t(getActualFan_percent(FAN0)));

    cmd.tag(5)
       .text(TEXT_POS(FAN_POS), fan_str);
  }
}

void SynLevelBase::draw_temperatures(CommandProcessor &cmd, draw_mode_t what) {
  if (what & BACKGROUND) {
    cmd.tag(5)
       .cmd(COLOR_RGB(daver_color));

    // Draw Extruder Bitmap on Extruder Temperature Button

    cmd.cmd (BITMAP_SOURCE(Extruder_Icon_Info))
       .cmd (BITMAP_LAYOUT(Extruder_Icon_Info))
       .cmd (BITMAP_SIZE  (Extruder_Icon_Info))
       .icon(ICON_POS(NOZ_POS), Extruder_Icon_Info, icon_scale);

    // Draw Bed Heat Bitmap on Bed Heat Button
    cmd.cmd (BITMAP_SOURCE(Bed_Heat_Icon_Info))
       .cmd (BITMAP_LAYOUT(Bed_Heat_Icon_Info))
       .cmd (BITMAP_SIZE  (Bed_Heat_Icon_Info))
       .icon(ICON_POS(BED_POS), Bed_Heat_Icon_Info, icon_scale);
  }

  if (what & FOREGROUND) {
    char e0_str[20], bed_str[20];

    format_temp(e0_str, getActualTemp_celsius(H0));
    format_temp(bed_str, getActualTemp_celsius(BED));

    cmd.tag(5)
       .font(font_medium)
       .cmd(COLOR_RGB(daver_color))
       .text(TEXT_POS(NOZ_POS), e0_str)
       .text(TEXT_POS(BED_POS), bed_str);
  }
}

void SynLevelBase::restore_bitmaps(CommandProcessor &cmd) {
  TERN_(TOUCH_UI_USE_UTF8, load_utf8_bitmaps(cmd)); // Restore font bitmap handles
}

void SynLevelBase::_format_time(char *outstr, uint32_t time) {
  const uint8_t hrs = time / 3600,
                min = (time / 60) % 60,
                sec = time % 60;
  if (hrs)
    sprintf_P(outstr, PSTR("%02d:%02d"), hrs, min);
  else
    sprintf_P(outstr, PSTR("%02d:%02ds"), min, sec);
}

void SynLevelBase::draw_progress(CommandProcessor &cmd, draw_mode_t what) {
  if (what & FOREGROUND) {
    const uint32_t elapsed = getProgress_seconds_elapsed();
    char elapsed_str[10];
    _format_time(elapsed_str, elapsed);

    cmd.cmd(COLOR_RGB(bg_text_enabled))
       .font(font_medium)
       .text(TIME_POS, elapsed_str);
  }
}

void SynLevelBase::loadBitmaps() {
  // Load the bitmaps for the status screen
  using namespace Theme;
  constexpr uint32_t base = ftdi_memory_map::RAM_G;
  CLCD::mem_write_pgm(base + TD_Icon_Info.RAMG_offset,       TD_Icon,       sizeof(TD_Icon));
  CLCD::mem_write_pgm(base + Extruder_Icon_Info.RAMG_offset, Extruder_Icon, sizeof(Extruder_Icon));
  CLCD::mem_write_pgm(base + Bed_Heat_Icon_Info.RAMG_offset, Bed_Heat_Icon, sizeof(Bed_Heat_Icon));
  CLCD::mem_write_pgm(base + Fan_Icon_Info.RAMG_offset,      Fan_Icon,      sizeof(Fan_Icon));

  // Load fonts for internationalization
  #if ENABLED(TOUCH_UI_USE_UTF8)
    load_utf8_data(base + UTF8_FONT_OFFSET);
  #endif
}

void SynLevelBase::send_buffer(CommandProcessor &cmd, const void *data, uint16_t len) {
  const char *ptr = (const char*) data;
  constexpr uint16_t block_size = 512;
  char               block[block_size];
  for(;len > 0;) {
    const uint16_t nBytes = min(len, block_size);
    memcpy_P(block, ptr, nBytes);
    cmd.write((void*)block, nBytes);
    cmd.execute();
    if(cmd.has_fault()) {
      SERIAL_ECHOLNPGM("Recovering from fault: ");
      cmd.reset();
      delay(1000);
      return;
    }
    ptr += nBytes;
    len -= nBytes;
  }
}

void SynLevelBase::load_background(const void *data, uint16_t len) {
  CommandProcessor cmd;
  cmd.inflate(BACKGROUND_OFFSET)
     .execute();
  send_buffer(cmd, data, len);
  cmd.wait();
}

void SynLevelBase::draw_background(CommandProcessor &cmd) {
  constexpr float scale_w = float(FTDI::display_width)/bitmap_w;
  constexpr float scale_h = float(FTDI::display_height)/bitmap_h;
  uint16_t linestride;
  uint32_t color;
  switch(format) {
    case RGB565: linestride = bitmap_w * 2; color = 0xFFFFFF; break;
    case RGB332: linestride = bitmap_w    ; color = 0xFFFFFF; break;
    case L1:     linestride = bitmap_w/8  ; color = 0x000000; break;
    case L2:     linestride = bitmap_w/4  ; color = 0x000000; break;
    case L4:     linestride = bitmap_w/2  ; color = 0x000000; break;
    case L8:     linestride = bitmap_w    ; color = 0x000000; break;
  }
  cmd.cmd(COLOR_RGB(color))
     .cmd(BITMAP_SOURCE(BACKGROUND_OFFSET))
     .bitmap_layout(format, linestride, bitmap_h)
     .bitmap_size(BILINEAR, BORDER, BORDER, bitmap_w*scale_w, bitmap_h*scale_h)
     .cmd(BITMAP_TRANSFORM_A(uint32_t(float(256)/scale_w)))
     .cmd(BITMAP_TRANSFORM_E(uint32_t(float(256)/scale_h)))
     .cmd(BEGIN(BITMAPS))
     .cmd(VERTEX2II(0, 0, 0, 0))
     .cmd(BITMAP_TRANSFORM_A(256))
     .cmd(BITMAP_TRANSFORM_E(256));
}

void SynLevelBase::draw_title(CommandProcessor &cmd, const char *message) {
    cmd.cmd(COLOR_RGB(bg_text_enabled));
    draw_text_box(cmd, STATUS_POS, message, OPT_CENTER, font_large);
}

void SynLevelBase::draw_title(CommandProcessor &cmd, progmem_str message) {
    cmd.cmd(COLOR_RGB(bg_text_enabled));
    draw_text_box(cmd, STATUS_POS, message, OPT_CENTER, font_large);
}

#endif // SYNDAVER_LEVEL_BASE
