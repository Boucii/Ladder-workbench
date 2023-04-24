#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0, canvas_w=0 ,canvas_h=0;

 int gettimeofday(struct timeval *tv, struct timezone *tz);
uint32_t NDL_GetTicks() {
  struct timeval a;
  //_gettimeofday(&a,NULL);
  gettimeofday(&a,NULL);
  return a.tv_sec*1000+a.tv_usec/1000;
}

int NDL_PollEvent(char *buf, int len) {
  int fd=open("/dev/events", O_RDONLY);
  int size=0;
  size=read(fd,buf,len);
  close(fd);

  return size;
}

void NDL_OpenCanvas(int *w, int *h) {
  if (getenv("NWM_APP")) {
    int fbctl = 4;
    fbdev = 5;
    screen_w = *w; screen_h = *h;
    char buf[64];
    int len = sprintf(buf, "%d %d", screen_w, screen_h);
    // let NWM resize the window and create the frame buffer
    write(fbctl, buf, len);
    while (1) {
      // 3 = evtdev
      int nread = read(3, buf, sizeof(buf) - 1);
      if (nread <= 0) continue;
      buf[nread] = '\0';
      if (strcmp(buf, "mmap ok") == 0) break;
    }
    close(fbctl);
  }else{
	  /*
      FILE *fp=fopen("/proc/dispinfo","r+");
      assert(fp);
      fseek(fp,0,SEEK_SET);
      //fetch screen size
      fscanf(fp,"WIDTH:%d\nHEIGHT:%d",&screen_w,&screen_h);
      */
      char buf[50];
      int fd = open("/proc/dispinfo", O_RDONLY);
      read(fd,buf,50);
      sscanf(buf,"WIDTH:%d\nHEIGHT:%d",&screen_w,&screen_h);

      if(*w==0) {canvas_w=screen_w;*w=canvas_w;}
      if(*h==0) {canvas_h=screen_h;*h=canvas_h;}//?
      canvas_w=*w;canvas_h=*h;
  }
}
int open(const char *path, int flags, ...);
void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
  int fd = open("/dev/fb", O_WRONLY);
  x += (screen_w - canvas_w) / 2;
  y += (screen_h - canvas_h) / 2;

  for (int i = 0; i < h; ++ i) {
    lseek(fd, ((y + i) * screen_w + x) * sizeof(uint32_t), SEEK_SET);
    write(fd, pixels + i * w, w * sizeof(uint32_t));
  }
}
/*
void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h) {
	//assert(x+w<=canvas_w);
	//assert(y+h<=canvas_h);
	
	FILE *fp=fopen("/dev/fb","w");
	fseek(fp,0,SEEK_SET);
	uint32_t begin=(y*screen_w+x);
	for(int i=0;i<h;i++){
	    fwrite((void*)((i*w*4)+(intptr_t)pixels),4*w,1,fp);
	    fseek(fp,((y+i)*screen_w+x)*4,SEEK_SET);
	    begin+=screen_w;
	}
}
*/
void NDL_OpenAudio(int freq, int channels, int samples) {
}

void NDL_CloseAudio() {
}

int NDL_PlayAudio(void *buf, int len) {
  return 0;
}

int NDL_QueryAudio() {
  return 0;
}

int NDL_Init(uint32_t flags) {
  if (getenv("NWM_APP")) {
    evtdev = 3;
  }


  return 0;
}

void NDL_Quit() {
}
