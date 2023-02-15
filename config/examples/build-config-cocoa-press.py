#!/usr/bin/python

# Portions (c) 2019, Cocoa Press.
# Portions (c) 2019 Aleph Objects, Inc.
# Portions (c) 2019 Marcio Teixeira
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# To view a copy of the GNU General Public License, go to the following
# location: <http://www.gnu.org/licenses/>.

from __future__ import print_function
import argparse, re, sys, os

PRINTER_CHOICES = [
    "CocoaPress_Archim"
]

TOOLHEAD_CHOICES = [
    "CocoaPress_SingleExtruder"
]

usage = (
'''This script modifies Marlin's "Configuration.h" and "Configuration_adv.h" for
LulzBot printers.'''
+ "   \n\n    Printer  Choices:\n      " + "\n      ".join(PRINTER_CHOICES)
+ "   \n\n    Toolhead Choices:\n      " + "\n      ".join(TOOLHEAD_CHOICES)
)

############################ START OF CONFIGURATION ###########################

# Table of Contents:
# ------------------
#
#   1. PRINTER MODEL CHARACTERISTICS
#   2. GENERAL CONFIGURATION
#   3. EXPERIMENTAL FEATURES
#   4. CASE LIGHT SUPPORT
#   5. MOTHERBOARD AND PIN CONFIGURATION
#   6. ENDSTOP CONFIGURATION
#   7. HOMING & AXIS DIRECTIONS
#   8. PROBING OPTIONS
#   9. COCOA PRESS TOOLHEADS
#  10. TEMPERATURE SETTINGS
#  11. COOLING FAN CONFIGURATION
#  12. AXIS TRAVEL LIMITS
#  13. AUTOLEVELING / BED PROBE
#  14. FILAMENT CONFIGURATION
#  15. MOTOR DRIVER TYPE
#  16. TRINAMIC DRIVER CONFIGURATION
#  17. TRINAMIC SENSORLESS HOMING
#  18. MOTOR CURRENTS
#  19. ACCELERATION, FEEDRATES AND XYZ MOTOR STEPS
#  20. LCD OPTIONS

def C_STRING(str):
    return '"' + str.strip().replace('\n','\\n') + '"'

def make_config(PRINTER, TOOLHEAD):
    MARLIN = {}

    def ENABLED(str):
        return str in MARLIN and MARLIN[str]

################################## DEFAULTS ###################################

    USE_AUTOLEVELING                                     = True
    USE_TOUCH_UI                                         = True
    USE_COOLING_CHAMBER                                  = False

    MARLIN["SDSUPPORT"]                                  = True

    MARLIN["LIN_ADVANCE"]                                = True
    MARLIN["ADVANCE_K"]                                  = 0.0

    MARLIN["MARLIN_DEV_MODE"]                            = False
    MARLIN["USE_WATCHDOG"]                               = True

######################## PRINTER MODEL CHARACTERISTICS ########################

    MARLIN["STRING_CONFIG_H_AUTHOR"]                     = C_STRING("(Cocoa Press Marlin)")
    MARLIN["EEPROM_SETTINGS"]                            = True # EW - Enabled
    MARLIN["PRINTCOUNTER"]                               = True # EW - Enabled
    MARLIN["CUSTOM_MACHINE_NAME"]                        = C_STRING("Cocoa Press")
    MARLIN["MACHINE_UUID"]                               = C_STRING("c51664e3-50b4-40fb-9bd0-63a8cd30df18")
    MARLIN["FILAMENT_RUNOUT_SENSOR"]                     = True
    MARLIN["COCOA_PRESS_CHAMBER_COOLER"]                 = False
    MARLIN["COCOA_PRESS_CHOCOLATE_LEVEL_SENSOR"]         = False
    MARLIN["COCOA_PRESS_FIRMWARE"]                       = True
    MARLIN["DEFAULT_NOMINAL_FILAMENT_DIA"]               = 22.66

############################## CASE LIGHT SUPPORT ##############################

    MARLIN["CASE_LIGHT_ENABLE"]                          = True

