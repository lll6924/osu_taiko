#include "osu.h"
#include "taiko.h"
#include <stdio.h>

const int maxspeed=50;
const int song=0;
int hitcnt;

unsigned char* keymem;

unsigned char* buffer;

unsigned char* comboX;

unsigned char* bkg;

unsigned char** faces;

unsigned char** numbers;

unsigned char* mapmem;

unsigned char* soundStatus;

//unsigned char* VGA;

struct pin* pins;

int Zdown,Xdown,Cdown,Vdown,lastZdown,lastXdown,lastCdown,lastVdown;

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
  pins=(struct pin*)getMap(song,mapmem);
  hitcnt=getHitCount(song,mapmem);
  int i;
  for(i=0;i<hitcnt;i++){
    pins[i].combo=(i%3==0);
    pins[i].locationx=4*pins[i].locationx+targetx;
    pins[i].type=note;
    pins[i].locationy=targety;
    /*if(pins[i].size==1)*/pins[i].r2=2500;
    //else pins[i].r2=35*35;
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
  //for(i=1;i<stack[0];i++)
    //VGA[stack[i]]=buffer[stack[i]];
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

int stack[width*height+1];


void drawgraph(int x,int y,int Width,int Height,unsigned char* source){
  int j,k;
  for(j=0;j<Width;j++)
    for(k=0;k<Height;k++){
      int X=j+x,Y=k+y;
      if(X<0||Y<0||X>=width||Y>=height){
        continue;
      }
      if(source[k*Width+j]!=0){
        buffer[(k+y)*width+j+x]=source[k*Width+j];
        stack[++stack[0]]=(k+y)*width+j+x;
      }
    }
}

void InputUpdate(){
  Zdown=keydown(0);
  Xdown=keydown(1);
  Cdown=keydown(2);
  Vdown=keydown(3);
}

unsigned char drumboard[121*121];

void* taiko(void *arg){
  buffer=(unsigned char *)malloc(width*height * sizeof(unsigned char));
  keymem=getKeyMem();
  bkg=getbkg();
  comboX=getCombo();
  numbers=getNumbers();
  faces=getFaces();
  mapmem=getMapMem();
  //VGA=getVGA();
  soundStatus=getSoundStatus();
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
  int score=0;
  int diescore=0;
  int count=0;
  int combo=0;
  unsigned int showcombo=0;
  int thecombo=0;
  int state=0;
  for(int i=-60;i<=60;i++)
    for(int j=-60;j<=60;j++){
      int d,x,y;
      x=i+60;
      y=j+60;
      drumboard[x+y*121]=0;
      d=dist(i,j,0,0);
      if(d<50*50){
        if(i==0)drumboard[y*121+x]=(unsigned char)(219);
        else if(i<0)drumboard[y*121+x]=2;
        else drumboard[y*121+x]=3;
      }else if(d<52*52){
        drumboard[y*121+x]=(unsigned char)(219);
      }else if(d<60*60){
        if(i==0)drumboard[y*121+x]=(unsigned char)(219);
        else if(i<0)drumboard[y*121+x]=1;
        else drumboard[y*121+x]=4;
      }
    }
  while(1){
    nowtime=getTime();
    while(nowtime-lasttime<maxspeed){
      nowtime=getTime();
    }
    lasttime=nowtime;
    InputUpdate();
    if((Zdown&&lastZdown==0)||(Xdown&&lastXdown==0)||(Cdown&&lastCdown==0)||(Vdown&&lastVdown==0)){
      soundStatus[3]=1;
    }
    printf("%u\n",nowtime);
    if(state==0){
      if(Zdown==1)state++;
    }if(state==1){
      soundStatus[song]=1;
      gametime+=20;
      stack[0]=0;
      //drawgraph(0,0,width,height,bkg);
      /*for(i=0;i<width;i++)
        for(j=220;j<380;j++)
          if(i<140)buffer[j*width+i]=(unsigned char)(219);
          else buffer[j*width+i]=(unsigned char)(146);*/
      for(int i=-60;i<60;i++)
        for(int j=-60;j<60;j++){
          int x,y,d;
          x=targetx+i;
          y=targety+j;
          //d=dist(x,y,targetx,targety);
          buffer[y*width+x]=(unsigned char)(73);
        }
      for(int i=-60;i<=60;i++)
        for(int j=-60;j<=60;j++){
          int x,y,d,X,Y;
          x=drumx+i;
          y=drumy+j;
          X=i+60;
          Y=j+60;
          unsigned char todraw=drumboard[Y*121+X];
          if(todraw==0)continue;
          buffer[y*width+x]=todraw;
          if(todraw<5){
            buffer[y*width+x]=(unsigned char)(140);
            if(todraw==1&&Zdown)buffer[y*width+x]=(unsigned char)(39);
            if(todraw==2&&Xdown)buffer[y*width+x]=(unsigned char)(229);
            if(todraw==3&&Cdown)buffer[y*width+x]=(unsigned char)(229);
            if(todraw==4&&Vdown)buffer[y*width+x]=(unsigned char)(39);
          }
        }
      //printf("%d\n",begin);
      for(i=begin;i<hitcnt;i++){
        if(pins[i].state==accept){
          pins[i].locationy-=10;
          pins[i].locationx+=25;
          pins[i].r2-=200;
          if(pins[i].r2<0)pins[i].state=trulydead;
        }else if(pins[i].state==dead){
          //printf("die\n");
          pins[i].locationy+=10;
          pins[i].locationx+=20;
          pins[i].r2-=200;
          if(pins[i].r2<0)pins[i].state=trulydead;
        }else if(pins[i].locationx<gametime-100||pins[i].locationy<-100||pins[i].locationy>=height+100){
          pins[i].state=trulydead;
        }else if(pins[i].locationx<gametime+150){
          count=0;
          showcombo=0;
          combo=0;
          pins[i].state=dead;
          diescore+=100;
        }
        if(pins[i].state==trulydead){
          begin=i+1;
          continue;
        }
        if(pins[i].locationx-gametime>width+100)break;
        if(pins[i].locationy>-100&&pins[i].locationy<height+100){
          if(pins[i].state==pending&&pins[i].locationx-gametime>targetx-pins[i].range&&pins[i].locationx-gametime<targetx+pins[i].range){
            if(pins[i].type==note){
              if((Zdown==0&&Vdown==0)||(Xdown==0&&Cdown==0)){
                int toaccept=0;
                int todie=0;
                if(pins[i].zpoint==0){
                  if(Zdown==0){
                    pins[i].zpoint=1;
                  }
                }else if(pins[i].zpoint==1&&Zdown==1){
                  pins[i].zpoint++;
                  if(pins[i].color==0){
                    toaccept++;
                  }else{
                    todie++;
                  }
                }
                if(pins[i].xpoint==0){
                  if(Xdown==0){
                    pins[i].xpoint=1;
                  }
                }else if(pins[i].xpoint==1&&Xdown==1){
                  pins[i].xpoint++;
                  if(pins[i].color==1){
                    toaccept++;
                  }else{
                    todie++;
                  }
                }
                if(pins[i].cpoint==0){
                  if(Cdown==0){
                    pins[i].cpoint=1;
                  }
                }else if(pins[i].cpoint==1&&Cdown==1){
                  pins[i].cpoint++;
                  if(pins[i].color==1){
                    toaccept++;
                  }else{
                    todie++;
                  }
                }
                if(pins[i].vpoint==0){
                  if(Vdown==0){
                    pins[i].vpoint=1;
                  }
                }else if(pins[i].vpoint==1&&Vdown==1){
                  pins[i].vpoint++;
                  if(pins[i].color==0){
                    toaccept++;
                  }else{
                    todie++;
                  }
                }
                if(todie){
                  pins[i].state=dead;
                  count=0;
                  combo=0;
                  diescore+=100;
                }
                /*toaccept=1;
                if(pins[i].size==1)toaccept=2;*/
                while(toaccept){
                  toaccept--;
                  showcombo=0;
                  if(pins[i].size==1&&pins[i].state==accept)score+=100;
                  if(pins[i].state!=accept){
                    score+=100;
                    count++;
                    if(combo!=0)combo++;
                    if(pins[i].combo!=0)combo=1;
                    if(i==hitcnt-1||pins[i+1].combo!=0){
                      if(combo!=0){
                        score+=300;
                        showcombo=nowtime;
                        thecombo=combo;
                      }else diescore+=300;
                    }
                  }
                  pins[i].state=accept;
                }
              }
            }
          }
          int r=pins[i].size*10+35;
          drawgraph(pins[i].locationx-gametime-r,pins[i].locationy-r,r*2,r*2,faces[pins[i].size*2+pins[i].color]);
        }
      }
      //printf("%d\n",score);
      int toprint=score;
      for(i=0;i<8;i++){
        drawgraph(745-i*45,10,numberwidth,numberheight,numbers[toprint%10]);
        toprint/=10;
      }
      toprint=count;
      for(i=0;i<4;i++){
        drawgraph(90-i*20,285,20,30,numbers[toprint%10+20]);
        toprint/=10;
      }
      if(nowtime-showcombo<500){
        thecombo%=100;
        int x=180;
        if(thecombo>=10){
          drawgraph(x,150,numberwidth,numberheight,numbers[thecombo/10+10]);
          x+=numberwidth;
        }
        drawgraph(x,150,numberwidth,numberheight,numbers[thecombo%10+10]);
        drawgraph(30,150,150,60,comboX);
      }
      if(score+diescore==0)toprint=100;
      else toprint=score*100/(score+diescore);
      i=0;
      while(toprint){
        drawgraph(700-i*45,80,numberwidth,numberheight,numbers[toprint%10]);
        toprint/=10;
        i++;
      }
      drawgraph(745,80,numberwidth,numberheight,numbers[30]);
      copyToGraph(buffer);
      for(i=1;i<=stack[0];i++)
        buffer[stack[i]]=bkg[stack[i]];
    }
    lastZdown=Zdown;
    lastXdown=Xdown;
    lastCdown=Cdown;
    lastVdown=Vdown;
  }
}
