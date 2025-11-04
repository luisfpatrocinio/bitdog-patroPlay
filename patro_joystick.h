#ifndef PATRO_JOYSTICK_H_
#define PATRO_JOYSTICK_H_

#include "tusb.h"

// A struct that defines the format of our HID report
typedef struct TU_ATTR_PACKED
{
    uint8_t buttons;
    int8_t x;
    int8_t y;
} hid_report_t;

// Main joystick logic function
void send_hid_report(void);

#endif /* PATRO_JOYSTICK_H_ */