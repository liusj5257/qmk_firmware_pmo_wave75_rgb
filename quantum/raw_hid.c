// Copyright 2025 QMK
// SPDX-License-Identifier: GPL-2.0-or-later

#include "raw_hid.h"
#include "host.h"

#if defined(PROTOCOL_CHIBIOS)
// Implemented by the ChibiOS USB driver (tmk_core/protocol/chibios/usb_main.c).
void send_raw_hid(uint8_t *data, uint8_t length);
#endif

void raw_hid_send(uint8_t *data, uint8_t length) {
#if defined(PROTOCOL_CHIBIOS)
    // Raw HID is USB only, so it must not be routed through the currently
    // active host driver. Some keyboards install a wireless host driver that
    // does not implement `send_raw_hid` (e.g. the prebuilt library used by
    // hm/wave75_rgb); routing through it would silently drop every reply sent
    // back to VIA.
    send_raw_hid(data, length);
#else
    host_raw_hid_send(data, length);
#endif
}

__attribute__((weak)) void raw_hid_receive(uint8_t *data, uint8_t length) {
    // Users should #include "raw_hid.h" in their own code
    // and implement this function there. Leave this as weak linkage
    // so users can opt to not handle data coming in.
}
