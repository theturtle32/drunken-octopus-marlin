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
#include "png/screen2.h"
#include "png/screen3.h"

#include <CRC32.h>

using namespace FTDI;

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
 
    static void draw_background(CommandProcessor &cmd, const void *data, uint16_t len) {
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
      cmd.inflate(0)
         .execute();
      send_buffer(cmd, data, len);
      cmd.wait();
      cmd.cmd(BITMAP_SOURCE(0))
         .bitmap_layout(RGB565, 480*2, 272)
         .bitmap_size(BILINEAR, BORDER, BORDER, 480, 272)
         .cmd(BEGIN(BITMAPS))
         .cmd(VERTEX2II(0, 0, 0, 0))
         .execute();
    }
 
    static void onRedraw(draw_mode_t what) {
      CommandProcessor cmd;
      cmd.cmd(CLEAR_COLOR_RGB(0xFFFFFF))
         .cmd(CLEAR(true,true,true))
         .cmd(COLOR_RGB(0x000000))
         .font(30)
         .text(0, 0, display_width, display_height, F("The quick brown fox jumps over the lazy fox"))
         .cmd(COLOR_RGB(0xFFFFFF));
      //draw_background(cmd, screen1, sizeof(screen1));
      inflate_background(cmd, screen3, sizeof(screen3));
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
