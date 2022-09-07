#include <isa.h>
#include <cpu/cpu.h>
#include <difftest-def.h>
#include <memory/paddr.h>

void difftest_memcpy(paddr_t addr, void *buf, size_t n, bool direction) {
	if(direction==DIFFTEST_TO_DUT){
	  for(int i=0;i<n;i++){
	    ((uint8_t*)buf)[i]=paddr_read(addr+i,1);
	  }
	}else{
	  for(int i=0;i<n;i++){
	      paddr_write(addr+i,1,*((uint8_t*)buf+i));
	  }
	}
  //assert(0);
}

void difftest_regcpy(void *dut, bool direction) {

	if(direction==DIFFTEST_TO_DUT){
		for(int i=0;i<32;i++){
		  ((uint64_t *)dut)[i]=cpu.gpr[i];
		}
		((uint64_t *)dut)[32]=(uint64_t)cpu.pc;
	}else{  //DIFFTEST_TO_REF
		for(int i=0;i<32;i++){
		    cpu.gpr[i]=((uint64_t *)dut)[i];
	}
  }
}

void difftest_exec(uint64_t n) {
  cpu_exec(n);
  //assert(0);
}

void difftest_raise_intr(word_t NO) {
  assert(0);
}

void difftest_init(int port) {
  /* Perform ISA dependent initialization. */
  init_isa();
}
