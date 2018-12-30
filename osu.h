#ifndef osu_h
#define osu_h

#include <gtk/gtk.h>
#include "taiko.h"


static const int MASKS[8]={1,2,4,8,16,32,64,128};
static const int width=800;
static const int height=600;

unsigned char* getKeyMem();

unsigned int getTime();

unsigned char* getbkg();

GdkPixbuf* getDrawer();



int convert(char * filename, char *outFilename, struct pin* data);

unsigned char* getMap();

int getHitCnt();

#endif
