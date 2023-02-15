/******************************
 * cocoa_press_extras.cpp *
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

#include "cocoa_press_extras.h"

/**************************** EXTRA HEATER CHECK ****************************/

#if ENABLED(COCOA_PRESS_EXTRA_HEATER)
    bool extra_heater_installed;

    void check_extra_heater() {
      extra_heater_installed = analogRead(TEMP_2_PIN) < 900;
    }

    bool has_extra_heater() {
      return extra_heater_installed;
    }
#endif

/************************** CHOCOLATE LEVEL SENSOR **************************/

#if ENABLED(COCOA_PRESS_CHOCOLATE_LEVEL_SENSOR)
  float get_chocolate_fill_level() {
    // Note, this requires a new pin 108 to be added to to access ADC9
    // "ArduinoAddons/arduino-1.8.5/packages/ultimachine/hardware/sam/1.6.9-b/variants/archim/variant.cpp"

    #if NUM_SERVOS < 2
      constexpr uint8_t CHOC_LEVEL_ADC_PIN = 108;
      constexpr uint8_t CHOC_LEVEL_SAMPLES = 8;
      const int sample = analogRead(CHOC_LEVEL_ADC_PIN);
      static int samples[CHOC_LEVEL_SAMPLES];

      // Compute rolling average as a low-pass filter.
      long sum = 0;
      for(uint8_t i = 0; i < CHOC_LEVEL_SAMPLES - 1; i++) {
        samples[i] = samples[i+1];
        sum += samples[i];
      }
      samples[CHOC_LEVEL_SAMPLES-1] = sample;
      sum += sample;

      return float(sum) / 1024 / CHOC_LEVEL_SAMPLES;
    #else
      return 1.0f;
    #endif
  }
#endif

/************************** CHAMBER HEATER MODULATION **************************/

#if ENABLED(COCOA_PRESS_CYCLE_COOLER)
  static uint32_t cycle_start_time;
  static bool cooler_cycling = false;

  constexpr uint32_t on_cycle_end_ms  = (COOLER_ON_CYCLE_TIME                        ) * 1000;
  constexpr uint32_t off_cycle_end_ms = (COOLER_ON_CYCLE_TIME + COOLER_OFF_CYCLE_TIME) * 1000;

  void cycle_cooler_init() {
    SET_OUTPUT(HEATER_CHAMBER_CYCLE_PIN);
    cycle_start_time = millis() - off_cycle_end_ms;
  }

  bool cycle_cooler_enabled() {
    return cooler_cycling;
  }

  void cycle_cooler_state(bool state) {
    cooler_cycling = state;
    if(!cooler_cycling) WRITE(HEATER_CHAMBER_CYCLE_PIN, false);
  }

  void cycle_cooler_idle() {
    const uint32_t cycle_elapsed_ms = millis() - cycle_start_time;
    const bool on_cycle_done  = cycle_elapsed_ms > on_cycle_end_ms;
    const bool off_cycle_done = cycle_elapsed_ms > off_cycle_end_ms;

    bool cycle_cooler_state = READ(HEATER_CHAMBER_PIN);
    if(cooler_cycling && cycle_cooler_state) {
      if(on_cycle_done)  cycle_cooler_state = false;
      if(off_cycle_done) cycle_start_time = millis();
    }
    WRITE(HEATER_CHAMBER_CYCLE_PIN, cycle_cooler_state);
  }
#endif