#include "osu.h"
#include "taiko.h"
#include <stdio.h>

const int maxspeed=5;

unsigned char* keymem;

unsigned char* buffer;

unsigned char* bkg;

void loadpins(struct pin* pins){
  pins[0].locationx=10;
  pins[1].locationx=400;
  pins[2].locationx=600;
  pins[3].locationx=1200;
  pins[4].locationx=1600;
  pins[5].locationx=2000;
  pins[6].locationx=2800;
  pins[7].locationx=3000;
  pins[8].locationx=3600;
  pins[9].locationx=4000;
  pins[10].locationx=4400;
  int i;
  for(i=1;i<=pins[0].locationx;i++){
    pins[i].locationy=targety;
    pins[i].range=20;
    pins[i].r2=6400;
    pins[i].state=pending;
  }
}

int dist(int x1,int y1,int x2,int y2){
  return (x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);
}

void copyToGraph(unsigned char* source){
  GdkPixbuf* todraw;
  todraw=getDrawer();
  guchar* pixels;
  guchar* p;
  pixels = gdk_pixbuf_get_pixels (todraw);
  int n_channels,rowstride;
  n_channels = gdk_pixbuf_get_n_channels (todraw);
  rowstride = gdk_pixbuf_get_rowstride (todraw);
  int i,j;
  for(i=0;i<width;i++)
    for(j=0;j<height;j++){
      p = pixels + j * rowstride + i * n_channels;
      p[0]=source[j*width+i]/32*32;
      p[1]=source[j*width+i]%32/4*32;
      p[2]=source[j*width+i]%4*64;
    }
}

int keydown(int val){
  if((keymem[val/8]&MASKS[val%8])>0)return 1;
  return 0;
}

void* taiko(void *arg){
  buffer=(unsigned char *)malloc(width*height * sizeof(unsigned char));
  keymem=getKeyMem();
  bkg=getbkg();
  int i,j,k;
  unsigned int lasttime,nowtime;
  lasttime=getTime();
  struct pin* pins;
  pins=(struct pin *)malloc(128 * sizeof(struct pin));
  loadpins(pins);
  int begin,gametime;
  begin=1;
  gametime=0;
  for(i=0;i<width;i++)
    for(j=0;j<height;j++)
      buffer[j*width+i]=bkg[j*width+i];
  while(1){
    nowtime=getTime();
    while(nowtime-lasttime<maxspeed){
      nowtime=getTime();
    }
    lasttime=nowtime;
    printf("%u\n",nowtime);
    gametime+=5;
    for(int i=-100;i<100;i++)
      for(int j=-100;j<100;j++){
        int x,y,d;
        x=targetx+i;
        y=targety+j;
        d=dist(x,y,targetx,targety);
        if(d>=80*80&&d<90*90){
          if(keydown(122)==1)buffer[y*width+x]=(unsigned char)(234);
          else buffer[y*width+x]=(unsigned char)(200);
        }
      }
    for(i=begin;i<=pins[0].locationx;i++){
      if(pins[i].state==accept){
        pins[i].locationy-=2;
        pins[i].r2-=50;
        if(pins[i].r2<0)pins[i].state=dead;
      }else if(pins[i].locationx<gametime-100){
        pins[i].state=dead;
      }
      if(pins[i].state==dead){
        begin=i+1;
        continue;
      }
      if(pins[i].locationx-gametime>width+100)break;
      if(pins[i].locationy>-100&&pins[i].locationy<height+100){
        if(pins[i].state==pending&&pins[i].locationx-gametime>targetx-pins[i].range/2&&pins[i].locationx-gametime<targetx+pins[i].range/2){
          keydown(122)==1
          pins[i].state=accept;
        }
        for(j=-100;j<100;j++)
          for(k=-100;k<100;k++){
            int x,y;
            x=pins[i].locationx-gametime+j;
            y=pins[i].locationy+k;
            if(x>=0&&y>=0&&x<width&&y<height&&dist(x,y,pins[i].locationx-gametime,pins[i].locationy)<pins[i].r2){
              if(pins[i].state==accept){
                buffer[y*width+x]=(unsigned char)(234);
              }else
                buffer[y*width+x]=(unsigned char)(123);
            }
          }
      }
    }
    copyToGraph(buffer);
    for(i=begin;i<=pins[0].locationx;i++){
      if(pins[i].locationx-gametime>width+100)break;
      if(pins[i].locationy>-100&&pins[i].locationy<height+100){
        for(j=-100;j<100;j++)
          for(k=-100;k<100;k++){
            int x,y;
            x=pins[i].locationx-gametime+j;
            y=pins[i].locationy+k;
            if(x>=0&&y>=0&&x<width&&y<height&&dist(x,y,pins[i].locationx-gametime,pins[i].locationy)<pins[i].r2){
              buffer[y*width+x]=bkg[y*width+x];
            }
          }
      }
    }
  }
}
