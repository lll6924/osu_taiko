osu: osu.o core.o converter.o sound/sound.o
	cc osu.o core.o converter.o sound/sound.o -o osu `pkg-config --libs gtk+-3.0` -lSDL2 -lSDL2_mixer
osu.o: osu.c osu.h
	cc `pkg-config --cflags gtk+-3.0` osu.c -c
core.o: core.c osu.h
	cc `pkg-config --cflags gtk+-3.0` core.c -c 
sound.o: sound/sound.c sound/sound.h
	cc sound/ sound.c  -mmacosx-version-min=10.6 -c
converter.o: converter.c taiko.h
clean:
	rm *.o osu osu.exe