#include <NDL.h>
#include <SDL.h>
#include <assert.h>
#include <string.h>

#define keyname(k) #k,


static uint8_t keystate[83]={0}; //this might not work cause the async read of poll/wait events
static const char *keyname[] = {
  "NONE",
  _KEYS(keyname)
};

int SDL_PushEvent(SDL_Event *ev) {
	assert(0);
  return 0;
}

int SDL_PollEvent(SDL_Event *ev) {
    char buf[64];
    buf[0]='\0';
    int size=NDL_PollEvent(buf,64);

    if(buf[0]=='k'&&(buf[1]=='d'||buf[1]=='u')){
      if(buf[1]=='d')
        ev->type = SDL_KEYDOWN;
      else if(buf[1]=='u'){
        ev->type = SDL_KEYUP;
      }
    
	  char temp[50];
      for(int i=0;i<256;i++){
	      strcpy(temp,keyname[i]);
	      strcat(temp,"\n");
        if(strcmp(buf+3,temp)==0){
	  keystate[i]=(buf[1]=='u'?0:1);
          ev->key.keysym.sym = i;
          break;
        }
      }
      return 1;
    }
  return 0;
}
int SDL_WaitEvent(SDL_Event *event) {
  while(1){
    char buf[64];
    buf[0]='\0';
    NDL_PollEvent(buf,64);
    if(buf[0]=='k'&&(buf[1]=='d'||buf[1]=='u')){
      if(buf[1]=='d')
        event->type = SDL_KEYDOWN;
      else if(buf[1]=='u'){
        event->type = SDL_KEYUP;
      }
    
	  char temp[50];
      for(int i=0;i<256;i++){
	      strcpy(temp,keyname[i]);
	      strcat(temp,"\n");
        if(strcmp(buf+3,temp)==0){
	  keystate[i]=(buf[1]=='u'?0:1);
          event->key.keysym.sym = i;
          break;
        }
      }
      break;
    }
  }
  return 1;
}
int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask) {
  assert(0);
  return 0;
}

uint8_t* SDL_GetKeyState(int *numkeys) {
  return keystate;
  return NULL;
}
