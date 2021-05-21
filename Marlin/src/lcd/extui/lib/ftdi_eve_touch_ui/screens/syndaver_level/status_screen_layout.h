/*****************************************
 * syndaver_level/status_screen_layout.h *
 *****************************************/

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

#ifdef TOUCH_UI_800x480
  #undef  EDGE_L
  #undef  EDGE_R
  #undef  EDGE_B
  #undef  MARGIN_L
  #undef  MARGIN_R
  #undef  MARGIN_T
  #undef  MARGIN_B
  #define EDGE_L         14
  #define EDGE_R         14
  #define EDGE_B         14
  #define MARGIN_L       14
  #define MARGIN_R       14
  #define MARGIN_T       14
  #define MARGIN_B       14
#endif

#define GRID_ROWS 7
#define GRID_COLS 4
#define STATUS_POS       BTN_POS(1,1), BTN_SIZE(4,2)
#define NOZ_POS          BTN_POS(1,6), BTN_SIZE(1,2)
#define BED_POS          BTN_POS(2,6), BTN_SIZE(1,2)
#define FAN_POS          BTN_POS(3,6), BTN_SIZE(1,2)
#define TIME_POS         BTN_POS(4,6), BTN_SIZE(1,2)

#define BUTTONS_POS      BTN_POS(1,3), BTN_SIZE(4,3)
#define BTN1_POS         BTN_POS(1,3), BTN_SIZE(1,3)
#define BTN2_POS         BTN_POS(2,3), BTN_SIZE(1,3)
#define BTN3_POS         BTN_POS(3,3), BTN_SIZE(1,3)
#define BTN4_POS         BTN_POS(4,3), BTN_SIZE(1,3)
#define BACK_POS         BTN_POS(3,6), BTN_SIZE(2,2)

