/* Portions (c) 2019, Cocoa Press.
 * Portions (c) 2019 Aleph Objects, Inc.
 * Portions (c) 2019 Marcio Teixeira
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
 * To view a copy of the GNU General Public License, go to the following
 * location: <http://www.gnu.org/licenses/>.
 */

const path = require('path');
const fsOrig = require('fs');
const fs = fsOrig.promises;


const PRINTER_CHOICES = [
    "CocoaPress_Archim"
]

const TOOLHEAD_CHOICES = [
    "CocoaPress_SingleExtruder"
]

/************************** START OF CONFIGURATION **************************
 * Table of Contents:
 * ------------------
 *  1. PRINTER MODEL CHARACTERISTICS
 *  2. GENERAL CONFIGURATION
 *  3. EXPERIMENTAL FEATURES
 *  4. CASE LIGHT SUPPORT
 *  5. MOTHERBOARD AND PIN CONFIGURATION
 *  6. ENDSTOP CONFIGURATION
 *  7. HOMING & AXIS DIRECTIONS
 *  8. PROBING OPTIONS
 *  9. COCOA PRESS TOOLHEADS
 * 10. TEMPERATURE SETTINGS
 * 11. COOLING FAN CONFIGURATION
 * 12. AXIS TRAVEL LIMITS
 * 13. AUTOLEVELING / BED PROBE
 * 14. FILAMENT CONFIGURATION
 * 15. MOTOR DRIVER TYPE
 * 16. TRINAMIC DRIVER CONFIGURATION
 * 17. TRINAMIC SENSORLESS HOMING
 * 18. MOTOR CURRENTS
 * 19. ACCELERATION, FEEDRATES AND XYZ MOTOR STEPS
 * 20. LCD OPTIONS
 */

function C_STRING(str) {
    return "\"" + str.replace(/\n$/g, "").replace(/^\n/g, "").replace(/\n/g, "\\n") + "\"";
}

function format_val(val) {
    // Formats a list in C style, i.e. {0, 1, 2}
    const limitPrecision = (key, val) => val.toFixed ? Number(val.toFixed(2)) : val;
    return typeof val == 'string' ? val : JSON.stringify(val, limitPrecision).replace(/\[/g,'{').replace(/\]/g,'}')
}

function format(str, ...args) {
    return str.replace(/{}/g, () => format_val(args.shift()));
}

