#include <isa.h>
#include <memory/paddr.h>

word_t vaddr_ifetch(vaddr_t addr, int len) {
  return paddr_read(addr, len);
}

word_t vaddr_read(vaddr_t addr, int len) {
	//if(addr<0x80000000)addr+=0x80000000;
  if(addr<0x80000000 ||addr>0xa0000000 ||addr==0xa0000000){
  	#ifdef CONFIG_TARGET_SHARE
			return 0;
  	#endif
  }
  return paddr_read(addr, len);
}

void vaddr_write(vaddr_t addr, int len, word_t data) {
  if(addr<0x80000000 ||addr>0xa0000000 ||addr==0xa0000000){
  	#ifdef CONFIG_TARGET_SHARE
  	  	return ;
  	#endif
  }
	//if(addr<0x80000000)addr+=0x80000000;
  paddr_write(addr, len, data);
}
