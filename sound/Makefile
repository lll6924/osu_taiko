.PHONY: all
all: testsound
testsound: testsound.c sound.c sound.h
	cc -o testsound testsound.c sound.c -lSDL2 -lSDL2_mixer -mmacosx-version-min=10.6
.PHONY: clean
clean:
	rm -r *.o testsound testsound.exe