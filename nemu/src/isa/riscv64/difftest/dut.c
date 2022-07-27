#include <isa.h>
#include <cpu/difftest.h>
#include "../local-include/reg.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  bool res=true;
	for(int i=0;i<32;i++){
            if(cpu.gpr[i]!=ref_r->gpr[i]){
		    res=false;
	            break;
	}
  
  }
  return res;
}

void isa_difftest_attach() {
}
