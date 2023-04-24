#ifndef ARCH_H__
#define ARCH_H__

struct Context {
  // TODO: fix the order of these members to match trap.S
  //uintptr_t mepc, mstatus ,mcause, gpr[32];
  uintptr_t gpr[32],mcause,mstatus,mepc;
  void *pdir;
};

#define GPR1 gpr[17] // a7
#define GPR2 gpr[10] //args
#define GPR3 gpr[11]
#define GPR4 gpr[12]
#define GPRx gpr[10] //ret val
#endif
