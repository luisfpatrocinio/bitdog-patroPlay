#ifndef TUSB_CONFIG_H_
#define TUSB_CONFIG_H_

// --- Essential Configurations ---
// Define the operating mode of USB port 0 as "Device"
#define CFG_TUSB_RHPORT0_MODE OPT_MODE_DEVICE

// --- HID (Human Interface Device) Class Driver Configuration ---
// Enable the HID driver. '1' means there will be 1 HID interface.
#define CFG_TUD_HID 1

// Define the buffer size for HID reports.
// Our report is only 1 byte, but a buffer of 16 is a safe and standard value.
#define CFG_TUD_HID_REPORT_BUFSIZE 16

#endif /* TUSB_CONFIG_H_ */