###################### MOTHERBOARD AND PIN CONFIGURATION ######################

    MARLIN["CONTROLLER_FAN_PIN"]                         = 'FAN1_PIN' # Digital pin 6

    MARLIN["MOTHERBOARD"]                                = 'BOARD_ARCHIM2'
    MARLIN["SERIAL_PORT"]                                = -1

    # The host MMC bridge is impractically slow and should not be used
    if ENABLED("SDSUPPORT") or ENABLED("USB_FLASH_DRIVE_SUPPORT"):
        MARLIN["DISABLE_DUE_SD_MMC"]                     = True

############################ ENDSTOP CONFIGURATION ############################

    MARLIN["Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN"]         = True

    MARLIN["USE_XMIN_PLUG"]                              = False
    MARLIN["USE_YMIN_PLUG"]                              = False
    MARLIN["USE_ZMIN_PLUG"]                              = True

    MARLIN["USE_XMAX_PLUG"]                              = True
    MARLIN["USE_YMAX_PLUG"]                              = True
    MARLIN["USE_ZMAX_PLUG"]                              = False

    MARLIN["Y_MAX_ENDSTOP_INVERTING"]                    = 'false'

    MARLIN["SD_ABORT_ON_ENDSTOP_HIT"]                    = ENABLED("SDSUPPORT")
    MARLIN["DIRECT_PIN_CONTROL"]                         = True

########################## HOMING & AXIS DIRECTIONS ###########################

    MARLIN["COREXY"]                                     = True
    MARLIN["INVERT_X_DIR"]                               = 'false'
    MARLIN["INVERT_Y_DIR"]                               = 'false'
    MARLIN["INVERT_Z_DIR"]                               = 'false'
    MARLIN["INVERT_E0_DIR"]                              = 'false'
    MARLIN["INVERT_E1_DIR"]                              = 'false'

    MARLIN["X_HOME_DIR"]                                 =  1
    MARLIN["Y_HOME_DIR"]                                 =  1
    MARLIN["Z_HOME_DIR"]                                 = -1

    MARLIN["Z_SAFE_HOMING"]                              = True # EW - Enabled to zero z in the middle of the bed
    MARLIN["HOMING_FEEDRATE_MM_M"]                       = [50*60, 50*60, 5*60] # EW - changed Z from 4 to 6

############################# NOZZLE PARK FEATURE #############################

    MARLIN["NOZZLE_PARK_FEATURE"]                        = True
    MARLIN["NOZZLE_PARK_POINT"]                          = [0, 0, 20]

############################ COCOA PRESS TOOLHEADS ############################

    if TOOLHEAD in ["CocoaPress_SingleExtruder"]:
        MARLIN["EXTRUDERS"]                              = 1
        MARLIN["HOTENDS"]                                = 3
        MARLIN["E0_CURRENT"]                             = 960 # mA
        MARLIN["COCOA_PRESS_EXTRUDER"]                   = True

