#include "sound.h"
#define NUM_WAVEFORMS 8
#define BGM_NUM 5
const char* _waveFileNames[NUM_WAVEFORMS] =
{
    "1.wav",
    "2.wav",
    "../songs/2.wav",
    "../songs/5.wav",
    "../songs/6.wav",
    "hit.wav",
    "hit2.wav",
    "combo.wav"
};

Mix_Chunk* _sample[NUM_WAVEFORMS];

void (*endPlayCallback)(int);

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

    result = Mix_ReserveChannels(BGM_NUM);
    if ( result < 0 )
    {
        fprintf(stderr, "Unable to reserve mixing channels: %s\n", SDL_GetError());
        return 0;
    }

    Mix_ChannelFinished(endPlay);

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

void endPlay(int channel) {
    //printf("endplay %d\n", channel);
    if (channel < BGM_NUM) (*endPlayCallback)(channel);
}

int initSoundPlayer(void (*callbackFunc)(int)) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO );
    atexit(SDL_Quit);
    endPlayCallback = callbackFunc;
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

int sound(int id, int times) {
    if (id >= 0 && id < NUM_WAVEFORMS) {
        if (id < BGM_NUM) Mix_PlayChannel(id, _sample[id], times);
        else
            Mix_PlayChannel(-1, _sample[id], times);
    }
}

void stopAll() {
    Mix_HaltChannel(-1);
}

void stop(int id) {
    Mix_HaltChannel(id);
}
