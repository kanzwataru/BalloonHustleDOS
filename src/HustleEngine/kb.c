#include "src/hustle~1/kb.h"

byte keyboard_read(void)
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