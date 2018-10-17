all:
	dosbox -c "cd C:\DEV\BALLOON" -c "make -f MKTURBOC"

run:
	dosbox -c "cd C:\DEV\BALLOON" -c "balloon.exe"

buildnrun:
	dosbox -c "cd C:\DEV\BALLOON" -c "make -f MKTURBOC" -c "balloon.exe"

watcom:
	dosbox -c "cd C:\DEV\BALLOON" -c "wmake -f MKDOSWC"

buildnrun_watcom:
	dosbox -c "cd C:\DEV\BALLOON" -c "wmake -f MKDOSWC" -c "balloon.exe"

clean:
	rm -f *.EXE
	rm -f *.OBJ
	rm -f *.DSK
	rm -f *.ERR
	rm -f *.MAP
	rm -f *.LK1
	rm -f *.SWP
	rm -f *.CFG
	find . -type f -name '._*' -delete
