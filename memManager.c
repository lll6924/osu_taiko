#include "memManager.h"
#define PIC_NUM 3
#define MAX_BINFILE_SIZE 500000

unsigned char *data, *picHeader, *textHeader;

char face_str[20] = "bins/facex.bin";
char score_str[20] = "bins/score-x.bin";
char combo_str[20] = "bins/combo-x.bin";
char count_str[20] = "bins/count-x.bin";

int generate_pic_bin(char **bin_filenames, int filenum, unsigned char *headAddr) {
    int i = 0, offset, len;

    unsigned char **header = (unsigned char **) headAddr;
    offset = filenum * sizeof(char *);

    printf("%d\n", offset);

    unsigned char *ptr = &(headAddr[offset]);

    FILE *f;
    for (i = 0; i < filenum; i++) {
        f = fopen(bin_filenames[i], "rb");
        len = fread(ptr, sizeof(char), MAX_BINFILE_SIZE, f);
        if (len < 0) {
            printf("Error reading files!\n");
            continue;
        }
        else printf("This pic length: %d\n", len);
        header[i] = ptr;
        ptr += len;
        offset += len;
        ///printf("%d\n", offset);
        fclose(f);
    }
    printf("Total pic length: %d\n\n", offset);
    return offset;
}

unsigned char* generate_mem_bin() {
    int mem_size = 3 * 1024 * 1024;

    char ** osu_filenames = (char **) malloc(sizeof(char *) * MAX_MAPS);
    osu_filenames[0] = "test0.osu";
    osu_filenames[1] = "test1.osu";

    char ** pic_filenames = (char **) malloc(sizeof(char *) * PIC_NUM);

    pic_filenames[0] = "bkg.bin";
    pic_filenames[1] = "instructions.bin";
    pic_filenames[2] = "bins/combo.bin";

    int i;
    char face_str[] = "bins/facex.bin";
    for (i = 3; i < 7; i++) {
        face_str[9] = i - 3 + '0';
        pic_filenames[i] = new char[20];
        strcpy(pic_filenames[i], face_str);
    }

    ///printf("name: %s\n", pic_filenames[3]);

    for (i = 7; i < 17; i++) {
        score_str[11] = i - 7 + '0';
        pic_filenames[i] = new char[17];
        strcpy(pic_filenames[i], score_str);
        //printf("name: %s\n", pic_filenames[i]);
    }

    ///printf("name: %s\n", pic_filenames[3]);

    for (i = 17; i < 27; i++) {
        combo_str[11] = i - 17 + '0';
        pic_filenames[i] = new char[17];
        strcpy(pic_filenames[i], combo_str);
        //printf("name: %s\n", pic_filenames[i]);
    }

    ///printf("name: %s\n", pic_filenames[3]);

    for (i = 27; i < 37; i++) {
        ///printf("name:??? %s\n", pic_filenames[3]);
        count_str[11] = i - 27 + '0';
        pic_filenames[i] = new char[17];
        strcpy(pic_filenames[i], count_str);
        //printf("name: %s\n", pic_filenames[i]);
    }

    ///printf("name: %s\n", pic_filenames[3]);

    pic_filenames[3] = "bins/face0.bin";

    pic_filenames[37] = "bins/score-percent.bin";

    ///for (i = 0; i < 38; i++) printf("%s\n", pic_filenames[i]);
    data = (unsigned char *) malloc(mem_size);
    memset(data, 0, mem_size);
    textHeader = data;
    int offset = generate_text_bin(osu_filenames, 2, textHeader);
    int len = offset;
    picHeader = &(data[offset]);

    offset = generate_pic_bin(pic_filenames, 38, picHeader);
    len += offset;
    printf("Total mem length: %d\n", len);


    printTo(data, mem_size, "memory.bin");
    return data;
}


void release_mem(unsigned char* data) {
    free(data);
}
/*int main() {
    unsigned char* memory = generate_mem_bin();
    release_mem(memory);
    //printf("%d\n", sizeof(char *));
    return 0;
}*/
