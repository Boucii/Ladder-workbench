#include <am.h>
#include <nemu.h>

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

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl) {
	/*
  uint32_t *p = ctl->pixels;
  for (uint32_t i = ctl->y; i < ctl->y + ctl->h; i ++) {
    for (uint32_t j = ctl->x; j < ctl-> x + ctl->w; j ++) {
      outl(FB_ADDR + i*4*ctl->w + j*4, *p);
      p ++;
    }
  }
    outl(SYNC_ADDR, ctl->sync);
    */
	int x = ctl->x, y = ctl->y, w = ctl->w, h = ctl->h;
	uint32_t *pixels = ctl->pixels;
	for(int r = y; r < y + h; r++, pixels+=w){
		for(int c = x; c < x + w; c++){
			uint32_t p = pixels[c-x];
			outl(FB_ADDR+(c+r*W)*4, p);
		}
	}
	outl(SYNC_ADDR, ctl->sync);
}

void __am_gpu_status(AM_GPU_STATUS_T *status) {
  status->ready = true;
}
