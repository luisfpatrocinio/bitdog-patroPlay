#ifndef PATRO_JOYSTICK_H_
#define PATRO_JOYSTICK_H_

#include "tusb.h"

// A struct que define o formato do nosso relatório HID
typedef struct TU_ATTR_PACKED
{
    uint8_t buttons;
    int8_t x;
    int8_t y;
} hid_report_t;

// Protótipo da função principal do nosso joystick, que será chamada no loop do main.c
void send_hid_report(void);

#endif /* PATRO_JOYSTICK_H_ */