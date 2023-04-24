#include <stdio.h>
#include <assert.h>
#include <NDL.h>
int main() {
  FILE *fp = fopen("/share/files/num", "r+");
  assert(fp);

  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);
  assert(size == 5000);

  fseek(fp, 500 * 5, SEEK_SET);
  int i, n;
  for (i = 500; i < 1000; i ++) {
    fscanf(fp, "%d", &n);
    assert(n == i + 1);
  }

  fseek(fp, 0, SEEK_SET);
  for (i = 0; i < 500; i ++) {
    fprintf(fp, "%4d\n", i + 1 + 1000);
  }

  for (i = 500; i < 1000; i ++) {
    fscanf(fp, "%d", &n);
    assert(n == i + 1);
  }

  fseek(fp, 0, SEEK_SET);
  for (i = 0; i < 500; i ++) {
    fscanf(fp, "%d", &n);
    assert(n == i + 1 + 1000);
  }

  fclose(fp);

  printf("PASS!!!\n");

  return 0;
}
/*

uint32_t NDL_GetTicks();
int _gettimeofday(struct timeval *tv, struct timezone *tz);
int main() {
  int mm=1;
  uint32_t a;
  a=NDL_GetTicks();
 while(1){
   if(NDL_GetTicks()-a>mm*500){
     printf("11\n");
     mm++;
   }
 }
 return 0;}
 */
	/*
  struct timeval a,b;
  _gettimeofday(&a,NULL);
  long long s=1,u=0;
  printf("???\n");
  while(1){
    _gettimeofday(&b,NULL);
    u=b.tv_usec+(b.tv_sec-a.tv_sec)*1000000;
    if(u>s*500000){
      printf("aaa\n");
      s++;
    }else{
      //printf("%ld\n",b.tv_sec);
    }
  }
    return 0;
}


	
  printf("done1lalala\n\n\n");
  FILE *fp = fopen("/share/files/num", "r+");
  assert(fp);
  
  printf("done1nanana\n\n\n");
  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);
  assert(size == 5000);

  printf("done2papapa\n");
  fseek(fp, 500 * 5, SEEK_SET);
  int i, n;
  for (i = 500; i < 1000; i ++) {
    fscanf(fp, "%d", &n);
    assert(n == i + 1);
  }

  printf("done3\n");
  fseek(fp, 0, SEEK_SET);
  for (i = 0; i < 500; i ++) {
    fprintf(fp, "%4d\n", i + 1 + 1000);
  }

  printf("done4\n");
  for (i = 500; i < 1000; i ++) {
    fscanf(fp, "%d", &n);
    assert(n == i + 1);
  }

  printf("done5\n");
  fseek(fp, 0, SEEK_SET);
  for (i = 0; i < 500; i ++) {
    fscanf(fp, "%d", &n);
    assert(n == i + 1 + 1000);
  }

  fclose(fp);

  printf("PASS!!!\n");
  return 0;
}
*/
