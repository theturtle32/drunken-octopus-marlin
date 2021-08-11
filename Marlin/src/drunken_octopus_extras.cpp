/******************************
 * drunken_octopus_extras.cpp *
 ******************************/

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

#include "MarlinCore.h"

#include "drunken_octopus_extras.h"

/******************************** EMI MITIGATION *******************************/

#if ENABLED(EMI_MITIGATION)
  void emi_init(void) {
      enable_emi_pins(false);

      #define EMI_SHUTOFF(pin) SET_OUTPUT(pin); WRITE(pin, LOW);

      #if MB(ARCHIM2)
        //EMI_SHUTOFF(GPIO_PB1_J20_5)
        EMI_SHUTOFF(GPIO_PB0_J20_6)
        EMI_SHUTOFF(GPIO_PB3_J20_7)
        EMI_SHUTOFF(GPIO_PB2_J20_8)
        EMI_SHUTOFF(GPIO_PB6_J20_9)
        EMI_SHUTOFF(GPIO_PB5_J20_10)
        EMI_SHUTOFF(GPIO_PB8_J20_11)
        EMI_SHUTOFF(GPIO_PB4_J20_12)
        EMI_SHUTOFF(GPIO_PB9_J20_13)
        EMI_SHUTOFF(GPIO_PB7_J20_14)
        //EMI_SHUTOFF(GPIO_PB15_J20_15)
        //EMI_SHUTOFF(GPIO_PB15_J20_16)
        EMI_SHUTOFF(GPIO_PB14_J20_17)
        EMI_SHUTOFF(GPIO_PA18_J20_21)
        EMI_SHUTOFF(GPIO_PA17_J20_22)
      #endif
  }

  #define SET_PIN_STATE(pin, enable) \
    if(enable) { \
      /* Set as inputs with pull-up resistor */ \
      SET_INPUT(pin); \
      WRITE(pin, HIGH); \
    } else { \
      SET_OUTPUT(pin); \
      WRITE(pin, LOW); \
    }

  /* Enable the probe pins only only when homing/probing,
   * as this helps reduce EMI by grounding the lines.
   */
  void enable_emi_pins(const bool enable) {
    #if DISABLED(ENDSTOPS_ALWAYS_ON_DEFAULT) && ENABLED(NOZZLE_AS_PROBE)
      #if PIN_EXISTS(Z_MIN_PIN)
        SET_PIN_STATE(Z_MIN_PIN, enable);
      #endif
    #endif
    #if PIN_EXISTS(Z_MIN_PROBE) && ENABLED(NOZZLE_AS_PROBE)
      SET_PIN_STATE(Z_MIN_PROBE_PIN, enable)
    #endif

    /* Wait to charge up the lines */
    if(enable) delay(5);
  }
#endif

/************************** INDEPENDENT Z AUTO-DETECT **************************/

#if ENABLED(Z2_PRESENCE_CHECK)
    #if NUM_Z_STEPPER_DRIVERS != 2
      #error Z2_PRESENCE_CHECK requires two Z stepper drivers
    #endif
    #ifndef AXIS_LEVELING_COMMANDS
      #error Z2_PRESENCE_CHECK requires AXIS_LEVELING_COMMANDS
    #endif

    // Allow the presence of a second Z motor to be controlled
    // using a jumper on the GPIO pin of the Archim.
    bool has_z2_jumper() {
        SET_OUTPUT(GPIO_PB0_J20_6);
        WRITE(GPIO_PB0_J20_6, LOW);
        SET_INPUT(GPIO_PB2_J20_8);
        WRITE(GPIO_PB2_J20_8, HIGH);
        return !READ(GPIO_PB2_J20_8);
    }
#endif

/*************************** ELECTROMAGNETIC Z BRAKE ***************************/

#if ELECTROMAGNETIC_BRAKE_PIN
    #include "module/stepper/indirection.h"

    void em_brake_init() {
        ENABLE_AXIS_Z();
        safe_delay(500);
        SET_OUTPUT(ELECTROMAGNETIC_BRAKE_PIN);
        analogWrite(pin_t(ELECTROMAGNETIC_BRAKE_PIN), 255 * 1.0f);
        safe_delay(500);
        analogWrite(pin_t(ELECTROMAGNETIC_BRAKE_PIN), 255 * 0.75f);
    }
#endif

/*************************** PRINT TIMER AUTOSTART *****************************/

#if ENABLED(START_PRINT_TIMER_ON_G26)
    #include "module/printcounter.h"

    AutoPrintTimer::AutoPrintTimer() {
        was_running = print_job_timer.isRunning();
        was_paused = print_job_timer.isPaused();
        print_job_timer.start();
    }
    
    AutoPrintTimer::~AutoPrintTimer() {
        if(was_paused)
            print_job_timer.pause();
        else if(!was_running) print_job_timer.stop();
    }
#endif