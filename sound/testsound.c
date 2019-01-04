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
            case 'a': sound(0, 0); i = 0; break;
            case 'b': sound(1, 0); i = 1; break;
            case 'c': sound(2, 0); i = 2; break;
            case 'd': sound(3, 0); i = 3; break;
            case 'e': sound(4, 0); i = 4; break;
            case 'f': sound(5, 0); i = 5; break;
            case 'g': sound(6, 0); i = 6; break;
            case 's': stopAll(); i = -1;break;
            default : if (i >= 2) sound(i, 0); break;
        }
    }
    closeSoundPlayer();
    return 0;
}
