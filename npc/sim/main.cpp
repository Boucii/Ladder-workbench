#include <iostream>
#include <svdpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "VTOP.h"
#include <verilated.h>
#include "verilated_vcd_c.h"
#include "memory.h"
#include <svdpi.h>
#include "VTOP__Dpi.h"
#include "verilated_dpi.h"
#include "log.h"

//possible to change context when invoking getregs
using namespace std;

#define MAX_TIME 50

//from memory.h
extern int mem_init();
extern uint64_t pmem_read(int addr);
extern int pmem_write(uint64_t content,uint64_t addr,uint32_t len);
extern int free_memory();
extern long load_img(char **argv);


extern "C" svBit Check();

uint64_t *cpu_gpr = NULL;

extern "C" void set_gpr_ptr(const svOpenArrayHandle r) {
  cpu_gpr = (uint64_t *)(((VerilatedDpiOpenVar*)r)->datap());
}

// 一个输出RTL中通用寄存器的值的示例
void dump_gpr() {
  int i;
  for (i = 0; i < 32; i++) {
    printf("gpr[%d] = 0x%lx\n", i, cpu_gpr[i]);
  }
}

//Start of Program
static VTOP* top;
VerilatedVcdC* tfp=NULL;
VerilatedContext* contextp=NULL;
void dumpwave(){
    tfp->dump(contextp->time()); //dump wav
    contextp->timeInc(1); //推动仿真时间
}
void single_cycle() {
    top->clock = 0; top->eval(); 
    dumpwave();
    top->clock = 1; top->eval();
    dumpwave();
}
void single_cycleup() {
    top->clock = 0; top->eval(); 
    dumpwave();
}
void single_cycledown() {
    top->clock = 1; top->eval();
    dumpwave();
}

void reset(int n) {
	  top->reset = 1;
	    while (n -- > 0) single_cycle();
	      top->reset = 0;
}

int main(int argc, char** argv, char** env){
  cout<<argc<<endl;
  cout<<"argvs:"<<argv[0]<<argv[1]<<endl;
  contextp = new VerilatedContext;
  contextp->commandArgs(argc, argv);
  top= new VTOP{contextp};
  
  //set scope for dpi-c function  
  //Verilated::scopesDump();
  const svScope scope = svGetScopeFromName("TOP.nemu_stop");
  assert(scope);  // Check for nullptr if scope not found
  svSetScope(scope);

  contextp->traceEverOn(true); //打开追踪功能
  tfp = new VerilatedVcdC; //初始化VCD对象指针
  top->trace(tfp, 0); //
  tfp->open("wave.vcd"); //设置输出的文件wave.vcd

  int time=0;
  mem_init();
  load_img(argv);
  reset(10);
  cout<<"\nstart simulating\n";
  dumpwave();
  while (time<MAX_TIME) {
    cout<<"cycle "<<time<<" passed\n";
    //instruction fetch
    int addr=(int)(top->io_InstAddr);
    top->io_InstIn = (uint32_t)pmem_read(addr);
    //memory read/write
    single_cycleup();
    if(top->io_Men){
      if(top->io_Mwout){//write
	uint64_t data=top->io_MdataOut;
	uint32_t addr=top->io_Maddr;
	uint32_t len=top->io_Mlen;
	pmem_write(data,addr,len);
      }else{  //read
	uint32_t addr=top->io_Maddr;
	uint32_t len=top->io_Mlen;
	top->io_MdataIn=pmem_read(addr);
      }
    }
    single_cycledown();
    //check for trap
    if(Check()){
      cout<<"HIT GOOD TRAP"<<endl;
      break;  
    }
    time++;
  }
  if(time==MAX_TIME){
      cout<<"HIT BAD TRAP"<<endl;
  }
  dump_gpr();
  cout<<"simulation over\n";
  delete top;
  delete contextp;
  tfp->close();
  return 0;
}
