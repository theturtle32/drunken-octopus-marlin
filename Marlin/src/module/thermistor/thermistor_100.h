/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

// Custom Cocoa Press thermistor table, based on thermistor table 1
//multiplied by 10 for 10th degree accuracy
const temp_entry_t temptable_100[] PROGMEM = {

{OV(1),1530}, //just to add min bound
{OV(34.7297),1500},
{OV(44.3949),1400},
{OV(57.186),1300},
{OV(74.1678),1200},
{OV(96.7318),1100},
{OV(126.6755),1000},
{OV(166.0728),900},
{OV(190.0371),850},
{OV(217.1601),800},
{OV(281.6276),700},
{OV(359.7991),600},
{OV(449.0627),500},
{OV(547.2019),400},
{OV(647.8825),300},
{OV(696.5986),250},
{OV(743.0426),200},
{OV(825.6158),100},
{OV(891.5619),0},
{OV(940.2409),-100},
{OV(973.6306),-200},
{OV(995.1808),-300},
{OV(1008.2668),-400},
{OV(1015.8164),-500},
{OV(1020.0116),-600},
{OV(1023),-650}, //just to add max bound
};