function make_config(PRINTER, TOOLHEAD) {
    MARLIN = {}

    const ENABLED = str => MARLIN.hasOwnProperty(str) && MARLIN[str];

/********************************* DEFAULTS **********************************/

    USE_AUTOLEVELING                                     = true
    USE_TOUCH_UI                                         = true
    USE_COOLING_CHAMBER                                  = false

    MARLIN["SDSUPPORT"]                                  = true

    MARLIN["LIN_ADVANCE"]                                = true
    MARLIN["ADVANCE_K"]                                  = 0.0

    MARLIN["MARLIN_DEV_MODE"]                            = false
    MARLIN["USE_WATCHDOG"]                               = true

/*********************** PRINTER MODEL CHARACTERISTICS ***********************/

    MARLIN["STRING_CONFIG_H_AUTHOR"]                     = C_STRING("(Cocoa Press Marlin)")
    MARLIN["EEPROM_SETTINGS"]                            = true // EW - Enabled
    MARLIN["PRINTCOUNTER"]                               = true // EW - Enabled
    MARLIN["CUSTOM_MACHINE_NAME"]                        = C_STRING("Cocoa Press")
    MARLIN["MACHINE_UUID"]                               = C_STRING("c51664e3-50b4-40fb-9bd0-63a8cd30df18")
    MARLIN["FILAMENT_RUNOUT_SENSOR"]                     = true
    MARLIN["COCOA_PRESS_CHAMBER_COOLER"]                 = false
    MARLIN["COCOA_PRESS_CHOCOLATE_LEVEL_SENSOR"]         = false
    MARLIN["COCOA_PRESS_FIRMWARE"]                       = true
    MARLIN["DEFAULT_NOMINAL_FILAMENT_DIA"]               = 22.66

/***************************** CASE LIGHT SUPPORT *****************************/

    MARLIN["CASE_LIGHT_ENABLE"]                          = true

/********************* MOTHERBOARD AND PIN CONFIGURATION *********************/

    MARLIN["CONTROLLER_FAN_PIN"]                         = 'FAN1_PIN' // Digital pin 6

    MARLIN["MOTHERBOARD"]                                = 'BOARD_ARCHIM2'
    MARLIN["SERIAL_PORT"]                                = -1

    // The host MMC bridge is impractically slow and should not be used
    if (ENABLED("SDSUPPORT") || ENABLED("USB_FLASH_DRIVE_SUPPORT")) {
        MARLIN["DISABLE_DUE_SD_MMC"]                     = true
    }

/*************************** ENDSTOP CONFIGURATION ***************************/

    MARLIN["Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN"]         = true

    MARLIN["USE_XMIN_PLUG"]                              = false
    MARLIN["USE_YMIN_PLUG"]                              = false
    MARLIN["USE_ZMIN_PLUG"]                              = true

    MARLIN["USE_XMAX_PLUG"]                              = true
    MARLIN["USE_YMAX_PLUG"]                              = true
    MARLIN["USE_ZMAX_PLUG"]                              = false

    MARLIN["Y_MAX_ENDSTOP_INVERTING"]                    = 'false'

    MARLIN["SD_ABORT_ON_ENDSTOP_HIT"]                    = ENABLED("SDSUPPORT")
    MARLIN["DIRECT_PIN_CONTROL"]                         = true

/************************* HOMING & AXIS DIRECTIONS **************************/

    MARLIN["COREXY"]                                     = true
    MARLIN["INVERT_X_DIR"]                               = 'false'
    MARLIN["INVERT_Y_DIR"]                               = 'false'
    MARLIN["INVERT_Z_DIR"]                               = 'false'
    MARLIN["INVERT_E0_DIR"]                              = 'false'
    MARLIN["INVERT_E1_DIR"]                              = 'false'

    MARLIN["X_HOME_DIR"]                                 =  1
    MARLIN["Y_HOME_DIR"]                                 =  1
    MARLIN["Z_HOME_DIR"]                                 = -1

    MARLIN["Z_SAFE_HOMING"]                              = true // EW - Enabled to zero z in the middle of the bed
    MARLIN["HOMING_FEEDRATE_MM_M"]                       = [50*60, 50*60, 5*60] // EW - changed Z from 4 to 6

/**************************** NOZZLE PARK FEATURE ****************************/

    MARLIN["NOZZLE_PARK_FEATURE"]                        = true
    MARLIN["NOZZLE_PARK_POINT"]                          = [0, 0, 20]

/*************************** COCOA PRESS TOOLHEADS ***************************/

    if(["CocoaPress_SingleExtruder"].includes(TOOLHEAD)) {
        MARLIN["EXTRUDERS"]                              = 1
        MARLIN["HOTENDS"]                                = 3
        MARLIN["E0_CURRENT"]                             = 960 // mA
        MARLIN["COCOA_PRESS_EXTRUDER"]                   = true
    }

/**************************** TEMPERATURE SETTINGS ***************************/

    MARLIN["PIDTEMP"]                                    = true // EW - skipping this section for now

    MARLIN["THERMAL_PROTECTION_HOTENDS"]                 = false // EW - TEMP DISABLED
    MARLIN["PREVENT_COLD_EXTRUSION"]                     = false // EW - Turning off so we can use solenoid even when chocolate is cold
    MARLIN["EXTRUDE_MINTEMP"]                            = 10 // EW - changed from 175 to 10

    // 100 is the custom CocoaPress thermistor profile
    MARLIN["TEMP_SENSOR_0"]                              = 100
    if(MARLIN["HOTENDS"] > 1) {
      MARLIN["TEMP_SENSOR_1"]                            = 100
      MARLIN["TEMP_SENSOR_2"]                            = 100
    }

    MARLIN["TOUCH_UI_LCD_TEMP_SCALING"]                  = 10 // Scale all UI temperatures by 10
    MARLIN["TOUCH_UI_LCD_TEMP_PRECISION"]                = 1  // Use one decimal point for temperatures

    // These values are scaled by 10
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
    

    MARLIN["THERMAL_PROTECTION_HYSTERESIS"]              = 1 // EW - changed from 4 to 1

    // Cooled chamber options

    if(ENABLED("COCOA_PRESS_CHAMBER_COOLER")) {
        MARLIN["TEMP_SENSOR_CHAMBER"]                    = 100
        MARLIN["CHAMBER_MAXTEMP"]                        = 500
        MARLIN["CHAMBER_MINTEMP"]                        = -10

        // Cooler cycling options

        MARLIN["COCOA_PRESS_CYCLE_COOLER"]               = true
        MARLIN["COOLER_ON_CYCLE_TIME"]                   = 4*60 // seconds
        MARLIN["COOLER_OFF_CYCLE_TIME"]                  = 1*60 // seconds

        MARLIN["HEATER_CHAMBER_INVERTING"]               = 'true' // Activate cooler when temperature is above threshold
        MARLIN["THERMAL_PROTECTION_CHAMBER"]             = false
    }

    // Preheat options for chocolate

    MARLIN["PREHEAT_1_LABEL"]                            = C_STRING("Cocoa")
    MARLIN["COCOA_PRESS_PREHEAT_SECONDS"]                = 15*60
    MARLIN["COCOA_PRESS_PREHEAT_DARK_CHOCOLATE_INT_SCRIPT"]  = C_STRING("M104 S324 T0\nM104 S323 T1\nM141 S160")
    MARLIN["COCOA_PRESS_PREHEAT_MILK_CHOCOLATE_INT_SCRIPT"]  = C_STRING("M104 S329 T0\nM104 S328 T1\nM141 S160")
    MARLIN["COCOA_PRESS_PREHEAT_WHITE_CHOCOLATE_INT_SCRIPT"] = C_STRING("M104 S328 T0\nM104 S326 T1\nM141 S200")
    MARLIN["COCOA_PRESS_PREHEAT_DARK_CHOCOLATE_EXT_SCRIPT"]  = C_STRING("M104 S335 T2")
    MARLIN["COCOA_PRESS_PREHEAT_MILK_CHOCOLATE_EXT_SCRIPT"]  = C_STRING("M104 S327 T2")
    MARLIN["COCOA_PRESS_PREHEAT_WHITE_CHOCOLATE_EXT_SCRIPT"] = C_STRING("M104 S290 T2")

    MARLIN["COCOA_PRESS_EXTRA_HEATER"]                   = false
    MARLIN["SD_ABORT_NO_COOLDOWN"]                       = true
    MARLIN["EVENT_GCODE_SD_ABORT"]                       = C_STRING( "G0 X0 Y0")

/************************* COOLING FAN CONFIGURATION *************************/

    MARLIN["USE_CONTROLLER_FAN"]                         = true

/****************************** AXIS TRAVEL LIMITS ******************************/

    MARLIN["X_MIN_POS"]                                  = 0
    MARLIN["Y_MIN_POS"]                                  = 0
    MARLIN["Z_MAX_POS"]                                  = 150

    MARLIN["X_BED_SIZE"]                                 = 140
    MARLIN["Y_BED_SIZE"]                                 = 150

/************************** AUTOLEVELING / BED PROBE *************************/

    if(USE_AUTOLEVELING) {
        MARLIN["FIX_MOUNTED_PROBE"]                      = true
        MARLIN["Z_CLEARANCE_DEPLOY_PROBE"]               = 15
        MARLIN["NOZZLE_TO_PROBE_OFFSET"]                 = [0, 43.3, -1.25]
        MARLIN["Z_MIN_PROBE_REPEATABILITY_TEST"]         = true // EW - enabled
        MARLIN["MESH_TEST_HOTEND_TEMP"]                  = 32 // EW - changed to 32 (celsius) Default nozzle temperature for the G26 Mesh Validation Tool.
        MARLIN["AUTO_BED_LEVELING_UBL"]                  = true
        MARLIN["RESTORE_LEVELING_AFTER_G28"]             = true
        MARLIN["GRID_MAX_POINTS_X"]                      = 5
        MARLIN["GRID_MAX_POINTS_Y"]                      = 5
        MARLIN["PROBING_MARGIN"]                         = 0
        MARLIN["MESH_INSET"]                             = 0
        MARLIN["BED_LEVELING_COMMANDS"]                  = C_STRING("G28\nG29 P1\nG29 P3\nG29 S1")
        MARLIN["G26_MESH_VALIDATION"]                    = false
    }


/**************************** FILAMENT SETTINGS ***************************/

    MARLIN["RETRACT_LENGTH"]                             = 0 // EW - changed retract to 0

    if(ENABLED("FILAMENT_RUNOUT_SENSOR")) {
        MARLIN["NUM_RUNOUT_SENSORS"]                     = 1
        MARLIN["FILAMENT_RUNOUT_SCRIPT"]                 = C_STRING("M25\nM0 I'm hungry, feed me more chocolate.")
        MARLIN["FILAMENT_RUNOUT_DISTANCE_MM"]            = 50
        MARLIN["TOUCH_UI_FILAMENT_RUNOUT_WORKAROUNDS"]   = USE_TOUCH_UI
    }

/***************************** MOTOR DRIVER TYPE *****************************/

    DRIVER_TYPE                                          = 'TMC2130'

    /* Workaround for E stepper not working on Archim 2.0
     *   https://github.com/MarlinFirmware/Marlin/issues/13040
     * 
     * For the Archim, setting this to the default (0) for TRINAMICS causes
     * the E stepper not to advance when LIN_ADVANCE is enabled, so force
     * the stepper pulse to 1 to match the other drivers.
     */
    MARLIN["MINIMUM_STEPPER_PULSE"]                  = 1

    MARLIN["X_DRIVER_TYPE"]                              =  DRIVER_TYPE
    MARLIN["Y_DRIVER_TYPE"]                              =  DRIVER_TYPE
    MARLIN["Z_DRIVER_TYPE"]                              =  DRIVER_TYPE
    MARLIN["E0_DRIVER_TYPE"]                             =  DRIVER_TYPE

/*********************** TRINAMIC DRIVER CONFIGURATION ***********************/

    RSENSE                                           = 0.12

    MARLIN["TMC_DEBUG"]                              = true
    MARLIN["MONITOR_DRIVER_STATUS"]                  = true
    MARLIN["HOLD_MULTIPLIER"]                        = 0.5

    MARLIN["X_RSENSE"]                               = RSENSE
    MARLIN["Y_RSENSE"]                               = RSENSE
    MARLIN["Z_RSENSE"]                               = RSENSE
    MARLIN["E0_RSENSE"]                              = RSENSE

    MARLIN["TMC_USE_SW_SPI"]                         = true

    // Turn off stealthchop on Z as it seems to cause skipped steps
    MARLIN["STEALTHCHOP_Z"]                          = false

    /* If LIN_ADVANCE enabled, then disable STEALTHCHOP_E, because of the
     * following bug:
     *
     * https://github.com/MarlinFirmware/Marlin/issues/17944
     */
    if (ENABLED("LIN_ADVANCE")) {
        MARLIN["STEALTHCHOP_E"]                      = false
    }

/******************************* MOTOR CURRENTS ******************************/

    /* These values specify the maximum current, but actual
     * currents may be lower when used with COOLCONF
     */

    MARLIN["X_CURRENT"]                              = 975 // mA
    MARLIN["Y_CURRENT"]                              = 975 // mA
    MARLIN["Z_CURRENT"]                              = 600 // mA

/**************** ACCELERATION, FEEDRATES AND XYZ MOTOR STEPS ****************/

    MARLIN["DEFAULT_AXIS_STEPS_PER_UNIT"]                = [80, 80, 400, 400]
    /*
     * EW - 1600 for IGUS Z changed from default of 4000
     * Z-axis leadscrew https://www.amazon.com/Witbot-Pillow-Bearing-Coupler-Printer/dp/B074Z4Q23M/ref=sr_1_4?ie=UTF8&qid=1549046242&sr=8-4&keywords=lead%20screw
     */

    MARLIN["DEFAULT_MAX_FEEDRATE"]                       = [50, 50, 5, 20] // mm/s
    MARLIN["MANUAL_FEEDRATE"]                            = [50*60, 50*60, 5*60, 20*60]             // mm/min
    MARLIN["XY_PROBE_FEEDRATE"]                          = (50*60)                              // mm/min

    // A 32-bit board can handle more segments
    MARLIN["MIN_STEPS_PER_SEGMENT"]                      = 1

/********************************* LCD OPTIONS *********************************/

    // Slow down SPI speed when using unshielded ribbon cables.
    MARLIN["SD_SPI_SPEED"]                               = 'SPI_SIXTEENTH_SPEED'

    MARLIN["LCD_TIMEOUT_TO_STATUS"]                      = 600000 // Ten Minutes
    MARLIN["TOUCH_UI_FTDI_EVE"]                          = true
    MARLIN["TOUCH_UI_COCOA_THEME"]                       = true
    MARLIN["TOUCH_UI_COCOA_PRESS"]                       = true
    //MARLIN["TOUCH_UI_SYNDAVER_LEVEL"]                    = true
    MARLIN["LCD_LULZBOT_CLCD_UI"]                        = true
    MARLIN["TOUCH_UI_800x480"]                           = true
    MARLIN["AO_EXP1_PINMAP"]                             = true
    MARLIN["TOUCH_UI_USE_UTF8"]                          = true
    MARLIN["TOUCH_UI_UTF8_COPYRIGHT"]                    = true
    MARLIN["TOUCH_UI_UTF8_SUPERSCRIPTS"]                 = true
    MARLIN["SCROLL_LONG_FILENAMES"]                      = true
    MARLIN["TOUCH_UI_DEBUG"]                             = false
    MARLIN["TOUCH_UI_DEVELOPER_MENU"]                    = false

    MARLIN["TOUCH_UI_FTDI_EVE"]                          = true
    MARLIN["TOUCH_UI_USE_UTF8"]                          = true
    MARLIN["TOUCH_UI_UTF8_COPYRIGHT"]                    = true
    MARLIN["TOUCH_UI_UTF8_SUPERSCRIPTS"]                 = true
    MARLIN["SET_PROGRESS_MANUALLY"]                      = true
    MARLIN["SCROLL_LONG_FILENAMES"]                      = true
    MARLIN["PAUSE_REHEAT_FAST_RESUME"]                   = true
    MARLIN["NO_TIME_AFTER_SD_PRINT"]                     = true
    MARLIN["LCD_TIMEOUT_TO_STATUS"]                      = 0

    // Virtual joystick functionality
    MARLIN["JOYSTICK"]                                   = true
    MARLIN["JOY_X_PIN"]                                  = -1
    MARLIN["JOY_Y_PIN"]                                  = -1
    MARLIN["JOY_Z_PIN"]                                  = -1
    MARLIN["JOY_EN_PIN"]                                 = -1
    MARLIN["JOY_X_LIMITS"]                               = false
    MARLIN["JOY_Y_LIMITS"]                               = false
    MARLIN["JOY_Z_LIMITS"]                               = false

    if(!MARLIN["AO_EXP1_PINMAP"]) {
      MARLIN["ARCHIM2_SPI_FLASH_EEPROM_BACKUP_SIZE"]     = 1000
    }

    MARLIN["SHOW_CUSTOM_BOOTSCREEN"]                     = true
    MARLIN["BABYSTEPPING"]                               = false
    MARLIN["BABYSTEP_XY"]                                = false

    if(USE_AUTOLEVELING) {
      MARLIN["BABYSTEP_ZPROBE_OFFSET"]                   = false
      MARLIN["BABYSTEP_HOTEND_Z_OFFSET"]                 = false
    }

/*********************************** CLEAN UP **********************************/

    return MARLIN
}
/***************************** END OF CONFIGURATION ****************************/

