#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>
#include <pthread.h>
#include "osu.h"
#include "taiko.h"
#include "sound/sound.h"
#include <sys/time.h>

unsigned char* bkg;

unsigned char* combo;

unsigned char* getCombo(){
  return combo;
}

unsigned char** numbers;

unsigned char** getNumbers(){
  return numbers;
}

unsigned char** faces;

unsigned char** getFaces(){
  return faces;
}

unsigned char *mapmem;

unsigned char* getMapMem(){
  return (unsigned char*)mapmem;
}

unsigned char* getbkg(){
  return bkg;
}

unsigned char* instructions;

unsigned char* getInstructions(){
  return instructions;
}

unsigned int getTime()
{
   struct timeval tv;
   gettimeofday(&tv,NULL);
   return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

void* taiko(void *arg);

unsigned char keys;

unsigned char* serial;

int serialL,serialR;

unsigned char getSerial(){
  unsigned char ret=serial[serialL];
  serialL++;
  if(serialL>15)serialL=0;
  return ret;
}

int getSerialNumber(){
  int ret=serialR-serialL;
  if(ret<0)ret+=16;
  //printf("%d %d\n",serialL,serialR);
  return ret;
}

void putSerial(unsigned char t){
  serial[serialR]=t;
  serialR++;
  //printf("wtfit?!!!%u\n",t);
  if(serialR>15)serialR=0;
}

int counter;



GdkPixbuf* todraw;

GdkPixbuf* getDrawer(){
  return todraw;
}

GtkWidget *window;
GtkWidget *drawing_area;

/* Surface to store current scribbles */
static cairo_surface_t *surface = NULL;

static void
clear_surface (void)
{
  cairo_t *cr;

  cr = cairo_create (surface);

  cairo_set_source_rgb (cr, 1, 1, 1);
  cairo_paint (cr);

  cairo_destroy (cr);
}


/* Create a new surface of the appropriate size to store our scribbles */
static gboolean
configure_event_cb (GtkWidget         *widget,
                    GdkEventConfigure *event,
                    gpointer           data)
{
  if (surface)
    cairo_surface_destroy (surface);
  surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
                                               CAIRO_CONTENT_COLOR,
                                               gtk_widget_get_allocated_width (widget),
                                               gtk_widget_get_allocated_height (widget));
  clear_surface();
  /* We've handled the configure event, no need for further processing. */
  return TRUE;
}

static gboolean
draw_cb (GtkWidget *widget,
         cairo_t   *cr,
         gpointer   data)
{
  cairo_set_source_surface (cr, surface, 0, 0);
  cairo_paint (cr);

  return FALSE;
}

gboolean deal_time( gpointer* label )
{
  cairo_t *cr = cairo_create (surface);
  gdk_cairo_set_source_pixbuf (cr, todraw, 0,0);
  cairo_paint (cr);
  cairo_destroy (cr);
  gtk_widget_queue_draw_area (drawing_area, 0,0,width,height);
  while(getGameSerialNumber()){
    unsigned char serial=getGameSerial();
    printf("%u\n",serial);
    if(serial==0)stopAll();
      else if((serial&3)==0)stop(serial/4);
      else if((serial&3)==1)sound(serial/4,0);
      else if((serial&3)==2)sound(serial/4,-1);
  }
  return TRUE;
}

void* stopSong(int song){
  keys|=MASKS[7];
  //printf("songid:%d\n",song);
  putSerial(keys);
  keys&=~MASKS[7];
}

gboolean time_counter(gpointer* label){
  counter++;
  return TRUE;
}

gboolean deal_key_press(GtkWidget *widget, GdkEventKey  *event, gpointer data)
{
  if (surface == NULL)
    return FALSE;
  int key = event->keyval; // 获取键盘键值类型
  switch(key){
    case Z:
      keys|=MASKS[0];
      putSerial(keys);
      break;
    case X:
      keys|=MASKS[1];
      putSerial(keys);
      break;
    case C:
      keys|=MASKS[2];
      putSerial(keys);
      break;
    case V:
      keys|=MASKS[3];
      putSerial(keys);
      break;
    case Q:
      keys|=MASKS[4];
      putSerial(keys);
      break;
    default:
      break;
  }
  //keys[key/8]|=MASKS[key%8];
  return TRUE;
}

gboolean deal_key_release(GtkWidget *widget, GdkEventKey  *event, gpointer data)
{
  if (surface == NULL)
    return FALSE;
  int key = event->keyval; // 获取键盘键值类型
  switch(key){
    case Z:
      keys&=~MASKS[0];
      putSerial(keys);
      break;
    case X:
      keys&=~MASKS[1];
      putSerial(keys);
      break;
    case C:
      keys&=~MASKS[2];
      putSerial(keys);
      break;
    case V:
      keys&=~MASKS[3];
      putSerial(keys);
      break;
    case Q:
      keys&=~MASKS[4];
      putSerial(keys);
      break;
    default:
      break;
  }
  //  keys[key/8]&=~MASKS[key%8];
  return TRUE;
}

