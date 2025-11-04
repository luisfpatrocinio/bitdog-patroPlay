#ifndef LED_H
#define LED_H

#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include <math.h>

#define LED_RED_PIN 13
#define LED_BLUE_PIN 12
#define LED_GREEN_PIN 11

// PWM Definitions
#define PWM_PERIOD 255
#define PWM_DIVIDER 16.0

void initLeds();
void setLedColor(int red, int green, int blue);
void turnOffLeds();
void manageLed(int led, int state);
void setLedBrightness(int led, int brightness);
void setAllLedsBrightness(int brightness);
void pulseLed(int led, float speed);

#endif