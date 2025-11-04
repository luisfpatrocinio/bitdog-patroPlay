#include "usb_descriptors.h"
#include <string.h>

// --- HID Report Descriptor (specific to this joystick) ---
// This is the "instruction manual" that tells the PC how to interpret the data from our joystick.
uint8_t const hid_report_descriptor[] = {
    0x05, 0x01, // USAGE_PAGE (Generic Desktop): Common PC device.
    0x09, 0x04, // USAGE (Joystick): Specifically, a joystick.
    0xA1, 0x01, // COLLECTION (Application): Start of the device definition.

    // --- Buttons Block (1 byte) ---
    0x05, 0x09, // USAGE_PAGE (Button): Buttons section.
    0x19, 0x01, // USAGE_MINIMUM (Button 1): Starting at button 1.
    0x29, 0x02, // USAGE_MAXIMUM (Button 2): Up to button 2.
    0x15, 0x00, // LOGICAL_MINIMUM (0): Value for "released".
    0x25, 0x01, // LOGICAL_MAXIMUM (1): Value for "pressed".
    0x75, 0x01, // REPORT_SIZE (1): Each button occupies 1 bit.
    0x95, 0x02, // REPORT_COUNT (2): We have 2 buttons.
    0x81, 0x02, // INPUT (Data,Var,Abs): Defines as input.
    // Padding to complete the byte (2 bits used + 6 padding bits = 8 bits)
    0x75, 0x06, // REPORT_SIZE (6): 6 bits.
    0x95, 0x01, // REPORT_COUNT (1): 1 padding field.
    0x81, 0x03, // INPUT (Constant): Defines as constant/ignored.

    // --- Axes Block (2 bytes) ---
    0x05, 0x01, // USAGE_PAGE (Generic Desktop): Return to general page.
    0x09, 0x30, // USAGE (X): Represents the X axis.
    0x09, 0x31, // USAGE (Y): Represents the Y axis.
    0x15, 0x81, // LOGICAL_MINIMUM (-127): Minimum axis value.
    0x25, 0x7F, // LOGICAL_MAXIMUM (127): Maximum axis value.
    0x75, 0x08, // REPORT_SIZE (8): Each axis occupies 8 bits (1 byte).
    0x95, 0x02, // REPORT_COUNT (2): We have 2 axes (X and Y).
    0x81, 0x02, // INPUT (Data,Var,Abs): Defines as input.

    0xC0 // END_COLLECTION: End of joystick definition.
};

// --- Generic USB Descriptors ---

tusb_desc_device_t const desc_device = {
    .bLength = sizeof(tusb_desc_device_t),
    .bDescriptorType = TUSB_DESC_DEVICE,
    .bcdUSB = 0x0200,
    .bDeviceClass = 0x00,
    .bDeviceSubClass = 0x00,
    .bDeviceProtocol = 0x00,
    .bMaxPacketSize0 = CFG_TUD_ENDPOINT0_SIZE,
    .idVendor = 0xCAFE,
    .idProduct = 0x0001,
    .bcdDevice = 0x0100,
    .iManufacturer = 0x01,
    .iProduct = 0x02,
    .iSerialNumber = 0x03,
    .bNumConfigurations = 0x01};

// ** ADDITION OF MISSING DEFINITIONS **
#define CONFIG_TOTAL_LEN (TUD_CONFIG_DESC_LEN + TUD_HID_DESC_LEN)
#define EPNUM_HID 0x81

uint8_t const desc_configuration[] = {
    // Configuration header
    TUD_CONFIG_DESCRIPTOR(1, 1, 0, CONFIG_TOTAL_LEN, TUSB_DESC_CONFIG_ATT_REMOTE_WAKEUP, 100),
    // HID Interface
    TUD_HID_DESCRIPTOR(0, 0, HID_ITF_PROTOCOL_NONE, sizeof(hid_report_descriptor), EPNUM_HID, CFG_TUD_HID_EP_BUFSIZE, 5)};

char const *string_desc_arr[] = {
    (char[]){0x09, 0x04}, "BitDogLab", "PatroPlay Joystick", "123456"};

static uint16_t _desc_str[32];

// --- Implementation of ALL descriptor callbacks ---

uint8_t const *tud_descriptor_device_cb(void)
{
    return (uint8_t const *)&desc_device;
}

uint8_t const *tud_descriptor_configuration_cb(uint8_t index)
{
    (void)index;
    return desc_configuration;
}

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
    _desc_str[0] = (TUSB_DESC_STRING << 8) | (2 * chr_count + 2);
    return _desc_str;
}

uint8_t const *tud_hid_descriptor_report_cb(uint8_t instance)
{
    return hid_report_descriptor;
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t *buffer, uint16_t reqlen)
{
    return 0; // Not used
}

void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const *buffer, uint16_t bufsize)
{
    // Not used
}