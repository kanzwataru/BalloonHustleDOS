all:
	dosbox -c "cd C:\DEV\BALLOON" -c "tc balloon.prj /m" -c "exit"

run:
	dosbox -c "cd C:\DEV\BALLOON" -c "balloon.exe"