############################# TEMPERATURE SETTINGS ############################

    MARLIN["PIDTEMP"]                                    = True # EW - skipping this section for now

    MARLIN["THERMAL_PROTECTION_HOTENDS"]                 = False # EW - TEMP DISABLED
    MARLIN["PREVENT_COLD_EXTRUSION"]                     = False # EW - Turning off so we can use solenoid even when chocolate is cold
    MARLIN["EXTRUDE_MINTEMP"]                            = 10 # EW - changed from 175 to 10

    # 100 is the custom CocoaPress thermistor profile
    MARLIN["TEMP_SENSOR_0"]                              = 100
    if MARLIN["HOTENDS"] > 1:
      MARLIN["TEMP_SENSOR_1"]                            = 100
      MARLIN["TEMP_SENSOR_2"]                            = 100


    MARLIN["TOUCH_UI_LCD_TEMP_SCALING"]                  = 10 # Scale all UI temperatures by 10
    MARLIN["TOUCH_UI_LCD_TEMP_PRECISION"]                = 1  # Use one decimal point for temperatures

    # These values are scaled by 10
    MARLIN["HEATER_0_MAXTEMP"]                           = 500
    MARLIN["HEATER_1_MAXTEMP"]                           = 500
    MARLIN["HEATER_2_MAXTEMP"]                           = 500
    MARLIN["HEATER_3_MAXTEMP"]                           = 500
    MARLIN["HEATER_4_MAXTEMP"]                           = 500
    MARLIN["HEATER_5_MAXTEMP"]                           = 500
    

    MARLIN["HEATER_0_MINTEMP"]                           = -10
    MARLIN["HEATER_1_MINTEMP"]                           = -10
    MARLIN["HEATER_2_MINTEMP"]                           = -10
    MARLIN["HEATER_3_MINTEMP"]                           = -10
    MARLIN["HEATER_4_MINTEMP"]                           = -10
    MARLIN["HEATER_5_MINTEMP"]                           = -10
    

    MARLIN["THERMAL_PROTECTION_HYSTERESIS"]              = 1 # EW - changed from 4 to 1

    # Cooled chamber options

    if ENABLED("COCOA_PRESS_CHAMBER_COOLER"):
        MARLIN["TEMP_SENSOR_CHAMBER"]                    = 100
        MARLIN["CHAMBER_MAXTEMP"]                        = 500
        MARLIN["CHAMBER_MINTEMP"]                        = -10

        # Cooler cycling options

        MARLIN["COCOA_PRESS_CYCLE_COOLER"]               = True
        MARLIN["COOLER_ON_CYCLE_TIME"]                   = 4*60 # seconds
        MARLIN["COOLER_OFF_CYCLE_TIME"]                  = 1*60 # seconds

        MARLIN["HEATER_CHAMBER_INVERTING"]               = 'true' # Activate cooler when temperature is above threshold
        MARLIN["THERMAL_PROTECTION_CHAMBER"]             = False

    # Preheat options for chocolate

    MARLIN["PREHEAT_1_LABEL"]                            = C_STRING("Cocoa")
    MARLIN["COCOA_PRESS_PREHEAT_SECONDS"]                = 15*60
    MARLIN["COCOA_PRESS_PREHEAT_DARK_CHOCOLATE_INT_SCRIPT"]  = C_STRING("M104 S324 T0\nM104 S323 T1\nM141 S160")
    MARLIN["COCOA_PRESS_PREHEAT_MILK_CHOCOLATE_INT_SCRIPT"]  = C_STRING("M104 S329 T0\nM104 S328 T1\nM141 S160")
    MARLIN["COCOA_PRESS_PREHEAT_WHITE_CHOCOLATE_INT_SCRIPT"] = C_STRING("M104 S328 T0\nM104 S326 T1\nM141 S200")
    MARLIN["COCOA_PRESS_PREHEAT_DARK_CHOCOLATE_EXT_SCRIPT"]  = C_STRING("M104 S335 T2")
    MARLIN["COCOA_PRESS_PREHEAT_MILK_CHOCOLATE_EXT_SCRIPT"]  = C_STRING("M104 S327 T2")
    MARLIN["COCOA_PRESS_PREHEAT_WHITE_CHOCOLATE_EXT_SCRIPT"] = C_STRING("M104 S290 T2")

    MARLIN["COCOA_PRESS_EXTRA_HEATER"]                   = True
    MARLIN["SD_ABORT_NO_COOLDOWN"]                       = True
    MARLIN["EVENT_GCODE_SD_ABORT"]                       = C_STRING( "G0 X0 Y0")

########################## COOLING FAN CONFIGURATION ##########################

    MARLIN["USE_CONTROLLER_FAN"]                         = True

############################### AXIS TRAVEL LIMITS ###############################

    MARLIN["X_MIN_POS"]                                  = 0
    MARLIN["Y_MIN_POS"]                                  = 0
    MARLIN["Z_MAX_POS"]                                  = 150

    MARLIN["X_BED_SIZE"]                                 = 140
    MARLIN["Y_BED_SIZE"]                                 = 150

########################## AUTOLEVELING / BED PROBE ###########################

    if USE_AUTOLEVELING:
        MARLIN["FIX_MOUNTED_PROBE"]                      = True
        MARLIN["Z_CLEARANCE_DEPLOY_PROBE"]               = 15
        MARLIN["NOZZLE_TO_PROBE_OFFSET"]                 = [0, 43.3, -1.25]
        MARLIN["Z_MIN_PROBE_REPEATABILITY_TEST"]         = True # EW - enabled
        MARLIN["MESH_TEST_HOTEND_TEMP"]                  = 32 # EW - changed to 32 (celsius) Default nozzle temperature for the G26 Mesh Validation Tool.
        MARLIN["AUTO_BED_LEVELING_UBL"]                  = True
        MARLIN["RESTORE_LEVELING_AFTER_G28"]             = True
        MARLIN["GRID_MAX_POINTS_X"]                      = 5
        MARLIN["GRID_MAX_POINTS_Y"]                      = 5
        MARLIN["PROBING_MARGIN"]                         = 0
        MARLIN["MESH_INSET"]                             = 0
        MARLIN["BED_LEVELING_COMMANDS"]                  = C_STRING("G28\nG29 P1\nG29 P3\nG29 S1")
        MARLIN["G26_MESH_VALIDATION"]                    = False


