#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000
void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
	/*
  if(AM_KEY_NONE==*KBD_ADDR){
    kbd->keydown = 0;
    kbd->keycode = AM_KEY_NONE;
  }else{
    kbd->keydown =1 ;
    kbd->keycode =*KBD_ADDR;
  }*/
	uint32_t input = inl(KBD_ADDR);
	kbd->keydown = (input)&KEYDOWN_MASK; 
	kbd->keycode = (input)&(~KEYDOWN_MASK);

}
