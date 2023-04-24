#include <am.h>
#include <stdio.h>

#define VGACTL_ADDR 0xa0000100
#define FB_ADDR     0xa1000000

static inline uint8_t  inb(uintptr_t addr) { return *(volatile uint8_t  *)addr; }
static inline uint16_t inw(uintptr_t addr) { return *(volatile uint16_t *)addr; }
static inline uint32_t inl(uintptr_t addr) { return *(volatile uint32_t *)addr; }

static inline void outb(uintptr_t addr, uint8_t  data) { *(volatile uint8_t  *)addr = data; }
static inline void outw(uintptr_t addr, uint16_t data) { *(volatile uint16_t *)addr = data; }
static inline void outl(uintptr_t addr, uint32_t data) { *(volatile uint32_t *)addr = data; }
static inline void outd(uintptr_t addr, uint64_t data) { *(volatile uint64_t *)addr = data; }

#define SYNC_ADDR (VGACTL_ADDR + 4)
#define W   inw(VGACTL_ADDR+2)
#define H   inw(VGACTL_ADDR) 
void __am_gpu_init() {
	/*
  int i;
  int w = W;
  int h = H;
  uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
  for (i = 0; i < w * h; i ++) fb[i] = i;
  outl(SYNC_ADDR, 1);
  */
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg) {
  *cfg = (AM_GPU_CONFIG_T) {
    .present = true, .has_accel = false,
    .width = W, .height = H,
    .vmemsz = W*H*32
  };
}
/*
void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {

	int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
	uint32_t *pixels = ctl->pixels;
	for(int r = y; r < y + h; r++, pixels+=w){
		uint64_t p1 = ((uint64_t)pixels[0]) | (((uint64_t)pixels[1]) << 32);
		uint64_t p2 = ((uint64_t)pixels[2]) | (((uint64_t)pixels[3]) << 32);
		outd(FB_ADDR+(x+r*W)*4, p1);
		outd(FB_ADDR+(x+2+r*W)*4, p2);
	}
	outl(SYNC_ADDR, ctl->sync);
}
*/
void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
	int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
	uint32_t *pixels = ctl->pixels;
	if(w&0x0001){
		for(int r = y; r < y + h; r++, pixels+=w){
			for(int c = x; c < x + w; c++){
				uint32_t p = pixels[c-x];
				outl(FB_ADDR+(c+r*W)*4, p);
			}
		}
		outl(SYNC_ADDR, ctl->sync);
		return ;
	}
	for(int r = y; r < y + h; r++, pixels+=w){
		for(int c = x; c < x + w; c+=2){
			uint32_t p = pixels[c-x];
			uint32_t p1 = pixels[c-x+1];
			uint64_t p3 = (uint64_t)p+((uint64_t)p1<<32);
			outd(FB_ADDR+(c+r*W)*4, p3);
		}
	}
	outl(SYNC_ADDR, ctl->sync);
}
/*
void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
	int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
	uint32_t *pixels = ctl->pixels;
	if(w&0x0001){
			printf("bb\n");
		for(int r = y; r < y + h; r++, pixels+=w){
			for(int c = x; c < x + w; c++){
				uint32_t p = pixels[c-x];
				outl(FB_ADDR+(c+r*W)*4, p);
			}
		}
		outl(SYNC_ADDR, ctl->sync);
		return ;
	}
	int r=0;
	for(r = y; (r+1)<(y + h); r+=2, pixels+=(w<<1)){
		for(int c = x; c < x + w; c+=2){
			uint32_t p = pixels[c-x];
			uint32_t p1 = pixels[c-x+1];
			uint64_t p3 = (uint64_t)p+((uint64_t)p1<<32);
			outd(FB_ADDR+(c+r*W)*4, p3);
			uint32_t p4 = pixels[c-x+w];
			uint32_t p5 = pixels[c-x+1+w];
			uint64_t p6 = (uint64_t)p4+((uint64_t)p5<<32);
			outd(FB_ADDR+(c+(r+1)*W)*4, p6);
		}
	}
	if(r==y+h-1){
		for(int c = x; c < x + w; c+=2){
			uint32_t p = pixels[c-x];
			uint32_t p1 = pixels[c-x+1];
			uint64_t p3 = (uint64_t)p+((uint64_t)p1<<32);
			outd(FB_ADDR+(c+r*W)*4, p3);
	}

	outl(SYNC_ADDR, ctl->sync);
}
}
*/
/*
void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
  int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
  uint32_t *pixels = ctl->pixels;
  int in_loop_time=w>>2;
  int sel=w&0x0003;
  if(sel==0){
  for(int r = y; r < y + h; r++, pixels+=w){
		  int i=0;
    for(int c = x; i < in_loop_time; c+=4){
      uint32_t p = pixels[c-x];
      uint32_t p1 = pixels[c-x+1];
      uint64_t p3 = (uint64_t)p+((uint64_t)p1<<32);

      uint32_t p4 = pixels[c-x+2];
      uint32_t p5 = pixels[c-x+3];
      uint64_t p6 = (uint64_t)p4+((uint64_t)p5<<32);

      outd(FB_ADDR+((c+r*W)<<2), p3);
      outd(FB_ADDR+((c+r*W+2)<<2), p6);
	  i++;
      }
    }
  outl(SYNC_ADDR, ctl->sync);
  return;
  }else if(sel ==1){
		  int i=0;
		  int c=0;
  for(int r = y; r < y + h; r++, pixels+=w){
    for(c = x; i < in_loop_time; c+=4){
      uint32_t p = pixels[c-x];
      uint32_t p1 = pixels[c-x+1];
      uint64_t p3 = (uint64_t)p+((uint64_t)p1<<32);

      uint32_t p4 = pixels[c-x+2];
      uint32_t p5 = pixels[c-x+3];
      uint64_t p6 = (uint64_t)p4+((uint64_t)p5<<32);

      outd(FB_ADDR+((c+r*W)<<2), p3);
      outd(FB_ADDR+((c+r*W+2)<<2), p6);
	  i++;
      }
	  c=c-4;
	  uint32_t p = pixels[c-x];
	  outl(FB_ADDR+((c+r*W)<<2), p);
    }
  outl(SYNC_ADDR, ctl->sync);
  return;
  }else if(sel ==2){
  for(int r = y; r < y + h; r++, pixels+=w){
		  int i=0;
		  int c=0;
    for(c = x; i < in_loop_time; c+=4){
      uint32_t p = pixels[c-x];
      uint32_t p1 = pixels[c-x+1];
      uint64_t p3 = (uint64_t)p+((uint64_t)p1<<32);

      uint32_t p4 = pixels[c-x+2];
      uint32_t p5 = pixels[c-x+3];
      uint64_t p6 = (uint64_t)p4+((uint64_t)p5<<32);

      outd(FB_ADDR+((c+r*W)<<2), p3);
      outd(FB_ADDR+((c+r*W+2)<<2), p6);
	  i++;
      }
	  c=c-4;
	  uint32_t p = pixels[c-x];
      uint32_t p1 = pixels[c-x+1];
      uint64_t p3 = (uint64_t)p+((uint64_t)p1<<32);
      outd(FB_ADDR+((c+r*W)<<2), p3);
  }
  outl(SYNC_ADDR, ctl->sync);
  return;
  }else{
  for(int r = y; r < y + h; r++, pixels+=w){
		  int i=0;
		  int c=0;
    for(c = x; i < in_loop_time; c+=4){
      uint32_t p = pixels[c-x];
      uint32_t p1 = pixels[c-x+1];
      uint64_t p3 = (uint64_t)p+((uint64_t)p1<<32);

      uint32_t p4 = pixels[c-x+2];
      uint32_t p5 = pixels[c-x+3];
      uint64_t p6 = (uint64_t)p4+((uint64_t)p5<<32);

      outd(FB_ADDR+((c+r*W)<<2), p3);
      outd(FB_ADDR+((c+r*W+2)<<2), p6);
	  i++;
      }
	  c=c-4;
	  uint32_t p = pixels[c-x];
      uint32_t p1 = pixels[c-x+1];
      uint32_t p4 = pixels[c-x+2];
      uint64_t p3 = (uint64_t)p+((uint64_t)p1<<32);
      outd(FB_ADDR+((c+r*W)<<2), p3);
	  outl(FB_ADDR+((c+r*W+2)<<2),p4);
  }
  outl(SYNC_ADDR, ctl->sync);
  return;
  }
}
*/

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