############################# FILAMENT SETTINGS ############################

    MARLIN["RETRACT_LENGTH"]                             = 0 # EW - changed retract to 0

    if ENABLED("FILAMENT_RUNOUT_SENSOR"):
        MARLIN["NUM_RUNOUT_SENSORS"]                     = 1
        MARLIN["FILAMENT_RUNOUT_SCRIPT"]                 = C_STRING("M25\nM0 I'm hungry, feed me more chocolate.")
        MARLIN["FILAMENT_RUNOUT_DISTANCE_MM"]            = 50
        MARLIN["TOUCH_UI_FILAMENT_RUNOUT_WORKAROUNDS"]   = USE_TOUCH_UI

############################## MOTOR DRIVER TYPE ##############################

    DRIVER_TYPE                                          = 'TMC2130'

    # Workaround for E stepper not working on Archim 2.0
    #   https://github.com/MarlinFirmware/Marlin/issues/13040

    # For the Archim, setting this to the default (0) for TRINAMICS causes
    # the E stepper not to advance when LIN_ADVANCE is enabled, so force
    # the stepper pulse to 1 to match the other drivers.
    MARLIN["MINIMUM_STEPPER_PULSE"]                  = 1

    MARLIN["X_DRIVER_TYPE"]                              =  DRIVER_TYPE
    MARLIN["Y_DRIVER_TYPE"]                              =  DRIVER_TYPE
    MARLIN["Z_DRIVER_TYPE"]                              =  DRIVER_TYPE
    MARLIN["E0_DRIVER_TYPE"]                             =  DRIVER_TYPE

######################## TRINAMIC DRIVER CONFIGURATION ########################

    RSENSE                                           = 0.12

    MARLIN["TMC_DEBUG"]                              = True
    MARLIN["MONITOR_DRIVER_STATUS"]                  = True
    MARLIN["HOLD_MULTIPLIER"]                        = 0.5

    MARLIN["X_RSENSE"]                               = RSENSE
    MARLIN["Y_RSENSE"]                               = RSENSE
    MARLIN["Z_RSENSE"]                               = RSENSE
    MARLIN["E0_RSENSE"]                              = RSENSE

    MARLIN["TMC_USE_SW_SPI"]                         = True

    # Turn off stealthchop on Z as it seems to cause skipped steps
    MARLIN["STEALTHCHOP_Z"]                          = False

    # If LIN_ADVANCE enabled, then disable STEALTHCHOP_E, because of the
    # following bug:
    #
    # https://github.com/MarlinFirmware/Marlin/issues/17944
    #
    if ENABLED("LIN_ADVANCE"):
        MARLIN["STEALTHCHOP_E"]                      = False

################################ MOTOR CURRENTS ###############################

    # These values specify the maximum current, but actual
    # currents may be lower when used with COOLCONF

    MARLIN["X_CURRENT"]                              = 975 # mA
    MARLIN["Y_CURRENT"]                              = 975 # mA
    MARLIN["Z_CURRENT"]                              = 975 # mA

################# ACCELERATION, FEEDRATES AND XYZ MOTOR STEPS #################

    MARLIN["DEFAULT_AXIS_STEPS_PER_UNIT"]                = [80, 80, 400, 400]
    # EW - 1600 for IGUS Z changed from default of 4000
    # Z-axis leadscrew https://www.amazon.com/Witbot-Pillow-Bearing-Coupler-Printer/dp/B074Z4Q23M/ref=sr_1_4?ie=UTF8&qid=1549046242&sr=8-4&keywords=lead%20screw

    MARLIN["DEFAULT_MAX_FEEDRATE"]                       = [50, 50, 5, 20] # mm/s
    MARLIN["MANUAL_FEEDRATE"]                            = [50*60, 50*60, 5*60, 20*60]             # mm/min
    MARLIN["XY_PROBE_FEEDRATE"]                          = (50*60)                              # mm/min

    # A 32-bit board can handle more segments
    MARLIN["MIN_STEPS_PER_SEGMENT"]                      = 1

