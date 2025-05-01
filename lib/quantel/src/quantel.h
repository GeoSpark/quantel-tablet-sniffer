#ifndef QUANTEL_H
#define QUANTEL_H

#include <cstdint>

struct status_t {
    uint8_t packet_start: 1; // Always 1
    uint8_t has_keycode: 1;
    uint8_t has_pen_data: 1;
    uint8_t unknown1: 1;
    uint8_t joystick_down: 1;
    uint8_t joystick_up: 1;
    uint8_t joystick_right: 1;
    uint8_t joystick_left: 1;
    uint8_t byte_marker: 1; // Always 0
    uint8_t button_5: 1;
    uint8_t unknown2: 1;
    uint8_t key_up: 1;
    uint8_t button_4: 1;
    uint8_t button_3: 1;
    uint8_t button_2: 1;
    uint8_t button_1: 1;
};

struct pen_data_t {
    int16_t x;
    int16_t y;
    uint8_t pressure;
};

class Quantel {
public:
    Quantel();

    status_t get_status() const { return status; }
    pen_data_t get_pen_data() const { return pen_data; }
    uint8_t get_keycode() const { return keycode; }

    bool handle_packet(uint8_t data);

protected:
    enum PACKET_STATE {
        PACKET_STATE_STATUS,
        PACKET_STATE_BUTTONS,
        PACKET_STATE_KEY_CODE,
        PACKET_STATE_PEN_X_MSB,
        PACKET_STATE_PEN_X_LSB,
        PACKET_STATE_PEN_Y_MSB,
        PACKET_STATE_PEN_Y_LSB,
        PACKET_STATE_PEN_PRESSURE,
        PACKET_STATE_CHECKSUM,
        PACKET_STATE_IDLE = 255
    };

    PACKET_STATE current_state = PACKET_STATE_IDLE;
    status_t status;
    pen_data_t pen_data;
    uint8_t keycode;
    uint8_t checksum;
};

#endif //QUANTEL_H
