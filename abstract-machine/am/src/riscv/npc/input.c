#include <am.h>

//#define DEVICE_BASE 0x0 //this is wrong ,,,but how did the previous worked?
# define DEVICE_BASE 0xa0000000
#define KBD_ADDR        (DEVICE_BASE + 0x0000060)
#define KEYDOWN_MASK 0x8000
#define VGACTL_ADDR     (DEVICE_BASE + 0x0000100)
static inline uint32_t inl(uintptr_t addr) { return *(volatile uint32_t *)addr; }

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
		/*
  kbd->keydown = 0;
  kbd->keycode = AM_KEY_NONE;
*/
	uint32_t input = inl(KBD_ADDR);
	kbd->keydown = (input)&KEYDOWN_MASK; 
	kbd->keycode = (input)&(~KEYDOWN_MASK);
}
