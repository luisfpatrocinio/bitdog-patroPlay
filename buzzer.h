/**
 * @file buzzer.h
 * @author Luis Felipe Patrocinio (https://github.com/luisfpatrocinio/)
 * @brief Buzzer control library for RP2040 (BitDogLab)
 *
 * Provides functions to control a buzzer using PWM on the RP2040 microcontroller.
 * Supports initialization, simple beeps, and tone playback with specified frequency.
 *
 * @version 0.1
 * @date 2025-05-14
 *
 * @copyright Copyright (c) 2025 Luis Felipe Patrocinio
 * @license This project is released under the MIT License.
 * See LICENSE file for full license text.
 * https://github.com/luisfpatrocinio/bitdog-patroLibs/blob/main/LICENSE
 *
 */
#ifndef BUZZER_H
#define BUZZER_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"

/**
 * @def BUZZER_PIN
 * @brief Default GPIO pin number connected to the buzzer
 *
 * @note Default is pin 21, which is the buzzer pin on the BitDogLab board. Can be redefined before inclusion if a different pin is needed.
 */
#define BUZZER_PIN 21

/**
 * @def CLOCK_PWM
 * @brief Base clock frequency of RP2040 in Hz (as float)
 */
#define CLOCK_PWM 125000000.0f

/**
 * @def BUZZER_PWM_DIVIDER
 * @brief PWM clock divider value for buzzer operation
 *
 * @note Higher values will lower the PWM frequency
 */
#define BUZZER_PWM_DIVIDER 16.0f

/**
 * @brief Initializes the PWM for buzzer control
 *
 * Configures the specified GPIO pin (BUZZER_PIN) for PWM operation with default settings.
 * Must be called before using other buzzer functions.
 *
 * @post PWM is configured and ready for buzzer operation
 * @note Uses default BUZZER_PIN and BUZZER_PWM_DIVIDER values
 */
void initBuzzerPWM();

/**
 * @brief Produces a simple beep sound
 *
 * Generates a beep with default frequency (approximately 2kHz) for the specified duration.
 *
 * @param duration Beep duration in milliseconds
 *
 * @note Uses a default frequency that produces an audible beep
 * @warning Blocks execution during beep duration
 * @see playTone() for frequency-controlled beeps
 */
void beep(int duration);

/**
 * @brief Plays a tone with specified frequency and duration
 *
 * Generates a PWM signal at the requested frequency for the specified duration.
 * Frequency accuracy depends on clock divider settings.
 *
 * @param frequency Frequency of the tone in Hertz (Hz) (recommended range 100-5000Hz)
 * @param duration_ms Duration of the tone in milliseconds
 *
 * @note Frequency range is limited by PWM capabilities
 * @warning Blocks execution during tone playback
 * @see initBuzzerPWM() must be called first
 *
 * @code
 * playTone(440, 500); // Plays A4 note (440Hz) for 500ms
 * @endcode
 */
void playTone(uint frequency, uint duration_ms);

/**
 * @brief Plays a welcome sequence of musical notes on the buzzer.
 *
 * @note This function blocks execution while the melody is playing.
 * @see playTone()
 */
void playWelcomeTones();

#endif // BUZZER_H