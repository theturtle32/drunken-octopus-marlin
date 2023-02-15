/************************
 * cocoa_press_extras.h *
 ************************/

/****************************************************************************
 *   Written By Marcio Teixeira 2020                                        *
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

#pragma once
#include <stdbool.h>

/**************************** EXTRA HEATER CHECK ****************************/

#if ENABLED(COCOA_PRESS_EXTRA_HEATER)
  void check_extra_heater();
  bool has_extra_heater();
#endif

#if ENABLED(COCOA_PRESS_CHOCOLATE_LEVEL_SENSOR)
  float get_chocolate_fill_level();
#endif

#if ENABLED(COCOA_PRESS_CYCLE_COOLER)
  bool cycle_cooler_enabled();
  void cycle_cooler_state(bool state);
  void cycle_cooler_init();
  void cycle_cooler_idle();
#endif