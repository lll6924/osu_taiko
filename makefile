osu: osu.o core.o converter.o
	cc osu.o core.o converter.o -o osu `pkg-config --libs gtk+-3.0`
osu.o: osu.c osu.h
	cc `pkg-config --cflags gtk+-3.0` osu.c -c 
core.o: core.c osu.h
	cc `pkg-config --cflags gtk+-3.0` core.c -c
converter.o: converter.c taiko.h
clean:
	rm *.o osu osu.exe