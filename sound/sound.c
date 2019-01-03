#include "sound.h"
#define NUM_WAVEFORMS 3
const char* _waveFileNames[NUM_WAVEFORMS] =
{
    "1.wav",
    "2.wav",
    "hit.wav",
};

Mix_Chunk* _sample[NUM_WAVEFORMS];

int Init()
{
    memset(_sample, 0, sizeof(Mix_Chunk*) * NUM_WAVEFORMS);

    // Set up the audio stream
    int result = Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 512);
    if( result < 0 )
    {
        fprintf(stderr, "Unable to open audio: %s\n", SDL_GetError());
        return 0;
    }

    result = Mix_AllocateChannels(16);
    if( result < 0 )
    {
        fprintf(stderr, "Unable to allocate mixing channels: %s\n", SDL_GetError());
        return 0;
    }

    // Load waveforms
    int i;
    for(i = 0; i < NUM_WAVEFORMS; i++ )
    {
        _sample[i] = Mix_LoadWAV(_waveFileNames[i]);
        if( _sample[i] == NULL )
        {
            fprintf(stderr, "Unable to load wave file: %s\n", _waveFileNames[i]);
        }
    }

    return 1;
}

int initSoundPlayer() {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO );
    atexit(SDL_Quit);
    if (Init() == 0)
        return -1;
    else return 0;
}

int closeSoundPlayer() {
    int i;
    for(i = 0; i < NUM_WAVEFORMS; i++)
    {
        Mix_FreeChunk(_sample[i]);
    }

    Mix_CloseAudio();
    SDL_Quit();
}

int sound(int id) {
    if (id >= 0 && id < NUM_WAVEFORMS) {
        Mix_PlayChannel(-1, _sample[id], 0);
    }
}

void stopAll() {
    Mix_HaltChannel(-1);
}
