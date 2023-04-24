#include <proc.h>
#include <elf.h>
#include <fs.h>
uint8_t buffer[0x300000];

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

size_t get_ramdisk_size();
size_t ramdisk_write(const void *buf, size_t offset, size_t len);
size_t ramdisk_read(void *buf, size_t offset, size_t len);

int fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);

static uintptr_t loader(PCB *pcb, const char *filename) {
	/*
  int size=get_ramdisk_size();
  ramdisk_read(buffer,0,size);
  Elf_Ehdr elfhdr=*((Elf_Ehdr*)buffer);
  Elf_Phdr elfphdr;
  assert(*(uint32_t *)elfhdr.e_ident==0x464c457f);

  for(int i=0;i<elfhdr.e_phnum;i++){
    ramdisk_read(&elfphdr,elfhdr.e_phoff+i*sizeof(Elf_Phdr),sizeof(Elf_Phdr));
    if(elfphdr.p_type==PT_LOAD){
        ramdisk_read((void*)elfphdr.p_vaddr,elfphdr.p_offset,elfphdr.p_memsz);
	memset((void*)(elfphdr.p_vaddr+elfphdr.p_filesz),0,elfphdr.p_memsz-elfphdr.p_filesz);
    }
  }
  return elfhdr.e_entry;
  */
  printf("loader,load=%s\n",filename);
  int fd=fs_open(filename,0,0);
   Elf_Ehdr elfhdr;
   Elf_Phdr elfphdr;
   fs_lseek(fd,0,SEEK_SET);
   fs_read(fd,&elfhdr,sizeof(Elf_Ehdr));
   assert(*(uint32_t *)elfhdr.e_ident==0x464c457f);
	//printf("section num=%d\n",elfhdr.e_phnum);
	//printf("e_ident=%x\n",elfhdr.e_ident);
	//printf("entry=%lx\n",elfhdr.e_entry);
  for(int i=0;i<elfhdr.e_phnum;i++){
    fs_lseek(fd,elfhdr.e_phoff+i*sizeof(Elf_Phdr),SEEK_SET);
    fs_read(fd,&elfphdr,sizeof(Elf_Phdr));
	//printf("phdr type=%d",elfphdr.p_type);
	//printf("PT_LOAD=%d",PT_LOAD);
    if(elfphdr.p_type==PT_LOAD&&elfphdr.p_memsz!=0&&elfphdr.p_paddr!=0){
	printf("sec loaded,to addr=%lx,sz=%d\n",elfphdr.p_paddr,elfphdr.p_memsz);
	fs_lseek(fd,elfphdr.p_offset,SEEK_SET);
	fs_read(fd,(void*)elfphdr.p_paddr,elfphdr.p_memsz);
	memset((void*)(elfphdr.p_paddr+elfphdr.p_filesz),0,elfphdr.p_memsz-elfphdr.p_filesz);
    }
  }
  //printf("entry=%lx\n",elfhdr.e_entry);
  return elfhdr.e_entry;



}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

