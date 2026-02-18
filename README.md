# esp32-web-keyboard

I needed to set up a PC but didn't have a USB keyboard available. Didn't want to buy one, didn't want to wait until the next day, didn't want to grab one from the office. An ESP32-S3 was sitting on my desk, so I built this instead.

The ESP32 creates a WiFi access point. Connect to it, open `192.168.4.1` in a browser, and you get a full QWERTZ keyboard in your phone's browser. The ESP32 is connected via USB to the target PC and shows up as a standard HID keyboard — no drivers, no setup on the target machine.

## Requirements

- ESP32-S3 (the S3 specifically, it has native USB support)
- USB cable (ESP32 to target PC)
- Any device with a browser to use as the keyboard

## Setup

1. Open the sketch in Arduino IDE
2. Set board to **ESP32S3 Dev Module**
3. Set `USB CDC On Boot` to **Enabled**
4. Flash the ESP32 and plug it into the target PC
5. Connect to the WiFi network `ESP-Keyboard` (password: `12345678`)
6. Open `192.168.4.1` in your browser

## Features

- Full QWERTZ layout including umlauts
- F1–F12, ESC, Tab, Caps Lock, Enter, Backspace
- Modifier keys (Ctrl, Shift, Alt, AltGr, Win) work as sticky keys — press once, they stay active until the next keypress
- Navigation keys, arrow keys, numpad

## Limitations

- No visual feedback while typing
- AltGr combinations not tested on macOS
- WiFi password is hardcoded in plaintext
- Not suitable for fast typing, but works fine for BIOS access, initial OS setup, or any one-off input

## Notes

Most existing approaches I found use `Keyboard.press()` with ASCII values instead of raw HID scan codes, which produces wrong characters on non-US layouts. This project uses `Keyboard.pressRaw()` throughout, so it works correctly regardless of what keyboard layout the target OS is set to.

## License

MIT
