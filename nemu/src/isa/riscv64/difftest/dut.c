#include <isa.h>
#include <cpu/difftest.h>
#include "../local-include/reg.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  bool res=true;
	for(int i=0;i<32;i++){
            res=res&&difftest_check_reg(reg_name(i,0),pc,ref_r->gpr[i],cpu.gpr[i]);
	}
	res=res&&difftest_check_reg("pc",pc,ref_r->pc,cpu.pc);
  return res;
}

void isa_difftest_attach() {
}
