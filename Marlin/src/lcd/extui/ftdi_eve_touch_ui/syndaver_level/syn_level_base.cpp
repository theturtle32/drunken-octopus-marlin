/*************************************
 * syndaver_level/syn_level_base.cpp *
 *************************************/

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

#include "../config.h"
#include "../screens.h"

#ifdef SYNDAVER_LEVEL_BASE

using namespace FTDI;
using namespace Theme;
using namespace ExtUI;

// Format for background image

constexpr uint8_t  format   = RGB332;
constexpr uint16_t bitmap_w = 480;
constexpr uint16_t bitmap_h = 272;

#define ICON_POS(x,y,w,h) x,     y,     h, h
#define TEXT_POS(x,y,w,h) x + h, y, w - h, h

void SynLevelUI::getTempColor(uint16_t temp, uint32_t &fg_col, uint32_t &rgb_col) {
  const rgb_t cool_rgb = fg_normal;
  const rgb_t warm_rgb (255,  128,     0);
  const rgb_t hot_rgb  (255,   0,      0);
  const uint16_t cool = 40;
  const uint16_t warm = 55;
  const uint16_t hot  = 65;
  rgb_t R0, R1, mix;

  float t;
  if (temp < cool) {
    R0 = cool_rgb;
    R1 = warm_rgb;
    t  = 0;
  }
  else if (temp < warm) {
    R0 = cool_rgb;
    R1 = warm_rgb;
    t = (float(temp)-cool)/(warm-cool);
  }
  else if (temp < hot) {
    R0 = warm_rgb;
    R1 = hot_rgb;
    t = (float(temp)-warm)/(hot-warm);
  }
  else if (temp >= hot) {
    R0 = warm_rgb;
    R1 = hot_rgb;
    t = 1;
  }
  rgb_t::lerp(t, R0, R1, mix);

  // Compute a contrasting text color
  fg_col = mix;
  rgb_col = mix.luminance() > 160 ? 0x000000 : 0xFFFFFF; 
}

void SynLevelUI::_format_time(char *outstr, uint32_t time) {
  const uint8_t hrs = time / 3600,
                min = (time / 60) % 60,
                sec = time % 60;
  if (hrs)
    sprintf_P(outstr, PSTR("%02d:%02d"), hrs, min);
  else
    sprintf_P(outstr, PSTR("%02d:%02ds"), min, sec);
}

