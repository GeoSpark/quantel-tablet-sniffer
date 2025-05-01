#include "quantel.h"

Quantel::Quantel():
status(),
pen_data(),
keycode(0),
checksum(0)
{}


// Set up a simple state machine. When the function returns true we have received a valid packet.
bool Quantel::handle_packet(const uint8_t data) {

    // If the top bit is set, then we're starting a new packet.
    // This means if we start parsing part-way through a packet, we wait until we get a positive start.
    // If a new packet is started before the previous one was finished (such as an interruption to the serial line),
    // we drop the partial packet and start again in a known fresh state.
    if (data & 0x80) {
        current_state = PACKET_STATE_STATUS;
        checksum = 0;
    }

    if (current_state == PACKET_STATE_IDLE) {
        return false;
    }

    switch (current_state) {
        case PACKET_STATE_STATUS:
            ((uint8_t*) &status)[0] = data;
            current_state = PACKET_STATE_BUTTONS;
            break;

        case PACKET_STATE_BUTTONS:
            ((uint8_t*) &status)[1] = data;
            current_state = PACKET_STATE_KEY_CODE;
            break;

        case PACKET_STATE_KEY_CODE:
            if (status.has_keycode) {
                keycode = data;
            } else {
                // Pen pressure MSb is stored in bit 6.
                pen_data.pressure = (data & 0x40) << 1;
            }
            current_state = PACKET_STATE_PEN_X_MSB;
            break;

        case PACKET_STATE_PEN_X_MSB:
            // Twiddle bits to extend the sign.
            // The pen position is a 14 bit signed quantity, so to get the correct sign, we need to do some magic
            // to extend the sign to the full 16 bits.
            pen_data.x = data << 7; // NOLINT(*-narrowing-conversions)
            if (data & 0x40) {
                pen_data.x |= (int16_t)0xc000;
            }
            current_state = PACKET_STATE_PEN_X_LSB;
            break;

        case PACKET_STATE_PEN_X_LSB:
            pen_data.x |= data; // NOLINT(*-narrowing-conversions)
            current_state = PACKET_STATE_PEN_Y_MSB;
            break;

        case PACKET_STATE_PEN_Y_MSB:
            // Twiddle bits to extend the sign.
            pen_data.y = data << 7; // NOLINT(*-narrowing-conversions)
            if (data & 0x40) {
                pen_data.y |= (int16_t)0xc000;
            }
            current_state = PACKET_STATE_PEN_Y_LSB;
            break;

        case PACKET_STATE_PEN_Y_LSB:
            pen_data.y |= data; // NOLINT(*-narrowing-conversions)
            current_state = PACKET_STATE_PEN_PRESSURE;
            break;

        case PACKET_STATE_PEN_PRESSURE:
            pen_data.pressure |= data; // NOLINT(*-narrowing-conversions)
            current_state = PACKET_STATE_CHECKSUM;
            break;

        case PACKET_STATE_CHECKSUM:
            current_state = PACKET_STATE_IDLE;

            if ((checksum & 0x7f) == data) {
                return true;
            }
            break;

        default:
            break;
    }

    checksum ^= data;

    return false;
}
