/****************************
 * drunken_octopus_extras.h *
 ****************************/

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

/******************************** EMI MITIGATION *******************************/

#if ENABLED(EMI_MITIGATION)
  void emi_init(void);
  void enable_emi_pins(const bool);
#endif

/******************************** INDEPENDENT Z AUTO-DETECT *******************************/

#if ENABLED(Z2_PRESENCE_CHECK)
  bool has_z2_jumper();
#endif

/*************************** ELECTROMAGNETIC Z BRAKE ***************************/

#if ELECTROMAGNETIC_BRAKE_PIN
  void em_brake_init();
#endif