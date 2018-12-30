#include "osu.h"
#include "taiko.h"
#include <stdio.h>

const int maxspeed=5;
int hitcnt;

unsigned char* keymem;

unsigned char* buffer;

unsigned char* bkg;

unsigned char** numbers;

unsigned char* mapmem;

struct pin* pins;

int Zdown,Xdown,Cdown,Vdown;

int getHitCount(int id, unsigned char* mem) {
    if (id < 0 || id >= MAX_MAPS) return -1;
    struct beatMap* maps = (struct beatMap*) mem;
    return maps[id].len;
}

unsigned char* getMap(int id, unsigned char* mem) {
    if (id < 0 || id >= MAX_MAPS) return NULL;
    struct beatMap* maps = (struct beatMap*) mem;
    struct pin *data = (struct pin*) &(mem[maps[id].offset]);
    return (unsigned char*)data;
};


void loadpins(){
  pins=(struct pin*)getMap(1,mapmem);
  hitcnt=getHitCount(1,mapmem);
  int i;
  for(i=0;i<hitcnt;i++){
    pins[i].locationx*=3;
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
  return ((*keymem&MASKS[val])!=0);
}

void InputUpdate(){
  Zdown=keydown(0);
  Xdown=keydown(1);
  Cdown=keydown(2);
  Vdown=keydown(3);
}

void* taiko(void *arg){
  buffer=(unsigned char *)malloc(width*height * sizeof(unsigned char));
  keymem=getKeyMem();
  bkg=getbkg();
  numbers=getNumbers();
  mapmem=getMapMem();
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
    InputUpdate();
    //printf("%u\n",nowtime);
    if(state==0){
      if(Zdown==1)state++;
    }if(state==1){
      
      gametime+=4;
      for(i=0;i<width;i++)
        for(j=0;j<height;j++)
          buffer[j*width+i]=bkg[j*width+i];
      for(i=0;i<width;i++)
        for(j=220;j<380;j++)
          if(i<140)buffer[j*width+i]=(unsigned char)(219);
          else buffer[j*width+i]=(unsigned char)(146);
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
            if(x==drumx)buffer[y*width+x]=(unsigned char)(219);
            else if(x<drumx){
              if(Xdown)buffer[y*width+x]=(unsigned char)(229);
              else buffer[y*width+x]=(unsigned char)(140);
            }else{
              if(Cdown)buffer[y*width+x]=(unsigned char)(229);
              else buffer[y*width+x]=(unsigned char)(140);
            }
          }else if(d<52*52){
            buffer[y*width+x]=(unsigned char)(219);
          }else if(d<60*60){
            if(x==drumx)buffer[y*width+x]=(unsigned char)(219);
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
          pins[i].locationx+=5;
          pins[i].r2-=20;
          if(pins[i].r2<0)pins[i].state=trulydead;
        }else if(pins[i].state==dead){
          pins[i].locationy+=2;
          pins[i].locationx+=4;
          pins[i].r2-=20;
          if(pins[i].r2<0)pins[i].state=trulydead;
        }else if(pins[i].locationx<gametime-100||pins[i].locationy<-100||pins[i].locationy>=height+100){
          pins[i].state=trulydead;
        }else if(pins[i].locationx<gametime+150){
          pins[i].state=dead;
        }
        if(pins[i].state==trulydead){
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
                  }else pins[i].state=dead;
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
                  }else pins[i].state=dead;
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
                  }else pins[i].state=dead;
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
                  }else pins[i].state=dead;
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
                  buffer[y*width+x]=(unsigned char)(216);
                }else if(pins[i].state==dead){
                  buffer[y*width+x]=(unsigned char)(37);
                }else if(pins[i].color==1)buffer[y*width+x]=(unsigned char)(233);
                else buffer[y*width+x]=(unsigned char)(71);
              }
            }
        }
      }
      //printf("%d\n",score);
      int toprint=score;
      for(i=0;i<8;i++){
        int x=745-i*45,y=10,place=toprint%10;
        for(j=0;j<numberwidth;j++)
          for(k=0;k<numberheight;k++)
            buffer[(k+y)*width+j+x]=numbers[place][k*numberwidth+j];
        toprint/=10;
      }
      copyToGraph(buffer);
    }
  }
}
