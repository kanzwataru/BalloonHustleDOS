#include "platform/kb.h"

#define RELEASED(x) ((x) | 0x80)

static byte keyboard_read(void)
{
    byte key;
    _asm {
        in  al,  60h    /* get key from keyboard input port */
        mov key, al

        in  al,  61h    /* send acknowledge (???) */
        or  al,  128
        out 61h, al
        xor al,  128
        out 61h, al
    }

    *(char*)(0x0040001A) = 0x20; /* flush buffer */
    *(char*)(0x0040001C) = 0x20; /*              */

    return key;
}

void keyboard_per_frame_update(void) {}

bool keyboard_os_quit_event(void)
{
    return false; /* DOS doesn't have an OS-level way of quitting */
}

bool keyboard_poll_keypress(Keypress *kp) {}