function do_substitions(config, counts, line) {
    /* Perform substitutions on a #define line from the config using the following regex:
     *
     *   ^(\s*)                 - Match indentation
     *   ((?:\/\/)?#define\s*)  - Match #define or //#define followed by whitespace
     *   (?<variable>\w+)       - Match variable name
     *   ((?:\(\))?\s*)         - Match optional () followed by whitespace
     *   ((?:(?!\s*\/\/).)*)    - Match remainder up to but not including // and preceeding whitespace
     *   (\s*\/\/.*)?           - Match the comment and preceeding whitespace
     */
    const m = line.match(/^(\s*)((?:\/\/)?#define\s*)(?<variable>\w+)((?:\(\))?\s*)((?:(?!\s*\/\/).)*)(\s*\/\/.*)?/);
    if (m && config.hasOwnProperty(m.groups.variable)) {
        let [match, indent, define, variable, separator, value, comment] = m;
        counts[variable] += 1;
        const new_val = config[variable];
        if (typeof new_val == 'boolean' && new_val == false) {
            if(!define.startsWith("//")) {
                define = "//" + define;
            }
        } else {
            define = define.replace("//","");
        }
        if (typeof new_val != 'boolean') {
            value = format_val(new_val);
        }
        const new_line = indent + define + variable + (separator || (value ? " " : "")) + (value || "");
        if (new_line + (comment || "") != line) {
          return new_line + (comment ? comment.replace("//", "// <-- changed:") : " // <-- changed")
        }
    }
    return line
}

async function dump_variables(config, out_filename) {
    // Dump all the variables in the config in sorted order
    const out = [];
    for(const key of Object.keys(config).sort()) {
        const val = config[key]
        if(typeof val == 'boolean') {
            out.push((val ? "" : "//") + "#define " + key)
        } else {
            out.push("#define " + key + " " + format_val(val))
        }
    }
    await fs.writeFile(out_filename, out.join("\n"));
}

async function process_config(config, counts, in_filename, out_filename) {
    // Perform substitutions on an entire file
    if (out_filename == in_filename) {
        // Handle special case of in-place substitutions
        fs.renameSync(in_filename, in_filename + ".saved")
        in_filename = in_filename + ".saved"
    }
    const out = (await fs.readFile(in_filename, 'utf-8'))
                .split(/\r?\n/)
                .map(line => do_substitions(config, counts, line))
                .join("\n");
    await fs.writeFile(out_filename, out);
}

async function process_dir(directory, input) {
    //if (!await fs.access(directory)) {
        await fs.mkdir(directory, {recursive: true})
    //}
    counts = init_counters(config)
    for (const i of input) {
        await process_config(config, counts, i, directory + "/" + path.basename(i))
    }
    dump_counters(config, counts)

    if (args.summary) {
        dump_variables(config, directory + "/Configuration_summary.txt")
    }
}

function init_counters(config) {
    counts = {}
    for(const k of Object.keys(config)) {
        counts[k] = 0
    }
    return counts
}

function dump_counters(config, counts) {
    for(const k of Object.keys(config)) {
        if (counts[k] == 0) {
            console.error("Warning:", k, "not found in any of the configuration files.")
        }
    }
}

function invalid_printer(str) {
    console.error(str + "\n\nPrinter must be one of:\n\n   " + PRINTER_CHOICES.join("\n   ") + "\n")
    process.exit()
}

function invalid_toolhead(str) {
    console.error(str + "\n\nToolhead must be one of:\n\n   " + TOOLHEAD_CHOICES.join("\n   ") + "\n")
    process.exit()
}

function match_selection(str, list) {
    withoutOptions = (str.replace("BLTouch","")
                         .replace("HallEffect","")
                         .replace("LCD","")
                         .replace("TouchSD","")
                         .replace("TouchUSB","")
                         .replace("Einsy","")
                         .replace("Archim","")
                         .replace("BTT002",""))
    // Try an exact match
    if (list.includes(withoutOptions)) {
        return str;
    }
    // Do a fuzzy match
    let re = new RegExp(str, 'i');
    let matches = list.filter(x => x.search(re) != -1);
    if (matches.length > 1) {
        // Try narrowing down the choices
        re = new RegExp("(\\b|_)" + str + "(\\b|_)", 'i');
        const matches2 = list.filter(x => x.search(re) != -1);
        if (matches2.length > 0) {
            matches = matches2
        }
    }
    if (matches.length > 1) {
        console.log("Selection is ambiguous, must be one of:\n\n   " + matches.join("\n   ") + "\n")
        process.exit()
    }
    if (matches.length == 0) {
        console.log("Invalid selection, must be one of:\n\n   " + list.join("\n   ") + "\n")
        process.exit()
    }
    return matches[0]
}

function usage() {
    console.log("usage: node build-config.js [-h] [-D DIRECTORY] [-i INPUT] [-s] [printer] [toolhead]\n");
    console.log("This script modifies Marlin's \"Configuration.h\" and \"Configuration_adv.h\" for LulzBot printers.");
    console.log("   \n\n    Printer  Choices:\n      " + PRINTER_CHOICES.join("\n      "));
    console.log("   \n\n    Toolhead Choices:\n      " + TOOLHEAD_CHOICES.join("\n      "));
}

// Parse the command line arguments

const argv = process.argv.slice();
const args = {input:[]};
const node = argv.shift();
const js   = argv.shift();

if(argv.length == 0) {
    usage();
    process.exit()
}

while(argv.length) {
    const arg = argv.shift();
    switch(arg) {
        case '-D':
        case '--directory':
            args.directory = argv.shift();
            break;
        case '-i':
        case '--input':
            args.input.push(argv.shift());
            break;
        case '-s':
        case '--summary':
            args.summary = true;
            break;
        case '-h':
            usage();
            process.exit()
            break;
        default:
            if(!args.printer) args.printer = arg;
            else if(!args.toolhead) args.toolhead = arg;
    }
}

// Do the main processing

if (!args.printer) {
    invalid_printer("Must specify a printer type.")
}

if (!args.toolhead) {
    invalid_toolhead("Must specify a toolhead type.")
}

args.printer  = match_selection(args.printer,  PRINTER_CHOICES)
args.toolhead = match_selection(args.toolhead, TOOLHEAD_CHOICES)

if (args.input.length == 0) {
    args.input = [
        "../default/Configuration.h",
        "../default/Configuration_adv.h"
    ]
}

if (!args.directory) {
    args.directory = "."
}

config = make_config(args.printer, args.toolhead)

if (args.directory) {
    process_dir(args.directory, args.input)
}