void SynLevelUI::send_buffer(CommandProcessor &cmd, const void *data, uint16_t len) {
  const char *ptr = (const char*) data;
  constexpr uint16_t block_size = 512;
  char               block[block_size];
  for(;len > 0;) {
    const uint16_t nBytes = min(len, block_size);
    memcpy_P(block, ptr, nBytes);
    cmd.write((const void*)block, nBytes);
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

void SynLevelUI::draw_start() {
  if (mode & BACKGROUND) {
    cmd.cmd(CLEAR_COLOR_RGB(bg_color))
       .cmd(CLEAR(true,true,true))
       .cmd(TAG(0));
  }
  cmd.cmd(COLOR_RGB(bg_text_enabled));
}

void SynLevelUI::draw_fan(PolyUI::poly_reader_t poly) {
  PolyUI ui(cmd, mode);
  int16_t x, y, w, h;
  ui.bounds(poly, x, y, w, h);

  if (mode & BACKGROUND) {
    // Draw Fan Percent Bitmap
    cmd.tag(7)
       .cmd(COLOR_RGB(bg_text_enabled))
       .cmd (BITMAP_SOURCE(Fan_Icon_Info))
       .cmd (BITMAP_LAYOUT(Fan_Icon_Info))
       .cmd (BITMAP_SIZE  (Fan_Icon_Info))
       .icon(ICON_POS(x, y, w, h), Fan_Icon_Info, icon_scale);
  }

  if (mode & FOREGROUND) {
    char fan_str[20];
    sprintf_P(fan_str, PSTR("%-3d %%"), int8_t(getActualFan_percent(FAN0)));

    cmd.tag(7)
       .cmd(COLOR_RGB(bg_text_enabled))
       .font(font_medium)
       .text(TEXT_POS(x, y, w, h), fan_str);
  }
}

void SynLevelUI::draw_noz(PolyUI::poly_reader_t poly, uint32_t color, uint8_t tag) {
  PolyUI ui(cmd, mode);
  int16_t x, y, w, h;
  ui.bounds(poly, x, y, w, h);

  if (mode & FOREGROUND) {
    const float temp = getActualTemp_celsius(H0);
    char e0_str[20];

    format_temp(e0_str, temp);

    uint32_t fg_col, rgb_col;
    SynLevelUI::getTempColor(temp, fg_col, rgb_col);

    cmd.tag(tag)
       .cmd (COLOR_RGB(color != -1u ? color : fg_col))
       .cmd (BITMAP_SOURCE(Extruder_Icon_Info))
       .cmd (BITMAP_LAYOUT(Extruder_Icon_Info))
       .cmd (BITMAP_SIZE  (Extruder_Icon_Info))
       .icon(ICON_POS(x, y, w, h), Extruder_Icon_Info, icon_scale)
       .font(font_medium)
       .text(TEXT_POS(x, y, w, h), e0_str)
       .cmd(COLOR_RGB(bg_text_enabled));
  }
}

void SynLevelUI::draw_bed(PolyUI::poly_reader_t poly, uint32_t color, uint8_t tag) {
  PolyUI ui(cmd, mode);
  int16_t x, y, w, h;
  ui.bounds(poly, x, y, w, h);

  if (mode & FOREGROUND) {
    const float temp = getActualTemp_celsius(BED);

    char bed_str[20];
    format_temp(bed_str, temp);

    uint32_t fg_col, rgb_col;
    SynLevelUI::getTempColor(temp, fg_col, rgb_col);

    cmd.tag(tag)
       .cmd (COLOR_RGB(color != -1u ? color : fg_col))
       .cmd (BITMAP_SOURCE(Bed_Heat_Icon_Info))
       .cmd (BITMAP_LAYOUT(Bed_Heat_Icon_Info))
       .cmd (BITMAP_SIZE  (Bed_Heat_Icon_Info))
       .icon(ICON_POS(x, y, w, h), Bed_Heat_Icon_Info, icon_scale)
       .font(font_medium)
       .text(TEXT_POS(x, y, w, h), bed_str)
       .cmd(COLOR_RGB(bg_text_enabled));
  }
}

void SynLevelUI::draw_encl(PolyUI::poly_reader_t poly, uint32_t color, uint8_t tag) {
  PolyUI ui(cmd, mode);
  int16_t x, y, w, h;
  ui.bounds(poly, x, y, w, h);

  if (mode & FOREGROUND) {
    const float temp = getActualTemp_celsius(CHAMBER);

    char chamber_str[20];
    format_temp(chamber_str, temp);

    uint32_t fg_col, rgb_col;
    SynLevelUI::getTempColor(temp, fg_col, rgb_col);

    cmd.tag(tag)
       .cmd (COLOR_RGB(color != -1u ? color : fg_col))
       .cmd (BITMAP_SOURCE(Chamber_Icon_Info))
       .cmd (BITMAP_LAYOUT(Chamber_Icon_Info))
       .cmd (BITMAP_SIZE  (Chamber_Icon_Info))
       .icon(ICON_POS(x, y, w, h), Chamber_Icon_Info, icon_scale)
       .font(font_medium)
       .text(TEXT_POS(x, y, w, h), chamber_str)
       .cmd(COLOR_RGB(bg_text_enabled));
  }
}

void SynLevelUI::draw_lamp(poly_reader_t poly, uint32_t color, uint8_t tag) {
  PolyUI ui(cmd, mode);
  int16_t x, y, w, h;
  ui.bounds(poly, x, y, w, h);

  if (mode & BACKGROUND) {
    // Hotspot for lamp toggle
    cmd.tag(tag)
       .cmd(COLOR_MASK(0,0,0,0))
       .rectangle(x, y, w, h)
       .cmd(COLOR_MASK(1,1,1,1));
  }
}

void SynLevelUI::draw_prog(PolyUI::poly_reader_t poly) {
  int16_t x, y, w, h;
  bounds(poly, x, y, w, h);

  if (mode & FOREGROUND) {
    const uint16_t bar_width = w * getProgress_percent() / 100;
    cmd.tag(8)
       .cmd(COLOR_RGB(accent_color_5))
       .rectangle(x, y, bar_width, h);
  }
}

void SynLevelUI::draw_bkgnd() {
  if (mode & BACKGROUND) {
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
       .cmd(BITMAP_TRANSFORM_E(256))
       .cmd(COLOR_RGB(bg_text_enabled));
    }
}

void SynLevelUI::draw_title(PolyUI::poly_reader_t poly, const char *message) {
  if (mode & BACKGROUND) {
    int16_t x, y, w, h;
    bounds(poly, x, y, w, h);
    cmd.cmd(COLOR_RGB(bg_text_enabled));
    draw_text_box(cmd, x, y, w, h, message, OPT_CENTER, font_large);
  }
}

void SynLevelUI::draw_title(PolyUI::poly_reader_t poly, FSTR_P message) {
  if (mode & BACKGROUND) {
    int16_t x, y, w, h;
    bounds(poly, x, y, w, h);
    cmd.cmd(COLOR_RGB(bg_text_enabled));
    draw_text_box(cmd, x, y, w, h, message, OPT_CENTER, font_large);
  }
}

void SynLevelUI::draw_file(PolyUI::poly_reader_t poly) {
  int16_t x, y, w, h;
  bounds(poly, x, y, w, h);

  if (mode & BACKGROUND) {
    cmd.tag(7)
       .cmd (BITMAP_SOURCE(File_Icon_Info))
       .cmd (BITMAP_LAYOUT(File_Icon_Info))
       .cmd (BITMAP_SIZE  (File_Icon_Info))
       .icon(ICON_POS(x, y, w, h), File_Icon_Info, icon_scale)
       .cmd(COLOR_RGB(bg_text_enabled));
  }

  if (mode & FOREGROUND) {
    cmd.cmd(COLOR_RGB(bg_text_enabled));

    if(!isMediaInserted())
      draw_text_with_ellipsis(cmd, TEXT_POS(x, y, w, h), F("No media present"), OPT_CENTERY, font_small);
    else if(isFileSelected()) {
      FileList list;
      draw_text_with_ellipsis(cmd, TEXT_POS(x, y, w, h), list.filename(), OPT_CENTERY, font_small);
    } else
      draw_text_with_ellipsis(cmd, TEXT_POS(x, y, w, h), F("No file selected"), OPT_CENTERY, font_small);
  }
}

bool SynLevelUI::isFileSelected() {
  if(!isMediaInserted()) return false;
  FileList list;
  if(list.isDir()) return false;
  const char *filename = list.filename();
  if(filename[0] == '\0') return false;
  return true;
}

void SynLevelUI::draw_time(PolyUI::poly_reader_t poly) {
  int16_t x, y, w, h;
  bounds(poly, x, y, w, h);

  if (mode & BACKGROUND) {
    cmd.cmd(COLOR_RGB(bg_text_enabled))
       .cmd (BITMAP_SOURCE(Clock_Icon_Info))
       .cmd (BITMAP_LAYOUT(Clock_Icon_Info))
       .cmd (BITMAP_SIZE  (Clock_Icon_Info))
       .icon(ICON_POS(x, y, w, h), Clock_Icon_Info, icon_scale)
       .cmd(COLOR_RGB(bg_text_enabled));
  }

  if (mode & FOREGROUND) {
    const uint32_t elapsed = getProgress_seconds_elapsed();
    char elapsed_str[10];
    _format_time(elapsed_str, elapsed);

    cmd.font(font_medium)
       .cmd(COLOR_RGB(bg_text_enabled))
       .text(TEXT_POS(x, y, w, h), elapsed_str);
  }
}

void SynLevelUI::draw_tile(PolyUI::poly_reader_t poly, uint8_t tag, FSTR_P label, bool enabled) {
  if (mode & FOREGROUND) {
    int16_t x, y, w, h;
    bounds(poly, x, y, w, h);
      
    cmd.cmd(TAG(enabled ? tag : 0))
       .cmd(COLOR_RGB(0));
    draw_text_box(cmd, x, y, w, h, label, OPT_CENTERX | OPT_BOTTOMY, font_xsmall);
    if(enabled) {
      cmd.cmd(COLOR_MASK(0,0,0,0));
    } else {
      cmd.cmd(SAVE_CONTEXT())
         .cmd(COLOR_A(224))
         .cmd(COLOR_RGB(bg_color));
    }
    cmd.rectangle(x, y, w, h);
    if(enabled)
      cmd.cmd(COLOR_MASK(1,1,1,1));
    else
      cmd.cmd(RESTORE_CONTEXT());
  }
}


void SynLevelUI::draw_button(PolyUI::poly_reader_t poly, FSTR_P label) {
  if (mode & FOREGROUND) {
    int16_t x, y, w, h;
    bounds(poly, x, y, w, h);

    cmd.colors(action_btn)
       .font(font_medium)
       .tag(6).button(x, y, w, h, label);
  }
}

void SynLevelUI::draw_back(PolyUI::poly_reader_t poly) {
  draw_button(poly, GET_TEXT_F(MSG_BUTTON_DONE));
}

void SynLevelUI::load_background(const void *data, uint16_t len) {
  CommandProcessor cmd;
  cmd.inflate(BACKGROUND_OFFSET)
     .execute();
  send_buffer(cmd, data, len);
  cmd.wait();
}

void SynLevelUI::restore_bitmaps() {
  TERN_(TOUCH_UI_USE_UTF8, load_utf8_bitmaps(cmd)); // Restore font bitmap handles
}

void SynLevelBase::loadBitmaps() {
  // Load the bitmaps for the status screen
  using namespace Theme;
  constexpr uint32_t base = ftdi_memory_map::RAM_G;
  CLCD::mem_write_xbm(base + Light_Bulb_Info.RAMG_offset,    Light_Bulb,    sizeof(Light_Bulb));
  CLCD::mem_write_xbm(base + Chamber_Icon_Info.RAMG_offset,  Chamber_Icon,  sizeof(Chamber_Icon));
  CLCD::mem_write_xbm(base + Clock_Icon_Info.RAMG_offset,    Clock_Icon,    sizeof(Clock_Icon));
  CLCD::mem_write_xbm(base + File_Icon_Info.RAMG_offset,     File_Icon,     sizeof(File_Icon));
  CLCD::mem_write_xbm(base + TD_Icon_Info.RAMG_offset,       TD_Icon,       sizeof(TD_Icon));
  CLCD::mem_write_xbm(base + Extruder_Icon_Info.RAMG_offset, Extruder_Icon, sizeof(Extruder_Icon));
  CLCD::mem_write_xbm(base + Bed_Heat_Icon_Info.RAMG_offset, Bed_Heat_Icon, sizeof(Bed_Heat_Icon));
  CLCD::mem_write_xbm(base + Fan_Icon_Info.RAMG_offset,      Fan_Icon,      sizeof(Fan_Icon));

  // Load fonts for internationalization
  #if ENABLED(TOUCH_UI_USE_UTF8)
    load_utf8_data(base + UTF8_FONT_OFFSET);
  #endif
}

void SynLevelBase::onIdle() {
  if (refresh_timer.elapsed(STATUS_UPDATE_INTERVAL)) {
    current_screen.onRefresh();
    refresh_timer.start();
  }
  BaseScreen::onIdle();
}

bool SynLevelBase::onTouchEnd(uint8_t tag) {
  switch (tag) {
    case 6: GOTO_PREVIOUS(); break;
    case 7: GOTO_SCREEN(TemperatureScreen); break;
    #if ENABLED(CASE_LIGHT_ENABLE)
      case 8: setCaseLightState(!getCaseLightState()); break;
    #endif
    default: return false;
  }
  return true;
}

#endif // SYNDAVER_LEVEL_BASE
