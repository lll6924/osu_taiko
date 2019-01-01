#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NUM_WAVEFORMS 3
const char* _waveFileNames[NUM_WAVEFORMS] =
{
    "1.wav",
    "2.wav",
    "hit.wav",
};

Mix_Chunk* _sample[NUM_WAVEFORMS];

// Initializes the application data
int Init(void)
{
    memset(_sample, 0, sizeof(Mix_Chunk*) * NUM_WAVEFORMS);

    // Set up the audio stream
    int result = Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512);
    if( result < 0 )
    {
        fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
        exit(-1);
    }

    result = Mix_AllocateChannels(2 * NUM_WAVEFORMS);
    if( result < 0 )
    {
        fprintf(stderr, "Unable to allocate mixing channels: %s\n", SDL_GetError());
        exit(-1);
    }

    // Load waveforms
    for( int i = 0; i < NUM_WAVEFORMS; i++ )
    {
        _sample[i] = Mix_LoadWAV(_waveFileNames[i]);
        if( _sample[i] == NULL )
        {
            fprintf(stderr, "Unable to load wave file: %s\n", _waveFileNames[i]);
        }
    }

    return true;
}

int main(int argc, char** argv)
{
    // Initialize the SDL library with the Video subsystem
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO );
    atexit(SDL_Quit);
    // Application specific Initialize of data structures
    if (Init() == false)
        return -1;

    char ch;
    while ((ch = getchar()) != 'q')
    {
        switch (ch)
        {
            case 'a':
                Mix_PlayChannel(-1, _sample[0], 0);
                break;
            case 'b':
                Mix_PlayChannel(-1, _sample[1], 0);
                break;
            case 'c':
                Mix_PlayChannel(-1, _sample[2], 0);
                break;
            case 's':
                Mix_HaltChannel(-1);
                break;
            default:
                break;
        }
    }

    for( int i = 0; i < NUM_WAVEFORMS; i++)
    {
        Mix_FreeChunk(_sample[i]);
    }

    Mix_CloseAudio();
    SDL_Quit();
    return 0;
}
