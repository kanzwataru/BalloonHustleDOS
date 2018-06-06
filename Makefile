all:
	dosbox -c "cd C:\DEV\BALLOON" -c "tc balloon.prj /m"

run:
	dosbox -c "cd C:\DEV\BALLOON" -c "balloon.exe"

buildnrun:
	dosbox -c "cd C:\DEV\BALLOON" -c "tc balloon.prj /m" -c "balloon.exe"

clean:
	rm *.EXE
	rm *.OBJ
	rm *.DSK
