#include "patro_joystick.h"
#include "buttons.h"
#include "analog.h"
#include "led.h"

// --- Main Joystick Logic ---

void send_hid_report(void)
{
    if (!tud_hid_ready())
    {
        return;
    }

    hid_report_t report = {0};
    bool buttonPressed = false;

    // Lê os botões
    if (isButtonPressed(BTA))
    {
        report.buttons |= (1 << 0);
        buttonPressed = true;
    }
    if (isButtonPressed(BTB))
    {
        report.buttons |= (1 << 1);
        buttonPressed = true;
    }

    // Feedback LED
    if (buttonPressed)
    {
        setLedBrightness(LED_RED_PIN, 127);
    }
    else
    {
        setLedBrightness(LED_RED_PIN, 0);
    }

    // Lê os eixos
    report.x = readAnalogX();
    report.y = readAnalogY();

    tud_hid_report(0, &report, sizeof(report));
}