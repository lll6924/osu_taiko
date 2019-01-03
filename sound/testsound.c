#include "sound.h"

int main() {
    char ch;
    initSoundPlayer();
    int i = 0;
    while ((ch = getchar()) != 'q') {
        switch (ch) {
            case 'a': sound(0); i = 0; break;
            case 'b': sound(1); i = 1; break;
            case 'c': sound(2); i = 2; break;
            case 'd': sound(3); i = 3; break;
            case 's': stopAll();break;
            default : sound(i); break;
        }
    }
    closeSoundPlayer();
    return 0;
}
