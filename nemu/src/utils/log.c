#include <common.h>
#include <iringbuf/iringbuf.h>
#include<cpu/decode.h>
extern uint64_t g_nr_guest_inst;
FILE *log_fp = NULL;

char iringbuf[20][128];

uint32_t ibufptr=0;

int write_irbuf(Decode *d){
  strcpy(iringbuf[ibufptr],d->logbuf);
  ibufptr++;
  if(!(ibufptr<20)){
    ibufptr=0;
  }
  return 0;
}
void print_buf(){
  int i=0;
  for(;i<ibufptr-1;i++){
	log_write("     %s\n",iringbuf[i]);
  }
  log_write("---->%s\n",iringbuf[i]);
}

void init_log(const char *log_file) {
  log_fp = stdout;
  if (log_file != NULL) {
    FILE *fp = fopen(log_file, "w");
    Assert(fp, "Can not open '%s'", log_file);
    log_fp = fp;
  }
  Log("Log is written to %s", log_file ? log_file : "stdout");
}

bool log_enable() {
  return MUXDEF(CONFIG_TRACE, (g_nr_guest_inst >= CONFIG_TRACE_START) &&
         (g_nr_guest_inst <= CONFIG_TRACE_END), false);
}
