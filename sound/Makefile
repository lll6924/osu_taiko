.PHONY: all
all: testsound
testsound: testsound.c sound.c sound.h
	g++ -o testsound testsound.c sound.c -lSDL2 -lSDL2_mixer
.PHONY: clean
clean:
	rm -r *.o testsound testsound.exe