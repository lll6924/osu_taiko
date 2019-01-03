#include "taiko.h"
#include<string.h>
#include<stdio.h>
#include<math.h>


void print(struct pin p) {
    printf("Loc %d range %d type %d size %d color %d\n", p.locationx, p.range, p.type, p.size, p.color);
}

int printTo(char* data, int len, char* outFilename) {
    if (outFilename == NULL) outFilename = "mem.bin";
    FILE * of = fopen(outFilename, "wb");
    if (fwrite(data, sizeof(char), len, of) < 0) printf("write error!\n");
    else printf("write success!\n");
    fclose(of);
}

///given input and output file name, generate bin file, store data in array (struct pin*)data
///returns number of hitpoints
int convert(char * filename, char *outFilename, struct pin* data) {
    //printf("size: %d\n", sizeof(struct pin) * MAX_HITS);
    int cnt = 0;

    int len = strlen(filename);
    char *ptr = filename + len - 4;
    if (strcmp(ptr, ".osu") != 0) {
        printf("filename error!\n");
        return 0;
    }

    if (outFilename == NULL) outFilename = "test.bin";

    FILE * f = fopen(filename, "r");
    char * s = (char *) malloc(MAX_LEN + 5);

    int status = 0;
    int x, y, time, type, hitsound;
    while (fgets(s, MAX_LEN, f) != NULL) {
        if (status) {
            if (strlen(s) < 10) break;
            sscanf(s, "%d,%d,%d,%d,%d", &x, &y, &time, &type, &hitsound);
            data[cnt].locationx = round(time / 10.0);

            if (type & 8) data[cnt].type = denden;
            if (type & 2) data[cnt].type = drumroll;
            if (type & 1) data[cnt].type = note;

            if (type & 4) data[cnt].combo = 1;

            data[cnt].size = (hitsound & 4) ? 1 : 0;
            data[cnt].color = (hitsound & 8) ? 0 : 1;

            data[cnt].range = 50;

            //print(data[cnt]);
            cnt++;
        }
        if (strncmp(s, "[HitObjects]", 12) == 0) status = 1;
    }
    fclose(f);
    free(s);
    printf("hitpoints: %d\n", cnt);
    return cnt;
}

unsigned char* generate_bin(char **names, int filenum) {
    int pos = 0, offset = 0;
    int i, cnt = 0;
    unsigned char* mem, ptr;
    struct beatMap* maps;
    struct pin* datas;
    mem = (unsigned char *) malloc(1024 * 1024);
    ptr = mem;
    maps = (struct beatMap*) mem;
    offset = sizeof(struct beatMap) * MAX_MAPS;
    printf("offset: %d\n", offset);

    datas = (struct pin*) &(maps[MAX_MAPS]);

    for (i = 0; i < filenum; i++) {
        printf("%s\n", names[i]);
        maps[i].offset = offset;
        maps[i].len = convert(names[i], NULL, datas);
        offset += maps[i].len *sizeof(struct pin);
        datas = &(datas[maps[i].len]);
    }
    printf("Total length: %d\n", offset);
    printTo(mem, offset, NULL);

    return mem;
}


/*int main() {
    char ** filenames = (char **) malloc(sizeof(char *) * MAX_MAPS);
    filenames[0] = "test0.osu";
    filenames[1] = "test1.osu";
    char *mem;
    mem = generate_bin(filenames, 2);
    struct pin* data = getMaps(1, mem);
    print(data[0]);
    free(filenames);
    free(mem);
    return 0;
}*/
