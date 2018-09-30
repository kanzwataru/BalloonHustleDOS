# Wrecking Balloon for MS-DOS
###### Currently very much a work in progress

MS-DOS port/remake of gamejam game Wrecking Balloon

https://nickladd.itch.io/wrecking-balloon

Original graphics by Nick Ladd, DOS graphics by me

Original code by Nathan Ladd and myself

![screenshot](https://i.imgur.com/jOATCKm.png)

### What is this!
Just a fun project to create a game from scratch for early 90s era
PC compatibles running MS-DOS. I mostly do high-level Python stuff
so I wanted to learn more low-level programming and what better way
than to program the PC hardware almost directly.

### How do I play it?
You'll need either DOSBOX or actual 90s PC hardware.
(or boot your modern PC using FreeDOS if you really want)

Just grab the lastest release from the releases tab.

### How do I compile it?
This should work with either OpenWatcom or the old Borland C++ compiler.
DJGPP is not supported because this is a purely 16-bit real mode program and 
assumes a segmented memory model, although I am planning to port
it to SDL (and thus modern systems) in the future. 

There is a make file that launches DOSBOX and has targets for both compilers.
###### (currently has a hardcoded path!)

### What's going on?
There is a very bare-bones game "engine" under HustleEngine which is responsible
for drawing/loading the graphics, running the game loop and responding to input.
*currently no audio support*

Mostly written in C with some assembly. A lot of this is referenced from a couple of
really old DOS game tutorials, listed below. The MS-DOS platform does not provide much,
mostly file loading and printing to the terminal. Most functionality including
graphics, input, and sound is written by poking memory locations and I/O ports directly,
although the PC BIOS does have some handy built-in routines called by interrupts.

### Why does it break every single coding rule!!
I only have about a year or so experience programming in C, so the quality is likely not
all that good. Combined with having to use C89 and squeezing every last byte to get somewhat
OK performance on ancient hardware, the result is that this probably is a bit of an abomination
code-wise. Sorry about that!

### I want to also develop MS-DOS games
Well check out these tutorials then, they're what I used to get started. And of course, 
you can take a look at the code in the HustleEngine directory and see how stuff works.

[256-Color VGA Programming in C](http://www.brackeen.com/vga/)

[Alex Russel's DOS Game Programming Tutorial](http://www3.telus.net/alexander_russell/course/introduction.htm)


