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
###### (there is an experimental linux/mac build)

Just grab the lastest release from the releases tab.

### What's going on?
There is a very bare-bones game "engine" called HustleEngine which is responsible
for drawing/loading the graphics, running the game loop and responding to input.
*currently no audio support*

Mostly written in C with some assembly. A lot of this is referenced from a couple of
really old DOS game tutorials, listed below. The MS-DOS platform does not provide much,
mostly file loading and printing to the terminal. Most functionality including
graphics, input, and sound is written by poking memory locations and I/O ports directly,
although the PC BIOS does have some handy built-in routines called by interrupts.

The header `common/platform.h` is included in almost all files and
bridges the gap between different compilers and systems. 
There is a clear separation between the part the platform-specific bits and the engine's logic,
to allow for portability. The `platform` folder contains headers with an interface that
each platform implements in its own .c files. The interface is generally based on what is
the most direct and fast way to do it on DOS machines and as such not necessarily the most friendly
or universal.

### Code quality disclaimer
I only have about a year or so experience programming in C, so the quality is likely not
all that good. Combined with having to use C89 and trying to get somewhat
OK performance on ancient hardware, the result is that this probably is a bit of an abomination
code-wise. Sorry about that!

### How do I compile it?
This should work with either OpenWatcom or the old Borland C++ compiler.
DJGPP is not supported because this is a purely 16-bit real mode program and 
assumes a segmented memory model (on DOS, at least). There is also an experimental SDL/SDL2 build
for unix-like systems.

There is a make file that includes other make files depending on environment variables.
Simply set the vars to control which platform to build for. Here is a list of variables:

* `TARGET_PLATFORM`
    * `dos`    - Compile with Borland Turbo C++ inside DOSBOX
    * `doswcc` - Cross-compile for DOS with native OpenWatcom (only tested on linux)
    * `unix`   - Native build (SDL backend) for unix-like systems

* `SDL_VER`
    * Can be either `1` or `2`. On SDL platforms (currently unix-only) chooses between SDL1.2 or SDL2

* `DEBUG_BUILD`
    * Build with asserts enabled, debug symbols, and no optimization

Apart from this there are some extra targets for DOS builds (`buildnrun` and `run`) which can start the game in DOSBox

UNIX Example:
```
$ export TARGET_PLATFORM=unix
$ export DEBUG_BUILD=1
$ export SDL_VER=2
$ make -j4 && ./balloon
```

DOS Example:
```
$ export TARGET_PLATFORM=doswcc
$ export DEBUG_BUILD=0
$ make buildnrun
```
###### (currently the dosbox environment has a hardcoded path! and it assumes certain autoexec settings!)

### How do I get started creating DOS games?
Check out these tutorials, they're what I used to get started. It wouldn't hurt to also
learn about the IBM PC hardware architecture and the role of MS-DOS (it doesn't actually do all that much!)
Also feel free to take a look at the code in the HustleEngine directory and see how stuff works.

[256-Color VGA Programming in C](http://www.brackeen.com/vga/)

[Alex Russel's DOS Game Programming Tutorial](http://www3.telus.net/alexander_russell/course/introduction.htm)

And of course, some old DOS games have been open-sourced so you can learn all of their tricks.

[Wolfenstein 3D](https://github.com/id-Software/wolf3d)

[Keen Dreams](https://github.com/keendreams/keen)


