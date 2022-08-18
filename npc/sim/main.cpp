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

using namespace std;

//from memory.h
extern int mem_init();
extern uint32_t pmem_read(int addr);
extern int pmem_write(uint32_t content,uint32_t addr);
extern int free_memory();
extern long load_img();


extern "C" void Check();
//from verilog DPI-C
extern "C" void helloFromCpp() {
  // 0 is 0
  // 1 is 1
  // 2 is Z
  // 3 is X
  int a_int = 1;
  cout << "(C++) a is " << a_int << endl;
  svScope scope_a = svGetScope();
  if(!scope_a)
  svSetScope(scope_a);
  cout << "Scope is " << svGetNameFromScope(svGetScope()) << endl;
  svSetScope (svGetScopeFromName ("top"));
  Check();
  //return 0;
}
//extern "C" svBit Check();
extern void check();
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
void reset(int n) {
	  top->reset = 1;
	    while (n -- > 0) single_cycle();
	      top->reset = 0;
}

int main(int argc, char** argv, char** env){
  contextp = new VerilatedContext;
  contextp->commandArgs(argc, argv);
  top= new VTOP{contextp};

  contextp->traceEverOn(true); //打开追踪功能
  tfp = new VerilatedVcdC; //初始化VCD对象指针
  top->trace(tfp, 0); //
  tfp->open("wave.vcd"); //设置输出的文件wave.vcd

  int time=0;
  mem_init();
  load_img();
  reset(10);
  cout<<"\nstart simulating\n";
  dumpwave();
  while (time<20) {
    cout<<"cycle passed\n";

    int addr=(int)(top->io_InstReadPort_addr);
    top->io_InstReadPort_data = pmem_read(addr);
   // svSetScope (svGetScopeFromName ("SimuControl.v"));
 // svSetScope (svGetScopeFromName ("VTOP.v"));
    //if(Check()){
      //break;  
    //}
    //helloFromCpp();
    single_cycle();
    time++;
  }
  cout<<"simulation over\n";
  delete top;
  delete contextp;
  tfp->close();
  return 0;
}
