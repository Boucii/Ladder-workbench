#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string>
#include "VTOP.h"
//#include <verilated.h>
#include "verilated_vcd_c.h"
#include "memory.h"
//#include <svdpi.h>
//#include "VTOP__Dpi.h"
#include "verilated_dpi.h"
#include "log.h"
#include <dlfcn.h>

#define DIFFTEST_EN 1
#define ITRACE_EN 0

#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */

//possible to change context when invoking getregs
using namespace std;

#define MAX_TIME 50

//from memory.h

#define RESET_VECTOR 0x80000000

extern uint8_t *mem;
uint64_t img_size=0;
extern uint64_t pmem_read(int addr);
extern int pmem_write(uint64_t content,uint64_t addr,uint32_t len);
extern int free_memory();
extern svBit Check();

uint64_t *cpu_gpr = NULL;
uint32_t pc=0;
long load_img(char **argv);

extern int init_disasm(const char *triple);
extern void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);

enum { DIFFTEST_TO_DUT, DIFFTEST_TO_REF };
void (*ref_difftest_memcpy)(uint32_t addr, void *buf, size_t n, bool direction) = NULL;
void (*ref_difftest_regcpy)(void *dut, bool direction) = NULL;
void (*ref_difftest_exec)(uint64_t n) = NULL;
void (*ref_difftest_raise_intr)(uint64_t NO) = NULL;


extern ofstream fout;
void init_difftest() {
  char ref_so_file[]="/home/mint/ysyx-workbench/nemu/build/riscv64-nemu-interpreter-so";

  assert(ref_so_file != NULL);

  void *handle;
  handle = dlopen(ref_so_file, RTLD_LAZY);
  //assert(handle);

  cout<<dlerror()<<"jajajajajajajajaja";
  ref_difftest_memcpy = (void (*)(uint32_t addr, void *buf, size_t n, bool direction))dlsym(handle, "difftest_memcpy");
  assert(ref_difftest_memcpy);

  ref_difftest_regcpy = (void (*)(void *dut, bool direction))(dlsym(handle, "difftest_regcpy"));
  //ref_difftest_regcpy = dlsym(handle, "difftest_regcpy");
  assert(ref_difftest_regcpy);

  ref_difftest_exec = (void (*)(uint64_t n))(dlsym(handle, "difftest_exec"));
  //ref_difftest_exec = dlsym(handle, "difftest_exec");
  assert(ref_difftest_exec);

  ref_difftest_raise_intr = (void (*)(uint64_t NO))(dlsym(handle, "difftest_raise_intr"));
  //ref_difftest_raise_intr = dlsym(handle, "difftest_raise_intr");
  assert(ref_difftest_raise_intr);

  void (*ref_difftest_init)(int) = (void (*)(int))dlsym(handle, "difftest_init");
  assert(ref_difftest_init);

  ref_difftest_init(0);
  ref_difftest_memcpy(RESET_VECTOR,mem, img_size, DIFFTEST_TO_REF);
  ref_difftest_regcpy(cpu_gpr, DIFFTEST_TO_REF);
}

char logbuf[50]="\0";
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
//----------------------DPI-C FUNCTIONS------------------------
const svOpenArrayHandle r=NULL;
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
//---------------------------------------------------------------
void diff_check_regs(){
	for(int i=0;i<32;i++){
	  if(ref_gpr[i]!=cpu_gpr[i]){
	      cout<<ios::hex<<"Error:Difftest failed at pc=0x"<<pc<<ios::dec<<"reg "<<i<<endl;
	      cout<<ios::hex<<"cpu_gpr="<<GREEN<<cpu_gpr[i]<<RESET<<"and ref ="<<BOLDGREEN<<ref_gpr[i]<<endl<<ios::dec;
	  }
	  if(ref_gpr[32]!=pc){
	      cout<<ios::hex<<RED<<"pc error! pc="<<GREEN<<pc<<RESET<<"and ref ="<<BOLDGREEN<<ref_gpr[32]<<endl<<ios::dec<<RESET;
	  }
	  
	}
}

uint64_t ref_gpr[33];
void difftest_exec_once(){
	ref_difftest_exec(1);
	ref_difftest_regcpy(ref_gpr,DIFFTEST_TO_DUT);
	diff_check_regs();
}



int main(int argc, char** argv, char** env){
  cout<<argc<<endl;
  cout<<"argvs:"<<argv[0]<<argv[1]<<endl;
  contextp = new VerilatedContext;
  contextp->commandArgs(argc, argv);
  top= new VTOP{contextp};
  
  //set scope for dpi-c function  
  Verilated::scopesDump();
  const svScope scope = svGetScopeFromName("TOP.npc_ctl");
  assert(scope);  // Check for nullptr if scope not found
  svSetScope(scope);

  set_gpr_ptr(r);

  contextp->traceEverOn(true); //打开追踪功能
  tfp = new VerilatedVcdC; //初始化VCD对象指针
  top->trace(tfp, 0); //
  tfp->open("wave.vcd"); //设置输出的文件wave.vcd

  int time=0;
  LogInit();
  mem_init();
  load_img(argv);
  if(ITRACE_EN){
      init_disasm("riscv64-pc-linux-gnu");
  }
  if(DIFFTEST_EN){
      init_difftest();
  }
  reset(10);
  cout<<"\nstart simulating\n";
  dumpwave();
  while (time<MAX_TIME) {
    cout<<"cycle "<<time<<" passed\n";
    //instruction fetch
    int addr=(int)(top->io_InstAddr);
    pc=addr;
    Log("0x");
    char hex_string[20];
    sprintf(hex_string, "%X", addr);
    string temp1(hex_string);
    Log(temp1);
    Log(":	");
    uint32_t cur_inst = (uint32_t)pmem_read(addr);
    top->io_InstIn = cur_inst;
    uint8_t *instaddr=(uint8_t *)&cur_inst;
    uint64_t addrin=(uint64_t)((uint64_t)addr)-(uint64_t)0xffffffff00000000;
    if(ITRACE_EN){
        disassemble(logbuf, 50, addrin, instaddr, 4);
    }
    string temp=logbuf;
    Log(temp);
    Log("\n");
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
    if(DIFFTEST_EN){
        difftest_exec_once();
    }
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
