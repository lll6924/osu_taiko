#include "sound.h"

int main() {
    char ch;
    initSoundPlayer();
    while ((ch = getchar()) != 'q') {
        switch (ch) {
            case 'a': sound(0);break;
            case 'b': sound(1);break;
            case 'c': sound(2);break;
            case 's': stopAll();break;
            default : break;
        }
    }
    return 0;
}
