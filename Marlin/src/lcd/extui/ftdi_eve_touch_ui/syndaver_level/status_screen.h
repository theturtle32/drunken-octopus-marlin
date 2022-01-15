/**********************************
 * syndaver_level/status_screen.h *
 **********************************/

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

#define SYNDAVER_LEVEL_STATUS_SCREEN
#define SYNDAVER_LEVEL_STATUS_SCREEN_CLASS StatusScreen

class StatusScreen : public SynLevelBase, public CachedScreen<STATUS_SCREEN_CACHE,STATUS_SCREEN_DL_SIZE> {
  public:
    static void setStatusMessage(const char *);
    static void setStatusMessage(FSTR_P);
    static void onRedraw(draw_mode_t);
    static void onEntry();
    static void onStartup();
    static bool onTouchEnd(uint8_t tag);
    static void onIdle();
    static void onMediaInserted();
    static void onMediaRemoved();
};
