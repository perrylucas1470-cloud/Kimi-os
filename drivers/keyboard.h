#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../include/types.h"

#define KEYBOARD_DATA_PORT   0x60
#define KEYBOARD_STATUS_PORT 0x64

// Special keys
#define KEY_ENTER     0x1C
#define KEY_BACKSPACE 0x0E
#define KEY_SPACE     0x39
#define KEY_SHIFT_L   0x2A
#define KEY_SHIFT_R   0x36
#define KEY_CTRL      0x1D
#define KEY_ALT       0x38
#define KEY_CAPS      0x3A
#define KEY_UP        0x48
#define KEY_DOWN      0x50
#define KEY_LEFT      0x4B
#define KEY_RIGHT     0x4D

void keyboard_init(void);
char keyboard_get_char(void);
bool keyboard_has_input(void);
void keyboard_handler(void);

#endif
