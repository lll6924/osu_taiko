#include "osu.h"
#include "memManager.h"

char *mem;

int pic_offset = 57644,
    bkg_offset = 0,
    numbers_offset = 7,
    faces_offset = 3,
    maps_offset = 0,
    combo_offset = 2,
    instructions_offset = 1;

void init_mem() {
    mem = (char *) generate_mem_bin();
    ///to be modified in final version
    ///mem = starting addr of baseRAM
}

unsigned char* getbkg() {
    return (unsigned char*) (mem + pic_offset + bkg_offset * sizeof(char *));
}

unsigned char** getNumbers() {
    return (unsigned char**) (mem + pic_offset + numbers_offset * sizeof(char *));
}

unsigned char** getFaces() {
    return (unsigned char**) (mem + pic_offset + faces_offset * sizeof(char *));
}
unsigned char* getMapMem() {
    return (unsigned char*) (mem + maps_offset);
}

unsigned char* getCombo() {
    return (unsigned char*) (mem + pic_offset + combo_offset * sizeof(char *));
}
unsigned char* getInstructions() {
    return (unsigned char*) (mem + pic_offset + instructions_offset * sizeof(char *));
}

void destruct_mem() {
    release_mem((unsigned char*) mem);
    ///to be removed in final version
}

int main() {
    init_mem();

    destruct_mem();
    return 0;
}
