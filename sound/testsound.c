#include "sound.h"

SDL_AudioSpec manager;

void test(int x) {
    printf("%d\n", x);
}
int main() {
    char ch;
    initSoundPlayer(&test);
    int i = -1;
    while ((ch = getchar()) != 'q') {
        switch (ch) {
            case 'a': sound(0); i = 0; break;
            case 'b': sound(1); i = 1; break;
            case 'c': sound(2); i = 2; break;
            case 'd': sound(3); i = 3; break;
            case 'e': sound(4); i = 4; break;
            case 's': stopAll(); i = -1;break;
            default : if (i >= 2) sound(i); break;
        }
    }
    closeSoundPlayer();
    return 0;
}