static void
close_window (void)
{
  if (surface)
    cairo_surface_destroy (surface);
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
  GtkWidget *frame;

  window = gtk_application_window_new (app);
  gtk_window_set_title (GTK_WINDOW (window), "Drawing Area");

  g_signal_connect (window, "destroy", G_CALLBACK (close_window), NULL);

  gtk_container_set_border_width (GTK_CONTAINER (window), 8);

  frame = gtk_frame_new (NULL);
  gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
  gtk_container_add (GTK_CONTAINER (window), frame);

  drawing_area = gtk_drawing_area_new ();
  /* set a minimum size */
  gtk_widget_set_size_request (drawing_area, width,height);

  gtk_container_add (GTK_CONTAINER (frame), drawing_area);
  
  guint timer = g_timeout_add(30, (GSourceFunc)deal_time, NULL);
  
  /* Signals used to handle the backing surface */
  g_signal_connect (drawing_area, "draw",
                    G_CALLBACK (draw_cb), NULL);
  g_signal_connect (drawing_area,"configure-event",
                    G_CALLBACK (configure_event_cb), NULL);

  g_signal_connect(window, "key-press-event",
          G_CALLBACK(deal_key_press), NULL);
  
  g_signal_connect(window, "key-release-event",
          G_CALLBACK(deal_key_release), NULL);

  gtk_widget_set_events (drawing_area, gtk_widget_get_events (drawing_area)
                                     | GDK_KEY_PRESS_MASK);

  gtk_widget_show_all (window);
}

int main (int argc,char **argv){

  char ** filenames = (char **) malloc(sizeof(char *) * MAX_MAPS);
  filenames[0] = "fripSide - snow blind -after- (chaica) [Taiko Oni].osu";
  filenames[1] = "LiSA - crossing field (gaston_2199) [Kantan].osu";
  mapmem = generate_bin(filenames, 2);
  
  /*taikomap = (struct pin*) malloc(sizeof(struct pin) * MAX_HITS);
  hitcnt = convert("fripSide - snow blind -after- (chaica) [Taiko Oni].osu", NULL, taikomap);*/
  bkg=(unsigned char*)malloc(width*height*sizeof(char));
  combo=(unsigned char*)malloc(150*60*sizeof(char));
  numbers=(unsigned char**)malloc(31*sizeof(char*));
  numbers[30]=(unsigned char*)malloc(numberwidth*numberheight*sizeof(char));
  instructions=(unsigned char**)malloc(150*200*sizeof(char*));
  FILE *fp=fopen("bkg.bin","rb");
  fread(bkg,1,width*height*sizeof(char),fp);
  fclose(fp);
  fp=fopen("bins/combo.bin","rb");
  fread(combo,1,150*60*sizeof(char),fp);
  fclose(fp);
  fp=fopen("instructions.bin","rb");
  fread(instructions,1,150*200*sizeof(char),fp);
  fclose(fp);
  int i;
  for(i=0;i<10;i++){
    numbers[i]=(unsigned char*)malloc(numberwidth*numberheight*sizeof(char));
    char ss[17]="bins/score-x.bin\0";
    ss[11]=i+'0';
    FILE *fp=fopen(ss,"rb");
    fread(numbers[i],1,numberwidth*numberheight*sizeof(char),fp);
    fclose(fp);
  }
  for(i=0;i<10;i++){
    numbers[10+i]=(unsigned char*)malloc(numberwidth*numberheight*sizeof(char));
    char ss[17]="bins/combo-x.bin\0";
    ss[11]=i+'0';
    FILE *fp=fopen(ss,"rb");
    fread(numbers[i+10],1,numberwidth*numberheight*sizeof(char),fp);
    fclose(fp);
  }
  for(i=0;i<10;i++){
    numbers[20+i]=(unsigned char*)malloc(20*30*sizeof(char));
    char ss[17]="bins/count-x.bin\0";
    ss[11]=i+'0';
    FILE *fp=fopen(ss,"rb");
    fread(numbers[i+20],1,20*30*sizeof(char),fp);
    fclose(fp);
  }
  fp=fopen("bins/score-percent.bin","rb");
  fread(numbers[30],1,numberwidth*numberheight*sizeof(char),fp);
  fclose(fp);
  
  faces=(unsigned char**)malloc(4*sizeof(char*));
  for(i=0;i<2;i++){
    faces[i]=(unsigned char*)malloc(70*70*sizeof(char));
    char ss[15]="bins/facex.bin\0";
    ss[9]=i+'0';
    FILE *fp=fopen(ss,"rb");
    fread(faces[i],1,70*70*sizeof(char),fp);
    fclose(fp);
  }
  for(i=2;i<4;i++){
    faces[i]=(unsigned char*)malloc(90*90*sizeof(char));
    char ss[15]="bins/facex.bin\0";
    ss[9]=i+'0';
    FILE *fp=fopen(ss,"rb");
    fread(faces[i],1,90*90*sizeof(char),fp);
    fclose(fp);
  }
  initSoundPlayer(stopSong);
  keys=(unsigned char *)malloc(sizeof(char));
  serial=(unsigned char *)malloc(16*sizeof(char));
  serialL=0;
  serialR=0;
  todraw=gdk_pixbuf_new (GDK_COLORSPACE_RGB,FALSE,8,width,height);
  int pd;
  pthread_t tid;
  pd = pthread_create(&tid,NULL,taiko,NULL);
  keys=0;

  GtkApplication *app;
  int status;

  app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);
  closeSoundPlayer();

  return status;
}
