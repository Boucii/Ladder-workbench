#ifndef __ISA_RISCV64_H__
#define __ISA_RISCV64_H__

#include <common.h>

enum csr_name{
  mtvec=0, mepc=1, mstatus=2, mcause=3
}CSRS;
typedef struct {
  word_t gpr[32];
  vaddr_t pc;
  word_t csr[4];
} riscv64_CPU_state;

// decode
typedef struct {
  union {
    uint32_t val;
  } inst;
} riscv64_ISADecodeInfo;

#define isa_mmu_check(vaddr, len, type) (MMU_DIRECT)

#endif
