#include "taiko.h"
#include<string.h>
#include<stdio.h>
#include<math.h>
#define MAX_LEN 100
#define MAX_HITS 2000

void print(struct pin p) {
    printf("Loc %d range %d type %d size %d color %d\n", p.locationx, p.range, p.type, p.size, p.color);
}

int printTo(struct pin* data, int len, char* outFilename) {
    FILE * of = fopen(outFilename, "wb");
    if (fwrite(data, sizeof(struct pin), len, of) < 0) printf("write error!\n");
    else printf("write success!\n");
    fclose(of);
}

int convert(char * filename, char *outFilename, struct pin* data) {
    printf("size: %d\n", sizeof(struct pin) * MAX_HITS);
    int cnt = 0;

    int len = strlen(filename);
    char *ptr = filename + len - 4;
    if (strcmp(ptr, ".osu") != 0) {
        printf("filename error!\n");
        return 0;
    }

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

            data[cnt].size = (hitsound & 4) ? 1 : 0;
            data[cnt].color = (hitsound & 8) ? 1 : 0;

            data[cnt].range = 50;

            //print(data[cnt]);
            cnt++;
        }
        if (strncmp(s, "[HitObjects]", 12) == 0) status = 1;
    }
    fclose(f);
    free(s);

    printTo(data, cnt, outFilename);
}

/*int main() {
    struct pin* data = (struct pin*) malloc(sizeof(struct pin) * MAX_HITS);
    convert("test.osu", "test.bin", data);
    free(data);
    return 0;
}*/