################################## LCD OPTIONS ##################################

    # Slow down SPI speed when using unshielded ribbon cables.
    MARLIN["SD_SPI_SPEED"]                               = 'SPI_SIXTEENTH_SPEED'

    MARLIN["LCD_TIMEOUT_TO_STATUS"]                      = 600000 # Ten Minutes
    MARLIN["TOUCH_UI_FTDI_EVE"]                          = True
    MARLIN["TOUCH_UI_COCOA_THEME"]                       = True
    MARLIN["TOUCH_UI_COCOA_PRESS"]                       = True
    #MARLIN["TOUCH_UI_SYNDAVER_LEVEL"]                    = True
    MARLIN["LCD_LULZBOT_CLCD_UI"]                        = True
    MARLIN["TOUCH_UI_800x480"]                           = True
    MARLIN["AO_EXP1_PINMAP"]                             = True
    MARLIN["TOUCH_UI_USE_UTF8"]                          = True
    MARLIN["TOUCH_UI_UTF8_COPYRIGHT"]                    = True
    MARLIN["TOUCH_UI_UTF8_SUPERSCRIPTS"]                 = True
    MARLIN["SCROLL_LONG_FILENAMES"]                      = True
    MARLIN["TOUCH_UI_DEBUG"]                             = False
    MARLIN["TOUCH_UI_DEVELOPER_MENU"]                    = False

    MARLIN["TOUCH_UI_FTDI_EVE"]                          = True
    MARLIN["TOUCH_UI_USE_UTF8"]                          = True
    MARLIN["TOUCH_UI_UTF8_COPYRIGHT"]                    = True
    MARLIN["TOUCH_UI_UTF8_SUPERSCRIPTS"]                 = True
    MARLIN["SET_PROGRESS_MANUALLY"]                      = True
    MARLIN["SCROLL_LONG_FILENAMES"]                      = True
    MARLIN["PAUSE_REHEAT_FAST_RESUME"]                   = True
    MARLIN["NO_TIME_AFTER_SD_PRINT"]                     = True
    MARLIN["LCD_TIMEOUT_TO_STATUS"]                      = 0

    # Virtual joystick functionality
    MARLIN["JOYSTICK"]                                   = True
    MARLIN["JOY_X_PIN"]                                  = -1
    MARLIN["JOY_Y_PIN"]                                  = -1
    MARLIN["JOY_Z_PIN"]                                  = -1
    MARLIN["JOY_EN_PIN"]                                 = -1
    MARLIN["JOY_X_LIMITS"]                               = False
    MARLIN["JOY_Y_LIMITS"]                               = False
    MARLIN["JOY_Z_LIMITS"]                               = False

    if not MARLIN["AO_EXP1_PINMAP"]:
      MARLIN["ARCHIM2_SPI_FLASH_EEPROM_BACKUP_SIZE"]     = 1000

    MARLIN["SHOW_CUSTOM_BOOTSCREEN"]                     = True
    MARLIN["BABYSTEPPING"]                               = False
    MARLIN["BABYSTEP_XY"]                                = False

    if USE_AUTOLEVELING:
      MARLIN["BABYSTEP_ZPROBE_OFFSET"]                   = False
      MARLIN["BABYSTEP_HOTEND_Z_OFFSET"]                 = False

#################################### CLEAN UP ###################################

    return MARLIN

############################## END OF CONFIGURATION #############################

def format_list(list):
    """Formats a list in C style, i.e. {0, 1, 2}"""
    return "{" + ", ".join(['{}'.format(x) for x in list]) + "}"

