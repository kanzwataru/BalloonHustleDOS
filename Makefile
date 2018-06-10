all:
	dosbox -c "cd C:\DEV\BALLOON" -c "tc balloon.prj /m"

run:
	dosbox -c "cd C:\DEV\BALLOON" -c "balloon.exe"

buildnrun:
	dosbox -c "cd C:\DEV\BALLOON" -c "tc balloon.prj /m" -c "balloon.exe"

watcom:
	dosbox -c "cd C:\DEV\BALLOON" -c "wmake -f balloon.mk"

buildnrun_watcom:
	dosbox -c "cd C:\DEV\BALLOON" -c "wmake -f balloon.mk" -c "balloon.exe"

clean:
	rm -f *.EXE
	rm -f *.OBJ
	rm -f *.DSK
	rm -f *.ERR
	rm -f *.MAP
	rm -f *.LK1

