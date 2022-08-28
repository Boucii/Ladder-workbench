#include<stdint.h>
#include<iostream>
#include<stdio.h>
#include<cassert>
int mem_init();
uint64_t pmem_read(int addr);
int pmem_write(uint32_t content,uint32_t addr,uint32_t len);
int free_memory();
long load_img();
