# Quantel input device packet format.

## General description
- A packet is formed of 9 bytes.
- They are sent roughly every 10ms, implying a baud-rate of 9600.
- Packets are sent regardless of any input, so have to pay attention to key/pen/button flags.
- The MSb of byte 1 is always 1 to signify the start of a packet. All other bytes have the MSb cleared.
- Byte 9 is the XOR of each of the other bytes, masked to the least significant 7 bits.
- Pen position can be negative and uses 2's compliment, and seems to range from about -948, -528 in the top left, to 6759, 5324 in the bottom right.
- Key events are only sent on key transition, so key repeats have to be handled in software.
- It's probable that key data and pen data aren't transmitted in the same packet because of the use of byte 3, bit 6 for pen pressure. If so, this would probably be unnoticeable on original hardware because the screen refresh would only be 30 FPS.

## Packet structure

| Byte | Bits | Description                                                    |
| ---- | ---- | -------------------------------------------------------------- |
| 1    | 7    | Start of packet - always set                                   |
|      | 6    | Keycode present in packet                                      |
|      | 5    | Pen data is present in packet                                  |
|      | 4    | Unknown                                                        |
|      | 3    | Joystick down                                                  |
|      | 2    | Joystick up                                                    |
|      | 1    | Joystick right                                                 |
|      | 0    | Joystick left                                                  |
| 2    | 7    | Always unset                                                   |
|      | 6    | Button 5, trigger                                              |
|      | 5    | Unknown - always set?                                          |
|      | 4    | Set if key up, unset if key down                               |
|      | 3    | Button 4                                                       |
|      | 2    | Button 3                                                       |
|      | 1    | Button 2                                                       |
|      | 0    | Button 1                                                       |
| 3    | 7    | Always unset                                                   |
|      | 6-0  | If byte 1 bit 6 is set, keycode                                |
|      | 6    | If byte 1 bit 6 is unset, most significant bit of pen pressure |
|      | 5-0  | If byte 1 bit 6 is unset, unknown                              |
| 4    | 7    | Always unset                                                   |
|      | 6-0  | Most significant bits of pen x position                        |
| 5    | 7    | Always unset                                                   |
|      | 6-0  | Least significant bits of pen x position                       |
| 6    | 7    | Always unset                                                   |
|      | 6-0  | Most significant bits of pen y position                        |
| 7    | 7    | Always unset                                                   |
|      | 6-0  | Least significant bits of pen y position                       |
| 8    | 7    | Always unset                                                   |
|      | 6-0  | Least significant bits of pen pressure                         |
| 9    | 7    | Always unset                                                   |
|      | 6-0  | XOR of previous 8 bytes                                        |

## Keycodes
The keyboard used by the Quantel Express (and others?) is the [AKE079339/3 keyboard](https://imgur.com/gallery/quantel-paintboxs-mechanical-keyboard-teardown-lfhp8) How rollover is handled is as yet unknown, but expect odd things if more than one key is held down.

| Key name          | Keycode |
| ----------------- | ---- |
| KEY_DOWN          | 0x00 |
| KEY_ESCAPE        | 0x01 |
| KEY_1             | 0x02 |
| KEY_2             | 0x03 |
| KEY_3             | 0x04 |
| KEY_4             | 0x05 |
| KEY_5             | 0x06 |
| KEY_6             | 0x07 |
| KEY_7             | 0x08 |
| KEY_8             | 0x09 |
| KEY_9             | 0x0A |
| KEY_0             | 0x0B |
| KEY_MINUS         | 0x0C |
| KEY_EQUALS        | 0x0D |
| KEY_DELETE        | 0x0E |
| KEY_TAB           | 0x0F |
| KEY_Q             | 0x10 |
| KEY_W             | 0x11 |
| KEY_E             | 0x12 |
| KEY_R             | 0x13 |
| KEY_T             | 0x14 |
| KEY_Y             | 0x15 |
| KEY_U             | 0x16 |
| KEY_I             | 0x17 |
| KEY_O             | 0x18 |
| KEY_P             | 0x19 |
| KEY_BRACKET_LEFT  | 0x1A |
| KEY_BRACKET_RIGHT | 0x1B |
| KEY_CONTROL       | 0x1D |
| KEY_A             | 0x1E |
| KEY_S             | 0x1F |
| KEY_D             | 0x20 |
| KEY_F             | 0x21 |
| KEY_G             | 0x22 |
| KEY_H             | 0x23 |
| KEY_J             | 0x24 |
| KEY_K             | 0x25 |
| KEY_L             | 0x26 |
| KEY_SEMICOLON     | 0x27 |
| KEY_APOSTROPHE    | 0x28 |
| KEY_SHIFT_LEFT    | 0x2A |
| KEY_NUMBER_SIGN   | 0x2B |
| KEY_Z             | 0x2C |
| KEY_BLANK         | 0x2D |
| KEY_C             | 0x2E |
| KEY_V             | 0x2F |
| KEY_B             | 0x30 |
| KEY_N             | 0x31 |
| KEY_M             | 0x32 |
| KEY_COMMA         | 0x33 |
| KEY_PERIOD        | 0x34 |
| KEY_SLASH         | 0x35 |
| KEY_SHIFT_RIGHT   | 0x36 |
| KEY_ALT           | 0x38 |
| KEY_SPACE         | 0x39 |
| KEY_CAPSLOCK      | 0x3A |
| KEY_X             | 0x55 |
| KEY_BACKSLASH     | 0x56 |
| KEY_DO            | 0x60 |
| KEY_CONT          | 0x61 |
| KEY_REC           | 0x62 |
| KEY_INS           | 0x63 |
| KEY_HELP          | 0x64 |
| KEY_M1            | 0x65 |
| KEY_M2            | 0x66 |
| KEY_M3            | 0x67 |
| KEY_M4            | 0x68 |
| KEY_M5            | 0x69 |
| KEY_M6            | 0x6A |
| KEY_M7            | 0x6B |
| KEY_M8            | 0x6C |
| KEY_M9            | 0x6D |
| KEY_M10           | 0x6E |
| KEY_GRAVE         | 0x70 |
| KEY_RETURN        | 0x71 |
| KEY_SEL           | 0x75 |
| KEY_UP            | 0x76 |
| KEY_LEFT          | 0x77 |
| KEY_RIGHT         | 0x78 |

## Things left to figure out
- The exact range of the tablet.
- Does the range change based on the size of the tablet?
- Is pen data included in a keyboard event?
- Byte 2 bit 5 is always set, but all other unknowns are always unset.
