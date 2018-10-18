#ifndef PLATFORM_H
#define PLATFORM_H

#include <malloc.h>
#include <stdio.h>

/***                      ***
 ***    COMPILER CHECK    ***
 ***                      ***/
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
/******************************************************/


/***                      ***
 ***    PLATFORM CHECK    ***
 ***                      ***/
#ifdef __GNUC__ 
    /* Modern 32-bit OS support (future) 
     *
     * DJGPP may also define this, so not guaranteed
     * to not be DOS, but DJGPP is not supported anyway
     */
    #define PLATFORM_SDL
    #include <stdint.h>
    #include <stdbool.h>
    #define far /* no more far pointers */

    typedef uint8_t buffer_t;
    typedef uint8_t byte;
    typedef uint16_t uint;
    typedef uint32_t ulong;

    #define PANIC(f) do { f; exit(1); } while(0);
#else           
    /* 16-bit DOS
     *
     * This is for both Borland and Watcom
     */
    #define PLATFORM_DOS
    #include <mem.h>
    #include <dos.h>
    #define inline /* no inline on C89 */
    typedef int bool;
    #define true 1
    #define false 0

    typedef unsigned char far buffer_t;
    typedef unsigned char byte;
    typedef unsigned int uint;
    typedef unsigned long ulong;
    
    #define PANIC(msg) while(1) { printf("!! %s (%d %d)\n", (msg), __FILE__, __LINE__); }
#endif /* platform */
/******************************************************/
#ifdef DEBUG
    static void assertion_failed(const char *file, int line) {
        while(1) printf("Assert fail %s, %d\n", file, line);
    }
    
    #define NOT_IMPLEMENTED \
        PANIC("Not implemented");    

    #define assert(expr) \
        if(expr) {} else assertion_failed(__FILE__, __LINE__)
#else /* if not debug */
    #define NOT_IMPLEMENTED /* disabled */
    #define assert(expr)    /* disabled */
#endif

#endif /* PLATFORM_H */