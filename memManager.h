#ifndef MEM_MANAGER_H_
#define MEM_MANAGER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "taiko.h"

int convert(unsigned char * filename, char *outFilename, struct pin* data);        ///convert osu files to data

int generate_text_bin(char **names, int filenum, unsigned char *headAddr);

int generate_pic_bin(char **bin_filenames, int filenum, unsigned char *headAddr);

unsigned char* generate_mem_bin();

int printTo(unsigned char* data, int len, char* outFilename);

void release_mem(unsigned char *data);

#endif // MEM_MANAGER_H_
