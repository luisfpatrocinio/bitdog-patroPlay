#include <stdio.h>
#include "pico/stdlib.h"
#include "tusb.h" // Importa a biblioteca TinyUSB

// Inclui a sua biblioteca de botões
#include "buttons.h"
#include "analog.h"

//--------------------------------------------------------------------+
// Descritores USB
//--------------------------------------------------------------------+

// Descritor de Dispositivo
tusb_desc_device_t const desc_device = {
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,        // USB 2.0
    .bDeviceClass = 0x00,    // Classe definida por interface
    .bDeviceSubClass = 0x00, // Subclasse definida por interface
    .bDeviceProtocol = 0x00, // Protocolo definido por interface
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,

    .idVendor = 0xCAFE,  // ID de Vendedor (Pode ser qualquer um para teste)
    .idProduct = 0x0001, // ID de Produto (Pode ser qualquer um para teste)
    .bcdDevice = 0x0100, // Versão 1.0.0 do dispositivo

    .iManufacturer = 0x01, // Índice da string "Fabricante"
    .iProduct = 0x02,      // Índice da string "Produto"
    .iSerialNumber = 0x03, // Índice da string "Número de Série"

    .bNumConfigurations = 0x01 // Número de configurações
};

// --- Descritor de Relatório HID (Joystick com Analógico) ---
uint8_t const hid_report_descriptor[] = {
    0x05, 0x01, // USAGE_PAGE (Generic Desktop)
    0x09, 0x04, // USAGE (Joystick)
    0xA1, 0x01, // COLLECTION (Application)

    // --- Definição dos Botões (1 byte) ---
    0x05, 0x09, // USAGE_PAGE (Button)
    0x19, 0x01, // USAGE_MINIMUM (Button 1)
    0x29, 0x02, // USAGE_MAXIMUM (Button 2)
    0x15, 0x00, // LOGICAL_MINIMUM (0)
    0x25, 0x01, // LOGICAL_MAXIMUM (1)
    0x75, 0x01, // REPORT_SIZE (1 bit por botão)
    0x95, 0x02, // REPORT_COUNT (2 botões)
    0x81, 0x02, // INPUT (Data,Var,Abs) - Os 2 primeiros bits do byte
    // --- Preenchimento para completar o byte de botões ---
    0x75, 0x06, // REPORT_SIZE (6 bits)
    0x95, 0x01, // REPORT_COUNT (1)
    0x81, 0x03, // INPUT (Cnst,Var,Abs) - Os 6 bits restantes

    // --- Definição dos Eixos Analógicos (X e Y) ---
    0x05, 0x01, // USAGE_PAGE (Generic Desktop)
    0x09, 0x30, // USAGE (X)
    0x09, 0x31, // USAGE (Y)
    0x15, 0x81, // LOGICAL_MINIMUM (-127)
    0x25, 0x7F, // LOGICAL_MAXIMUM (127)
    0x75, 0x08, // REPORT_SIZE (8 bits por eixo)
    0x95, 0x02, // REPORT_COUNT (2 eixos)
    0x81, 0x02, // INPUT (Data,Var,Abs)

    0xC0 // END_COLLECTION
};

// Estrutura do nosso relatório HID: 1 byte de botões, 2 bytes para os eixos
typedef struct TU_ATTR_PACKED
{
    uint8_t buttons;
    int8_t x;
    int8_t y;
} hid_report_t;

// Descritor de Configuração USB
// Contém a descrição da nossa interface HID
#define CONFIG_TOTAL_LEN (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN)
#define EPNUM_HID 0x81
uint8_t const desc_configuration[] = {
    // Cabeçalho da Configuração
    TUD_CONFIG_DESCRIPTOR(1, 1, 0, CONFIG_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),
    // Interface HID
    TUD_HID_DESCRIPTOR(0, 0, HID_ITF_PROTOCOL_NONE, sizeof(hid_report_descriptor), EPNUM_HID, CFG_TUD_HID_EP_BUFSIZE, 5)};

// Descritores de String
char const *string_desc_arr[] = {
    (char[]){0x09, 0x04}, // 0: Código de idioma (Inglês US)
    "BitDogLab",          // 1: Fabricante
    "PatroPlay Joystick", // 2: Produto
    "123456",             // 3: Número de Série
};

static uint16_t _desc_str[32];

// --- Implementação dos Callbacks de Descritor ---

// Chamado quando o host pede o descritor de dispositivo
uint8_t const *tud_descriptor_device_cb(void)
{
    return (uint8_t const *)&desc_device;
}

// Chamado quando o host pede o descritor de relatório HID
uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance)
{
    return hid_report_descriptor;
}

// Chamado quando o host pede o descritor de configuração
uint8_t const *tud_descriptor_configuration_cb(uint8_t index)
{
    (void)index; // Ignora o índice, pois só temos uma configuração
    return desc_configuration;
}

// Chamado quando o host pede um descritor de string
uint16_t const *tud_descriptor_string_cb(uint8_t index, uint16_t langid)
{
    (void)langid;
    uint8_t chr_count;

    if (index == 0)
    {
        memcpy(&_desc_str[1], string_desc_arr[0], 2);
        chr_count = 1;
    }
    else
    {
        // Converte a string ASCII para UTF-16
        if (!(index < sizeof(string_desc_arr) / sizeof(string_desc_arr[0])))
            return NULL;

        const char *str = string_desc_arr[index];
        chr_count = strlen(str);
        if (chr_count > 31)
            chr_count = 31;

        for (uint8_t i = 0; i < chr_count; i++)
        {
            _desc_str[1 + i] = str[i];
        }
    }

    // O primeiro byte do descritor de string contém seu tamanho e tipo
    _desc_str[0] = (TUSB_DESC_STRING << 8) | (2 * chr_count + 2);

    return _desc_str;
}

// Callback: Chamado quando o PC envia dados (não usaremos isso).
void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize)
{
    // Intencionalmente vazio
}

// Callback: Chamado quando o PC pede dados (não usaremos isso de forma síncrona).
uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen)
{
    // Intencionalmente vazio
    return 0;
}

// --- Lógica do Joystick ---

// Função para enviar o estado dos botões e do analógico para o PC
void send_hid_report()
{
    if (!tud_hid_ready())
    {
        return;
    }

    // Cria uma instância do nosso relatório
    hid_report_t report = {0};

    // 1. Lê os botões
    if (isButtonPressed(BTA))
    {
        report.buttons |= (1 << 0);
    }
    if (isButtonPressed(BTB))
    {
        report.buttons |= (1 << 1);
    }

    // 2. Lê os eixos X e Y usando sua biblioteca
    // A sua função já deve retornar um valor no intervalo correto.
    // Se não, precisaríamos mapear o valor lido do ADC (0-4095) para -127 a 127.
    // A sua PatroLibs já faz isso com mapValue e applyThreshold!
    report.x = readAnalogX();
    report.y = readAnalogY();

    // Envia o relatório (agora com 3 bytes) para o PC
    tud_hid_report(0, &report, sizeof(report));
}

// --- Função Principal ---
int main()
{
    stdio_init_all();
    initButtons();
    initAnalog(); 
    tusb_init();

    // Loop infinito
    while (1)
    {
        tud_task();
        send_hid_report();
    }

    return 0;
}