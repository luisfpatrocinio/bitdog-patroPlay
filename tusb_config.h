#ifndef TUSB_CONFIG_H_
#define TUSB_CONFIG_H_

// --- Configurações Essenciais ---
// Define o modo de operação da porta USB 0 como "Dispositivo" (Device)
#define CFG_TUSB_RHPORT0_MODE OPT_MODE_DEVICE

// --- Configuração do Driver de Classe HID (Human Interface Device) ---
// Habilita o driver HID. '1' significa que teremos 1 interface HID.
#define CFG_TUD_HID 1

// Define o tamanho do buffer para os relatórios HID.
// O nosso relatório tem apenas 1 byte, mas um buffer de 16 é um valor seguro e padrão.
#define CFG_TUD_HID_REPORT_BUFSIZE 16

#endif /* TUSB_CONFIG_H_ */