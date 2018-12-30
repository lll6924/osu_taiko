#include "osu.h"
#include "taiko.h"
#include <stdio.h>

const int maxspeed=5;
int hitcnt;

unsigned char* keymem;

unsigned char* buffer;

unsigned char* bkg;

struct pin* pins;


void loadpins(){
  pins=getMap();
  hitcnt=getHitCnt();
  int i;
  for(i=0;i<hitcnt;i++){
    pins[i].locationx*=2;
    pins[i].type=note;
    pins[i].locationy=targety;
    if(pins[i].size==1)pins[i].r2=45*45;
    else pins[i].r2=35*35;
    pins[i].state=pending;
    pins[i].zpoint=0;
    pins[i].xpoint=0;
    pins[i].cpoint=0;
    pins[i].vpoint=0;
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
  //pins=(struct pin *)malloc(128 * sizeof(struct pin));
  loadpins();
  int begin,gametime;
  begin=0;
  gametime=0;
  for(i=0;i<width;i++)
    for(j=0;j<height;j++)
      buffer[j*width+i]=bkg[j*width+i];
  copyToGraph(buffer);
  int score;
  score=0;
  int state=0;
  while(1){
    nowtime=getTime();
    while(nowtime-lasttime<maxspeed){
      nowtime=getTime();
    }
    lasttime=nowtime;
    //printf("%u\n",nowtime);
    int Zdown,Xdown,Cdown,Vdown;
    Zdown=keydown(Z);
    Xdown=keydown(X);
    Cdown=keydown(C);
    Vdown=keydown(V);
    if(state==0){
      if(Zdown==1)state++;
    }if(state==1){
      gametime+=2;
      for(i=0;i<width;i++)
        for(j=200;j<400;j++)
          buffer[j*width+i]=(unsigned char)(146);
      for(int i=-100;i<100;i++)
        for(int j=-100;j<100;j++){
          int x,y,d;
          x=targetx+i;
          y=targety+j;
          d=dist(x,y,targetx,targety);
          if(d<40*40)buffer[y*width+x]=(unsigned char)(73);
        }
      for(int i=-100;i<100;i++)
        for(int j=-100;j<100;j++){
          int x,y,d;
          x=drumx+i;
          y=drumy+j;
          d=dist(x,y,drumx,drumy);
          if(d<50*50){
            if(x==drumx)buffer[y*width+x]=(unsigned char)(X);
            else if(x<drumx){
              if(Xdown)buffer[y*width+x]=(unsigned char)(229);
              else buffer[y*width+x]=(unsigned char)(140);
            }else{
              if(Cdown)buffer[y*width+x]=(unsigned char)(229);
              else buffer[y*width+x]=(unsigned char)(140);
            }
          }else if(d<52*52){
            buffer[y*width+x]=(unsigned char)(X);
          }else if(d<60*60){
            if(x==drumx)buffer[y*width+x]=(unsigned char)(X);
            else if(x<drumx){
              if(Zdown)buffer[y*width+x]=(unsigned char)(39);
              else buffer[y*width+x]=(unsigned char)(140);
            }else{
              if(Vdown)buffer[y*width+x]=(unsigned char)(39);
              else buffer[y*width+x]=(unsigned char)(140);
            }
          }
        }
      for(i=begin;i<hitcnt;i++){
        if(pins[i].state==accept){
          pins[i].locationy-=2;
          pins[i].locationx+=4;
          pins[i].r2-=20;
          if(pins[i].r2<0)pins[i].state=dead;
        }else if(pins[i].locationx<gametime+150){
          pins[i].state=dead;
        }
        if(pins[i].state==dead){
          begin=i+1;
          continue;
        }
        if(pins[i].locationx-gametime>width+100)break;
        if(pins[i].locationy>-100&&pins[i].locationy<height+100){
          if(pins[i].state==pending&&pins[i].locationx-gametime>targetx-pins[i].range/2&&pins[i].locationx-gametime<targetx+pins[i].range/2){
            if(pins[i].type==note){
              if((Zdown==0&&Vdown==0)||(Xdown==0&&Cdown==0)){
                if(pins[i].zpoint==0){
                  if(Zdown==0){
                    pins[i].zpoint=1;
                  }
                }else if(pins[i].zpoint==1&&Zdown==1){
                  pins[i].zpoint++;
                  if(pins[i].color==0){
                    if(pins[i].size==1&&pins[i].state==accept)score+=100;
                    if(pins[i].state!=accept)score+=100;
                    pins[i].state=accept;
                  }
                }
                if(pins[i].xpoint==0){
                  if(Xdown==0){
                    pins[i].xpoint=1;
                  }
                }else if(pins[i].xpoint==1&&Xdown==1){
                  pins[i].xpoint++;
                  if(pins[i].color==1){
                    if(pins[i].size==1&&pins[i].state==accept)score+=100;
                    if(pins[i].state!=accept)score+=100;
                    pins[i].state=accept;
                  }
                }
                if(pins[i].cpoint==0){
                  if(Cdown==0){
                    pins[i].cpoint=1;
                  }
                }else if(pins[i].cpoint==1&&Cdown==1){
                  pins[i].cpoint++;
                  if(pins[i].color==1){
                    if(pins[i].size==1&&pins[i].state==accept)score+=100;
                    if(pins[i].state!=accept)score+=100;
                    pins[i].state=accept;
                  }
                }
                if(pins[i].vpoint==0){
                  if(Vdown==0){
                    pins[i].vpoint=1;
                  }
                }else if(pins[i].vpoint==1&&Vdown==1){
                  pins[i].vpoint++;
                  if(pins[i].color==0){
                    if(pins[i].size==1&&pins[i].state==accept)score+=100;
                    if(pins[i].state!=accept)score+=100;
                    pins[i].state=accept;
                  }
                }
              }
            }
          }
          for(j=-100;j<100;j++)
            for(k=-100;k<100;k++){
              int x,y;
              x=pins[i].locationx-gametime+j;
              y=pins[i].locationy+k;
              if(x>=0&&y>=0&&x<width&&y<height&&dist(x,y,pins[i].locationx-gametime,pins[i].locationy)<pins[i].r2){
                if(pins[i].state==accept){
                  buffer[y*width+x]=(unsigned char)(37);
                }else if(pins[i].color==1)buffer[y*width+x]=(unsigned char)(233);
                else buffer[y*width+x]=(unsigned char)(71);
              }
            }
        }
      }
      //printf("%d\n",score);
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
}
