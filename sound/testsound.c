#include "sound.h"

int main() {
    char ch;
    initSoundPlayer();
    while ((ch = getchar()) != 'q') {
        switch (ch) {
            case 'a': sound(0);break;
            case 'b': sound(1);break;
            case 's': stopAll();break;
            default : sound(2);break;
        }
    }
    closeSoundPlayer();
    return 0;
}