def do_substitions(config, counts, line):
    """Perform substitutions on a #define line from the config"""

    line = line.rstrip()

    # Separate line into line and comment
    fields  = re.split('\s*//(?!#define)', line)
    if len(fields) == 2:
        command = fields[0]
        comment = fields[1]
    else:
        command = line
        comment = ""

    m = re.match('(\s*)(?://)?(#define\s*)(\w+)((?:\(\))?\s*)(.*)', command)
    if m:
        var = m.group(3)
        separator = m.group(4)
        if len(separator) == 0:
            separator = " "
        if var in config:
            val = config[var]
            if type(val) == bool and val == True:
                new_command = m.group(1) + m.group(2) + var
            elif type(val) == bool and val == False:
                new_command = m.group(1) + "//" + m.group(2) + var + separator + m.group(5)
            else:
                if type(val) == list:
                    new_val = format_list(val)
                elif type(val) == str:
                    new_val = val
                else:
                    new_val = '{}'.format(val)

                new_command = m.group(1) + m.group(2) + var + separator + new_val

            if new_command.rstrip() != command:
              line = new_command + " // <-- changed" + ((": " + comment) if comment else "")

            counts[var] += 1

    return line

def dump_variables(config, out_filename):
    """Dump all the variables in the config in sorted order"""
    outfile = open(out_filename, "w")
    for key in sorted(config.keys()):
        val = config[key]
        if type(val) == bool and val == True:
            print("#define", key, file = outfile)
        elif type(val) == bool and val == False:
            print("//#define", key, file = outfile)
        else:
            if type(val) == list:
                val = format_list(val)
            print("#define", key, val, file = outfile)
    outfile.close()

def process_config(config, counts, in_filename, out_filename):
    """Perform substitutions on an entire file"""
    if out_filename == in_filename:
        # Handle special case of in-place substitutions
        os.rename(in_filename, in_filename + ".saved")
        in_filename = in_filename + ".saved"
    outfile = open(out_filename, "w")
    infile  = open(in_filename, "r")
    with infile as f:
        for line in f:
            line = do_substitions(config, counts, line)
            print(line, file = outfile)
    outfile.close()
    infile.close()

def init_counters(config):
    counts = {}
    for k in config:
        counts[k] = 0
    return counts

def dump_counters(config, counts):
    for k in counts:
        if counts[k] == 0:
            print("Warning:", k, "not found in any of the configuration files.", file=sys.stderr)

def invalid_printer(str):
    parser.error(str + "\n\nPrinter must be one of:\n\n   " + "\n   ".join(PRINTER_CHOICES) + "\n")

def invalid_toolhead(str):
    parser.error(str + "\n\nToolhead must be one of:\n\n   " + "\n   ".join(TOOLHEAD_CHOICES) + "\n")

def match_selection(str, list):
    # Try an exact match
    if str in list:
        return str;
    # Do a fuzzy match
    matches = [x for x in list if re.search(str, x, re.IGNORECASE)]
    if len(matches) > 1:
      # Try narrowing down the choices
      matches2 = [x for x in list if re.search("(\\b|_)" + str + "(\\b|_)", x, re.IGNORECASE)]
      if len(matches2) > 0:
        matches = matches2
    if len(matches) > 1:
        parser.error("Selection is ambiguous, must be one of:\n\n   " + "\n   ".join(matches) + "\n")
    if len(matches) == 0:
        parser.error("Invalid selection, must be one of:\n\n   " + "\n   ".join(list) + "\n")
    return matches[0]

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=usage, formatter_class=argparse.RawDescriptionHelpFormatter)
    parser.add_argument('printer', help='printer type', nargs='?')
    parser.add_argument('toolhead', help='toolhead type', nargs='?')
    parser.add_argument('-D', '--directory', help="directory to save modified files")
    parser.add_argument('-i', '--input', help='input file', action="append")
    parser.add_argument('-s', '--summary', action='store_true', help='summarize configuration')
    args = parser.parse_args()

    if not args.printer:
        invalid_printer("Must specify a printer type.")

    if not args.toolhead:
        invalid_toolhead("Must specify a toolhead type.")

    args.printer  = match_selection(args.printer,  PRINTER_CHOICES)
    args.toolhead = match_selection(args.toolhead, TOOLHEAD_CHOICES)

    if not args.input:
        args.input = [
            "../default/Configuration.h",
            "../default/Configuration_adv.h"
        ]

    if not args.directory:
        args.directory = "."

    config = make_config(args.printer, args.toolhead)

    if args.directory:
        if not os.path.exists(args.directory):
            os.makedirs(args.directory)
        counts = init_counters(config)
        for i in args.input:
            process_config(config, counts, i, args.directory + "/" + os.path.basename(i))
        dump_counters(config, counts)

        if args.summary:
            dump_variables(config, args.directory + "/Configuration_summary.txt")