#include <common.h>
int mm_brk(uintptr_t brk);
int fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);
//------------------------timer----------------------
#define RTC_ADDR 0xa0000048
struct timeval{
  long tv_sec;
  long tv_usec;
};
int gettimeofday(struct timeval *tv,void *tz){
  uint64_t *t = (uint64_t*)(uintptr_t)RTC_ADDR;
  tv->tv_sec = *t / 1000000;
  tv->tv_usec = *t % 1000000;
  return 0;
}

void do_syscall(Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;
  
#ifdef STRACE
  printf("syscall id=%d args1=%d\n",a[0],a[1]);
#endif
  switch (a[0]) {
    case 0:halt(c->GPR2);//exit
    case 1:yield();c->GPRx=0;break;//sys_yield 
    case 2:c->GPRx=fs_open((char*)a[1],a[2],a[3]);break;
    case 3:c->GPRx=fs_read(a[1],(void*)a[2],a[3]);break;
    case 4:c->GPRx=fs_write(a[1],(void*)a[2],a[3]);//break;
	   break;
    case 7:c->GPRx=fs_close(a[1]);break;
    case 8:c->GPRx=fs_lseek(a[1],a[2],a[3]);break;
    case 9:c->GPRx=mm_brk(a[1]);break;
    case 19:c->GPRx=gettimeofday((void*)a[1],(void*)a[2]);
	    break;
    default: panic("Aha!Unhandled syscall ID = %d", a[0]);
  }
#ifdef STRACE
  printf("syscall exeited\n");
#endif
}
/*
     case 4:
     //args fd,buf,count
     if(a[1]==1||a[1]==2){
       for(int i=0;i<a[3];i++){
 	      printf("\nfrom syscall,i=%d\n",i);
         putch(*((char*)a[2]+i));
       }
       c->GPRx = a[3];
     }else{
       c->GPRx = 0;
     }
     break;*/
