osu: osu.o core.o
	cc osu.o core.o -o osu `pkg-config --libs gtk+-3.0`
osu.o: osu.c osu.h
	cc `pkg-config --cflags gtk+-3.0` osu.c -c 
core.o: core.c osu.h
	cc `pkg-config --cflags gtk+-3.0` core.c -c
clean:
	rm *.o osu osu.exe