#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
# define MULTIPROGRAM_YIELD() yield()
#else
# define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) \
  [AM_KEY_##key] = #key,

#define KEYDOWN_MASK         0x8000
#define DEVICE_BASE          0xa0000000
#define KBD_ADDR             (DEVICE_BASE + 0x0000060) 
#define VGACTL_ADDR          0xA0000100
#define FB_ADDR              0xA1000000
#define SYNC_ADDR            (VGACTL_ADDR+4)
static const char *keyname[256] __attribute__((used)) = {
  [AM_KEY_NONE] = "NONE",
  AM_KEYS(NAME)
};

size_t serial_write(const void *buf, size_t offset, size_t len) {
	int i=0;
	for(;i<len;i++){
	  putch(((char*)buf)[i]);
	}
	return i;
  return 0;
}

size_t events_read(void *buf, size_t offset, size_t len) {
   uint32_t *key=(uint32_t*)(KBD_ADDR);
   ((char*)buf)[0]='\0';
  int written=0;
  if(*key){
    if(*key&KEYDOWN_MASK){
      written+=sprintf((char*)buf,"kd %s\n",keyname[*key&~KEYDOWN_MASK]);
    }else{
      written+=sprintf((char*)buf,"ku %s\n",keyname[*key&~KEYDOWN_MASK]);
    }
  }
  return len;
  return written;

	/*
  uint32_t *key=(uintptr_t)(KBD_ADDR);
  int written=0;
  while(*key&&written+4+sizeof(keyname[*key&~KEYDOWN_MASK])<=len){
    if(*key&KEYDOWN_MASK){
      written+=sprintf((char*)buf,"kd %s\n",keyname[*key&~KEYDOWN_MASK]);
    }else{
      written+=sprintf((char*)buf,"ku %s\n",keyname[*key&~KEYDOWN_MASK]);
    }
  }
  return written;
  */
}

size_t dispinfo_read(void *buf, size_t offset, size_t len) { 
  uint32_t *vgaptr=(uint32_t*)(uintptr_t)VGACTL_ADDR;
  sprintf(buf,"WIDTH:%d \nHEIGHT:%d",*vgaptr>>16,*(uint32_t*)(uintptr_t)VGACTL_ADDR&0xFFFF);
  return len;	
}

size_t fb_write(const void *buf, size_t offset, size_t len) {
  memcpy((void*)FB_ADDR+offset,buf,len);
  *(uint32_t*)SYNC_ADDR=1;
  return len;
}

void init_device() {
  Log("Initializing devices...");
  ioe_init();
}
