#include "patro_joystick.h"
#include "buttons.h"
#include "analog.h"

// --- Main Joystick Logic ---

void send_hid_report(void)
{
    if (!tud_hid_ready())
    {
        return;
    }

    hid_report_t report = {0};

    // Lê os botões
    if (isButtonPressed(BTA))
        report.buttons |= (1 << 0);
    if (isButtonPressed(BTB))
        report.buttons |= (1 << 1);

    // Lê os eixos
    report.x = readAnalogX();
    report.y = readAnalogY();

    tud_hid_report(0, &report, sizeof(report));
}