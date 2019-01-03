#ifndef SOUND_H_
#define SOUND_H_
#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sound(int id, int times);

int initSoundPlayer(void (*callbackFunc)(int));

int closeSoundPlayer();

void endPlay(int channel);

void stopAll();

void stop(int id);

#endif // SOUND_H_
