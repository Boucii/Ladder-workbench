#include <isa.h>
#include "local-include/reg.h"
extern CPU_state cpu;
const char *regs[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};

void isa_reg_display() {
 for(int i=0;i<32;i++){
   printf("%s: ",regs[i]);       
   printf("%#lx \n",cpu.gpr[i]);  
  }
}


word_t isa_reg_str2val(const char *s, bool *success) {
  if(strcmp(s,"$pc")==0){
    *success=true;
    return (word_t)cpu.pc;
  }else if(strcmp(s,"$0")==0){
    *success=true;
    return cpu.gpr[0];
  }
  *success=false;
  for(int i=1;i<32;i++){
    if(strcmp(s+1,regs[i])==0){
      *success=true;
      return cpu.gpr[i];
    }
  }
  return 0;

}
