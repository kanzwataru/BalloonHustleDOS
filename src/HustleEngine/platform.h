#ifndef PLATFORM_H
#define PLATFORM_H

#include <malloc.h>

#ifdef __WATCOMC__ /* watcom support */
    #include <mem.h>
    #include <conio.h>
    #define farmalloc(a)      _fmalloc(a)
    #define farcalloc(a, b)   _fcalloc(a, b)
    #define farfree(a)        _ffree(a)
    #define far __far
    #define MK_PTR(seg,off)   (((seg) << 4) + (off))
    #define inport(a)         inpw(a)
    #define inportb(a)        inp(a)
    #define outport(a, b)     outpw(a, b)
    #define outportb(a, b)    outp(a, b)
#endif /* compiler */

#ifdef __GNUC__ /* modern 32-bit OS support (future) */
    #include <stdint.h>
    #include <stdbool.h>
    #define far         /* no more far pointers */

    typedef uint8_t buffer_t;
    typedef uint8_t byte;
    typedef uint16_t uint;
    typedef uint32_t ulong;

#else           /* 16-bit DOS */
    typedef int bool;
    #define true 1
    #define false 0

    typedef unsigned char far buffer_t;
    typedef unsigned char byte;
    typedef unsigned int uint;
    typedef unsigned long ulong;
#endif /* platform */
/* */

#endif /* PLATFORM_H */