#ifndef taiko_h
#define taiko_h

#define MAX_LEN 100
#define MAX_HITS 2000

static const int targety=300;
static const int targetx=200;

static const int drumx=80;
static const int drumy=300;

static const int Z=122;
static const int X=120;
static const int C=99;
static const int V=118;


struct pin{
  enum STATE{
    pending,dead,accept
  };
  enum TYPE{
    note,drumroll,denden
  };
  int locationx,range;
  enum TYPE type;
  char color,size;//color:0蓝1红
  
  
  int locationy,r2;
  char zpoint,xpoint,cpoint,vpoint;
  enum STATE state;
};

#endif
