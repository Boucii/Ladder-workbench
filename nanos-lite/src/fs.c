#include <fs.h>
#define VGACTL_ADDR 0xA0000100
typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

size_t ramdisk_write(const void *buf, size_t offset, size_t len);
size_t ramdisk_read(void *buf, size_t offset, size_t len);

size_t dispinfo_read(void *buf, size_t offset, size_t len);
size_t fb_write(const void *buf, size_t offset, size_t len);

size_t serial_write(const void *buf, size_t offset, size_t len);
size_t events_read(void *buf, size_t offset, size_t len);
typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  size_t open_offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB, FD_EVENTS, FD_DISPINFO};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
/*path,size,offset*/
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]    = {"stdin" ,         0, 0, .read=invalid_read,  .write=invalid_write},
  [FD_STDOUT]   = {"stdout",         0, 0, .read=invalid_read,  .write=serial_write},
  [FD_STDERR]   = {"stderr",         0, 0, .read=invalid_read,  .write=serial_write},
  [FD_FB]       = {"/dev/fb",        0, 0, .read=invalid_read,  .write=fb_write},
  [FD_EVENTS]   = {"/dev/events",    0, 0, .read=events_read ,  .write=invalid_write},
  [FD_DISPINFO] = {"/proc/dispinfo", 0, 0, .read=dispinfo_read, .write=invalid_write },
#include "files.h"
};

static int fs_size=0;
void init_fs() {
  fs_size=sizeof(file_table)/sizeof(file_table[0]);
  //initiate all screen
  uint32_t hw = *(uint32_t*)(uintptr_t)VGACTL_ADDR;
  file_table[FD_FB].size = (hw>>16) * (hw&0xFFFF) * 4;
}
int fs_open(const char *pathname, int flags, int mode){
  bool found=0;
  int i=0;
  for(;i<fs_size;i++){
#ifdef STRACE
	  printf("name=%s,openoff=%d,disk_offset=%d\n",file_table[i].name,file_table[i].open_offset,file_table[i].disk_offset);
#endif
	  if(strcmp(file_table[i].name,pathname)==0){
	    file_table[i].open_offset=0;
	    found=1;
	    break;
	  }
  }
  if(!found) panic("file not found!");
  return i;
}
size_t fs_read(int fd, void *buf, size_t len){
  assert(fd<=fs_size);
  if(fd==0||fd==1||fd==2){
    return 0;//read std
  }
  if(file_table[fd].read){
    return (*file_table[fd].read)(buf,file_table[fd].open_offset,len);
  }
  size_t actual_len=(file_table[fd].open_offset+len)<=file_table[fd].size? \
		    len:file_table[fd].size-file_table[fd].open_offset;
  //printf("\nfread fd=%d,len=%d,alen=%d,open_offset=%d,disk_offset=%d\n",fd,len,actual_len,file_table[fd].open_offset,file_table[fd].disk_offset);
  ramdisk_read(buf,file_table[fd].open_offset+file_table[fd].disk_offset,actual_len);

  file_table[fd].open_offset += actual_len;
  return actual_len;
}
size_t fs_write(int fd, const void *buf, size_t len){
  assert(fd<=fs_size);
#ifdef STRACE
if(fd==3)
  //printf("fs_write,fd=%d,buf=%s,len=%d,offset=%d\n",fd,buf,len,file_table[fd].open_offset);
#endif
  if(file_table[fd].write){
    int alen=(*file_table[fd].write)(buf,file_table[fd].open_offset,len);
    file_table[fd].open_offset += alen;
    return alen;
  }
  if(fd==0){
    return 0;//write stdin
  }
  size_t actual_len=(file_table[fd].open_offset+len)<=file_table[fd].size? \
		    len:file_table[fd].size-file_table[fd].open_offset;
  ramdisk_write(buf,file_table[fd].open_offset+file_table[fd].disk_offset,actual_len);
  file_table[fd].open_offset += actual_len;
  return actual_len;

}
size_t fs_lseek(int fd, size_t offset, int whence){
  assert(fd<=fs_size);
  switch(whence){
    case SEEK_CUR:
	    file_table[fd].open_offset+=offset;break;
    case SEEK_SET:
	    file_table[fd].open_offset=offset;break;
    case SEEK_END:
	    file_table[fd].open_offset=offset+file_table[fd].size;break;
  }
  if(file_table[fd].open_offset>file_table[fd].size){
    file_table[fd].open_offset=file_table[fd].size;
  }
  return file_table[fd].open_offset;
}
int fs_close(int fd){
  return 0;
}
