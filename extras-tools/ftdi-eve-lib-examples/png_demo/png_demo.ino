/****************
 * png_demo.ino *
 ****************/

/****************************************************************************
 *   Written By Marcio Teixeira 2019 - Aleph Objects, Inc.                  *
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
 *   location: <http://www.gnu.org/licenses/>.                              *
 ****************************************************************************/

// Edit configuration in "src/config.h"

#include "src/ftdi_eve_lib/ftdi_eve_lib.h"
#include "png/screen1.h"

#include <CRC32.h>

using namespace FTDI;

//#define IS_JPG_OR_PNG

constexpr uint8_t  format   = RGB332;
constexpr uint16_t bitmap_w = 480;
constexpr uint16_t bitmap_h = 272;

/***************** SCREEN DEFINITIONS *****************/

class MainScreen : public UIScreen, public UncachedScreen {
  public:
    static void send_buffer(CommandProcessor &cmd, const void *data, uint16_t len) {
      const char *ptr = (const char*) data;
      constexpr uint16_t block_size = 512;
      char               block[block_size];
      SERIAL_ECHOLNPAIR("Writing image ", len);
      CRC32 crc;
      for(;len > 0;) {
        const uint16_t nBytes = min(len, block_size);
        memcpy_P(block, ptr, nBytes);
        crc.update((void*)block, nBytes);
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
      SERIAL_ECHOLNPAIR("Upload CRC: ", crc.finalize());   
    }
 
    static void inflate_background_png_or_jpg(CommandProcessor &cmd, const void *data, uint16_t len) {
      cmd.loadimage(0,OPT_FULLSCREEN)
         .execute();
      send_buffer(cmd, screen1, sizeof(screen1));
      cmd.cmd(BEGIN(BITMAPS))
         .cmd(VERTEX2II(0, 0, 0, 0))
         .execute();
      SERIAL_ECHOLNPGM("Waiting to complete");
      while(cmd.is_processing() && !cmd.has_fault());
      SERIAL_ECHOLNPGM("Finalized");
    }

    static void inflate_background(CommandProcessor &cmd, const void *data, uint16_t len) {
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
      cmd.inflate(0)
         .execute();
      send_buffer(cmd, data, len);
      cmd.wait();
      cmd.cmd(COLOR_RGB(color))
         .cmd(BITMAP_SOURCE(0))
         .bitmap_layout(format, linestride, bitmap_h)
         .bitmap_size(BILINEAR, BORDER, BORDER, bitmap_w*scale_w, bitmap_h*scale_h)
         .cmd(BITMAP_TRANSFORM_A(uint32_t(float(256)/scale_w)))
         .cmd(BITMAP_TRANSFORM_E(uint32_t(float(256)/scale_h)))
         .cmd(BEGIN(BITMAPS))
         .cmd(VERTEX2II(0, 0, 0, 0))
         .cmd(BITMAP_TRANSFORM_A(256))
         .cmd(BITMAP_TRANSFORM_E(256))
         .execute();
    }
 
    static void onRedraw(draw_mode_t what) {
      CommandProcessor cmd;
      cmd.cmd(CLEAR_COLOR_RGB(0xFFFFFF))
         .cmd(CLEAR(true,true,true))
         .cmd(COLOR_RGB(0xFFFFFF));
      #ifdef IS_JPG_OR_PNG
        inflate_background_png_or_jpg(cmd, screen1, sizeof(screen1));
      #else
        inflate_background(cmd, screen1, sizeof(screen1));
      #endif
    }
};

/***************** LIST OF SCREENS *****************/

SCREEN_TABLE {
  DECL_SCREEN(MainScreen),
};
SCREEN_TABLE_POST

/***************** MAIN PROGRAM *****************/

void setup() {
  Serial.begin(9600);
  Serial.println("Starting up");
  delay(5000);
  EventLoop::setup();
  CLCD::turn_on_backlight();
  SoundPlayer::set_volume(255);
}

void loop() {
  EventLoop::loop();
}
