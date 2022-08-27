#include<stdint.h>
#include<iostream>
#include<stdio.h>
#include<cassert>
#include <filesystem>
#include <unistd.h>


uint8_t *mem=nullptr;
int size=0x8000000;
int mem_init(){
  mem=new uint8_t[size];
  std::cout<<"mem defined\n";
  return 0;
}


uint32_t pmem_read(int addr){
  std::cout<<"mem read: 0x"<<std::hex<<addr<< "\n";
  int base_addr=addr-0x80000000;
  uint32_t lo0=(uint32_t)mem[base_addr];
  uint32_t lo1=((uint32_t)mem[base_addr+1])<<8;
  uint32_t lo2=((uint32_t)mem[base_addr+2])<<16;
  uint32_t lo3=((uint32_t)mem[base_addr+3])<<24;
  std::cout<<"0x";
  return lo0+lo1+lo2+lo3;
}
int pmem_write(uint32_t content,uint32_t addr){
  int base_addr=addr-0x80000000;
  mem[addr-0x80000000]=content;
  mem[addr-0x80000000+1]=content>>8;
  mem[addr-0x80000000+2]=content>>16;
  mem[addr-0x80000000+3]=content>>24;
  return 0;
}
int free_memory(){
    delete mem;
    return 0;
}

char name[30]="sim/demorevert.txt";
static char *img_file=name;


long load_img(){
  if(img_file==NULL){
    std::cout<<"no img is given,please specify a img";
    return -1;
  }

//打印输出当前路径
    char *buffer;
    //也可以将buffer作为输出参数
    if((buffer = getcwd(NULL, 0)) == NULL)
    {
        perror("getcwd error");
    }
    else
    {
        printf("path:%s\n", buffer);
        free(buffer);
    }

  std::cout<<"trying to find img\n";
  FILE *fp =fopen(img_file,"rb");
  if(fp==NULL){
  std::cout<<"open failed\n";
  }
  //assert(fp, "Can not open '%s'", img_file);

  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);

  std::cout<<"no img is "<<img_file<<", size="<<size;
  fseek(fp, 0, SEEK_SET);
  int ret = fread(mem, size, 1, fp);
  assert(ret == 1);

  fclose(fp);
  return size;
}


/*
int main(){
  mem_init();
  load_img();
  //pmem_write((uint32_t)0x12345678,0x80000000);
  //pmem_write((uint32_t)0x87654321,0x80000004);
  std::cout<<std::hex<<pmem_read(0x80000000)<<"\n";
  std::cout<<std::hex<<pmem_read(0x80000001)<<"\n";
  std::cout<<std::hex<<pmem_read(0x80000002)<<"\n";
  std::cout<<std::hex<<pmem_read(0x80000003)<<"\n";
  std::cout<<std::hex<<pmem_read(0x80000004)<<"\n";
  free_memory();
  return 0;
}
*/
