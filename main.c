#include "pico/stdlib.h"
#include "tusb.h"

// Includes hardware libraries
#include "buttons.h"
#include "analog.h"
#include "led.h"

// Includes our joystick module
#include "patro_joystick.h"

// --- Main Function ---
int main()
{
    // Hardware initialization
    stdio_init_all();
    initButtons();
    initAnalog();
    initLeds();

    // USB initialization
    tusb_init();

    // Infinite loop
    while (1)
    {
        // Main TinyUSB task
        tud_task();

        // Main task of our joystick
        send_hid_report();
    }

    return 0;
}