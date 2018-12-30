#ifndef taiko_h
#define taiko_h

static const int targety=300;
static const int targetx=100;

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
  char zeropoint;
  enum STATE state;
};

#endif
