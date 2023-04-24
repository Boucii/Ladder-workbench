#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  char *ptr=(char *)s;
  size_t len=0;
  while(*ptr!='\0'){
    ptr++;
    len++;
  }
  return len;
}

char *strcpy(char *dst, const char *src) {
  char *ptr=(char *)src;
  char *tar=dst;
  while(*ptr!='\0'){
    *tar=*ptr;
    tar++;
    ptr++;
  }
  *(++tar)='\0';
  return dst;
}

char *strncpy(char *dst, const char *src, size_t n) {
  size_t i;
  for(i=0;i<n&&src[i]!='\0';i++){
    dst[i]=src[i];
  }
  for(;i<n;i++){
    dst[i]='\0';
  }
  return dst;
}

char *strcat(char *dst, const char *src) {
  if (dst == NULL || src == NULL){
	  return NULL;
    }
  char *tmp=dst;       
  while(*dst!= '\0'){
    dst++;
  }
  while(*src != '\0'){
    *dst++ = *src++;    
  }                    
  *dst = '\0';	
  return tmp;  
}

int strcmp(const char *s1, const char *s2) {
    char *str1=(char*)s1;
    char *str2=(char*)s2;
    while((*str1!='\0')&&(*str1==*str2))
    {
        str1++;
        str2++;
    }
    int t,res;
    t=*str1-*str2;
    if(t<0){
      res=-1;
    }else if(t==0){
      res=0;
    }else{
      res=1;
    }
    return res;
}

int strncmp(const char *s1, const char *s2, size_t n) {
    char *str1=(char*)s1;
    char *str2=(char*)s2;
    int i=0;
    while((*str1!='\0')&&(*str1==*str2)&&i<n)
    {
        str1++;
        str2++;
	i++;
    }
    int t,res;
    t=*str1-*str2;
    if(t<0){
      res=-1;
    }else if(t==0){
      res=0;
    }else{
      res=1;
    }
    return res;
}

void *memset(void *s, int c, size_t n) {
    if(s==NULL||n<0){
        return NULL;
    }
    char *temp=(char*) s;
    for(int i=0;i<n;i++){
        temp[i]=c;   
    }
    return s;
}

void *memmove(void *dst, const void *src, size_t n) {
    int *temparea=malloc(n*sizeof(int));
    for(int i=0;i<n;i++){
        temparea[i]=((int*)src)[i];
    }
    int *res=dst;
    for(int j=0;j<n;j++){
        res[j]=temparea[j];
    }
    return dst;
}

void *memcpy(void* dst,const void* src, size_t size) {
    size_t i, j;
    size_t count = size / 8;
    unsigned long long* dst_8 = (unsigned long long*) dst;
    unsigned long long* src_8 = (unsigned long long*) src;
    char* dst_c = (char*) dst_8;
    char* src_c = (char*) src_8;
    for (i = 0; i < count; i++) {
        dst_8[i] = src_8[i];
    }
    for (j = i * 8; j < size; j++) {
        dst_c[j] = src_c[j];
    }
	return dst;
}

/*
void* memcpy(void* out, const void* in, size_t n) {
	bool align_to_64=((long)in&(0x000000000000003f))==0 &&((long)out&(0x000000000000003f))==0;
	bool align_to_32=((long)in&(0x000000000000001f))==0 &&((long)out&(0x000000000000001f))==0;
	bool align_to_16=((long)in&(0x000000000000000f))==0 &&((long)out&(0x000000000000000f))==0;
	if(align_to_64){
    	uintptr_t a = (uintptr_t) out;
    	uintptr_t b = (uintptr_t) in;
    	uint64_t* c = (uint64_t*) a;
    	uint64_t* d = (uint64_t*) b;
    	size_t i = 0;
    	for (; (i + 8) <= n; i += 8) {
    	    c[i>>3] = d[i>>3];
    	}
    	for (; i < n; i++) {
    	    ((char*)out)[i] = ((const char*)in)[i];
    	}
    	return out;
	}
	if(align_to_32){
		uintptr_t a = (uintptr_t) out;
		uintptr_t b = (uintptr_t) in;
		uint32_t* c = (uint32_t*) a;
		uint32_t* d = (uint32_t*) b;
		size_t i = 0;
		for (; (i + 4) <= n; i += 4) {
		    c[i>>2] = d[i>>2];
		}
		for (; i < n; i++) {
		    ((char*)out)[i] = ((const char*)in)[i];
		}
		return out;
	}
	if(align_to_16){
		uintptr_t a = (uintptr_t) out;
		uintptr_t b = (uintptr_t) in;
		uint16_t* c = (uint16_t*) a;
		uint16_t* d = (uint16_t*) b;
		size_t i = 0;
		for (; (i + 2) <= n; i += 2) {
		    c[i>>1] = d[i>>1];
		}
		for (; i < n; i++) {
		    ((char*)out)[i] = ((const char*)in)[i];
		}
		return out;
	}
    char *a=(char*)out;
    char *b=(char*)in;
	for(int i=0;i<n;i++){
        a[i]=b[i];
    }
		return out;
}
*/
/*
void *memcpy11(void *out, const void *in, size_t n) {
    //assert(in+n<out)
    char *a=(char*)out;
    char *b=(char*)in;
	
	
	for(int i=0;i<n;i++){
        a[i]=b[i];
    }

	uint64_t *c=(uint64_t*)out;
	uint64_t *d=(uint64_t*)in;
	int i=0;
    for(i=0;((i>>3)+8)<n;i++){
        c[i]=d[i];
    }
	for(;i<n;i++){
		a[i]=b[i];
	}

    return out;
}
*/
int memcmp(const void *s1, const void *s2, size_t n) {
        char *a=(char*)s1;
        char *b=(char*)s2;
	while(*a==*b&&n-->0){
		a++;b++;
	}
	if(n==0){
          return 0;
        }
        if(*a>*b){
	  return 1;
	}else if(*a==*b){
	  return 0;
	}else{
	  return -1;
	}
    }
#endif
