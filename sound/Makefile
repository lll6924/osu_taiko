.PHONY: all
all: testsound.exe
testsound.exe: testsound.c sound.c sound.h
	g++ -o testsound.exe testsound.c sound.c -lSDL2 -lSDL2_mixer
.PHONY: clean
clean:
	del *.o *.exe