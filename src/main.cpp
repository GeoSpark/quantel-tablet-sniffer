#include <kbd_ake079339.h>
#include <quantel.h>

#include <Arduino.h>
#include <usb_keyboard.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

#define TABLET_MAX_X 6759
#define TABLET_MAX_Y 5324

Quantel q;

// uint8_t example_data[9] = {0xA0, 0x20, 0x40, 0x30, 0x6B, 0x7F, 0x63, 0x5B, 0x5C};

void setup() {
    Serial.begin(115200);
    Serial1.begin(9600);
    Mouse.screenSize(SCREEN_WIDTH, SCREEN_HEIGHT, true);
}

void loop() {
    if (Serial1.available() > 0 && q.handle_packet(Serial1.read())) {
        if (q.get_status().has_pen_data) {
            uint16_t pen_x = map(q.get_pen_data().x, 0, TABLET_MAX_X, 0, SCREEN_WIDTH);
            constrain(pen_x, 0, SCREEN_WIDTH);
            uint16_t pen_y = map(q.get_pen_data().y, 0, TABLET_MAX_Y, 0, SCREEN_HEIGHT);
            constrain(pen_y, 0, SCREEN_HEIGHT);
            Mouse.moveTo(pen_x, pen_y);

            // TODO: Figure out pen pressure.
            // TODO: Handle buttons.

            Serial.print("Pen X: ");
            Serial.println(q.get_pen_data().x);
            Serial.print("Pen Y: ");
            Serial.println(q.get_pen_data().y);
            Serial.print("Pen pressure: ");
            Serial.println(q.get_pen_data().pressure);
        }

        if (q.get_status().has_keycode) {
            if (q.get_status().key_up) {
                Keyboard.release(USB_HID_MAPPING_AKE079339[q.get_keycode()]);
            } else {
                Keyboard.press(USB_HID_MAPPING_AKE079339[q.get_keycode()]);
            }
            Serial.print("USB HID keycode: ");
            Serial.println(USB_HID_MAPPING_AKE079339[q.get_keycode()]);
        }
    }
}
