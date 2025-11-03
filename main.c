// Arquivo: main.c

#include "pico/stdlib.h"
#include "tusb.h"

// Inclui as bibliotecas de hardware
#include "buttons.h"
#include "analog.h"

// Inclui nosso módulo de joystick
#include "patro_joystick.h"

// --- Função Principal ---
int main() {
    // Inicialização do hardware
    stdio_init_all();
    initButtons();
    initAnalog();

    // Inicialização do USB
    tusb_init();

    // Loop infinito
    while (1) {
        // Tarefa principal do TinyUSB (ESSENCIAL)
        tud_task();

        // Tarefa principal do nosso joystick
        send_hid_report();
    }

    return 0;
}