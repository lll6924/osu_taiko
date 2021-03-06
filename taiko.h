#ifndef taiko_h
#define taiko_h

#define MAX_LEN 100
#define MAX_HITS 2000
#define MAX_MAPS 10

static const int songs=2;

static const int targety=300;
static const int targetx=220;

static const int drumx=70;
static const int drumy=300;

static const int Z=122;
static const int X=120;
static const int C=99;
static const int V=118;
static const int Q=113;


struct pin{
  enum STATE{
    pending,dead,accept,trulydead
  };
  enum TYPE{
    note,drumroll,denden
  };
  int locationx,range;
  enum TYPE type;
  char color,size,combo;//color:0蓝1红


  int locationy,r2,Locationx;
  char zpoint,xpoint,cpoint,vpoint;
  enum STATE state;
};

struct beatMap{
    int offset;
    int len;
};
#endif
