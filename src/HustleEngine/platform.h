#ifndef PLATFORM_H
#define PLATFORM_H

#include <malloc.h>

#ifndef __BORLANDC__ /* watcom support */
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

#endif /* PLATFORM_H */