all: defines.h
	dosbox -c "cd C:\DEV\BALLOON" -c "tc balloon.prj /m"

run:
	dosbox -c "cd C:\DEV\BALLOON" -c "balloon.exe"

buildnrun: defines.h
	dosbox -c "cd C:\DEV\BALLOON" -c "tc balloon.prj /m" -c "balloon.exe"

watcom: defines.h
	dosbox -c "cd C:\DEV\BALLOON" -c "wmake -f balloon.mk"

buildnrun_watcom: defines.h
	dosbox -c "cd C:\DEV\BALLOON" -c "wmake -f balloon.mk" -c "balloon.exe"

clean:
	rm -f *.EXE
	rm -f *.OBJ
	rm -f *.DSK
	rm -f *.ERR
	rm -f *.MAP
	rm -f *.LK1
	rm -f *.SWP
	rm -f defines.h

defines.h:
	touch defines.h

set-debug:
	echo "#define DEBUG" > defines.h

set-no-debug:
	echo "" > defines.h
