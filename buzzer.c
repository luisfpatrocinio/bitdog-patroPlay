/**
 * @file buzzer.c
 * @brief Buzzer PWM implementation for RP2040
 *
 * @details Hardware PWM implementation for buzzer control using RP2040's PWM peripheral.
 * Provides tone generation with frequency and duration control.
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

#include "buzzer.h"

/**
 * @brief Initializes PWM hardware for buzzer control
 *
 * Configures the buzzer GPIO pin for PWM operation with default settings:
 * - GPIO function set to PWM
 * - Clock divider initialized to 4.0
 * - PWM initially disabled
 *
 * @post PWM hardware ready for buzzer operation
 * @note Uses BUZZER_PIN from buzzer.h as default pin
 *
 * @see BUZZER_PIN
 */
void initBuzzerPWM()
{
  gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
  uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);
  pwm_config config = pwm_get_default_config();
  pwm_config_set_clkdiv(&config, 4.0f); // Ajusta divisor de clock
  pwm_init(slice_num, &config, true);
  pwm_set_gpio_level(BUZZER_PIN, 0); // Desliga o PWM inicialmente
}

/**
 * @brief Generates a simple digital beep
 *
 * Creates a basic beep by toggling the buzzer pin directly (not using PWM).
 * Produces a square wave at the GPIO's maximum frequency.
 *
 * @param duration Beep length in milliseconds
 * @warning Blocks during entire beep duration
 * @note For frequency-controlled tones, use playTone() instead
 *
 * @see playTone()
 */
void beep(int duration)
{
  gpio_put(BUZZER_PIN, 1);
  sleep_ms(duration);
  gpio_put(BUZZER_PIN, 0);
}

/**
 * @brief Plays a tone with specific frequency and duration
 *
 * Generates a PWM signal at the requested frequency with 50% duty cycle.
 * Implements frequency calculation based on RP2040's clock system.
 *
 * @param frequency Target frequency in Hertz (50-5000Hz recommended)
 * @param duration_ms Tone duration in milliseconds
 *
 * @note Actual frequency may vary slightly due to integer rounding
 * @warning Frequency accuracy decreases at higher frequencies
 * @warning Blocks execution during tone playback
 *
 * @implnote Calculation:
 * @code
 * top = (CPU_CLOCK / (frequency * divider)) - 1
 * @endcode
 */
void playTone(uint frequency, uint duration_ms)
{
  uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);

  // Calculate the "top" (maximum PWM counter value)
  float div = 16.0f; // Use divider 1.0 for higher resolution
  uint32_t top = (uint32_t)(CLOCK_PWM / (frequency * div)) - 1;

  pwm_set_clkdiv(slice_num, div);          // Configure divider correctly
  pwm_set_wrap(slice_num, top);            // Set top value
  pwm_set_gpio_level(BUZZER_PIN, top / 2); // 50% duty cycle

  sleep_ms(duration_ms);

  pwm_set_gpio_level(BUZZER_PIN, 0); // Stop the sound
}

/**
 * @brief Plays a welcome sequence of musical notes on the buzzer.
 *
 * This function plays a predefined sequence of five notes (C4, D4, E4, F4, G4)
 * as a welcome melody. Each note is played for 50 milliseconds, with a short
 * pause of 30 milliseconds between notes.
 *
 * @note This function blocks execution while the melody is playing.
 * @see playTone()
 */
void playWelcomeTones()
{
  int welcome_notes[5] = {262, 294, 330, 349, 392}; // C4, D4, E4, F4, G4
  for (int i = 0; i < 5; i++)
  {
    playTone(welcome_notes[i], 50); // 50ms per note
    sleep_ms(30);                   // Short pause between notes
  }
}