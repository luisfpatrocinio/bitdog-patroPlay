// Arquivo: usb_descriptors.c

#include "usb_descriptors.h"
#include <string.h>

// --- Descritor de Relatório HID (específico para este joystick) ---
// MOVEMOS ESTE BLOCO PARA CÁ
uint8_t const hid_report_descriptor[] = {
    0x05, 0x01, 0x09, 0x04, 0xA1, 0x01, // Generic Desktop, Joystick, Application
    // Botões
    0x05, 0x09, 0x19, 0x01, 0x29, 0x02, 0x15, 0x00, 0x25, 0x01, 0x75, 0x01, 0x95, 0x02, 0x81, 0x02,
    // Preenchimento
    0x75, 0x06, 0x95, 0x01, 0x81, 0x03,
    // Eixos
    0x05, 0x01, 0x09, 0x30, 0x09, 0x31, 0x15, 0x81, 0x25, 0x7F, 0x75, 0x08, 0x95, 0x02, 0x81, 0x02,
    // Fim
    0xC0
};

// --- Descritores Genéricos USB ---
// (O resto do código que já estava aqui)
tusb_desc_device_t const desc_device = { /* ... conteúdo como antes ... */ };
uint8_t const desc_configuration[] = { /* ... conteúdo como antes ... */ };
char const *string_desc_arr[] = { /* ... conteúdo como antes ... */ };
static uint16_t _desc_str[32];


// --- Implementação de TODOS os Callbacks de Descritor ---

uint8_t const *tud_descriptor_device_cb(void) { /* ... código como antes ... */ }
uint8_t const *tud_descriptor_configuration_cb(uint8_t index) { /* ... código como antes ... */ }
uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid) { /* ... código como antes ... */ }

// MOVEMOS ESTES CALLBACKS PARA CÁ
uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance) {
    return hid_report_descriptor;
}
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen) {
    return 0; // Não usado
}
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize) {
    // Não usado
}