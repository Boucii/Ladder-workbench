#include <iringbuf/iringbuf.h>
#include<cpu/decode.h>
char [20][128] iringbuf;

inline static uint32_t ibufptr=0;

int write_irbuf(Decode *d){
  strcpy(iringbuf[ibufptr],d->logbuf);
  ibufptr++;
  if(!ibufptr<20){
    ibufptr=0;
  }
  return 0;
}
void print_buf(){
  for(int i=0;i<ibufptr-1;i++){
	log_write("     %s\n",iringbuf[i]);
  }
  log_write("---->%s\n",iringbuf[i]);
}
