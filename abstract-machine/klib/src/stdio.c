#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
void itoa(unsigned int n, char * buf)
{
        int i;
        
        if(n < 10)
        {
                buf[0] = n + '0';
                buf[1] = '\0';
                return;
        }
        itoa(n / 10, buf);

        for(i=0; buf[i]!='\0'; i++);
        
        buf[i] = (n % 10) + '0';
        
        buf[i+1] = '\0';
}
int printf(const char *fmt, ...) {
  char buf[65536];
  va_list args;
  va_start(args,fmt);
  int res=vsprintf(buf,fmt,args);
  va_end(args);
  putstr(buf);
  free(buf);
  return res;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
	int count=0;
        char buf[65];
        char digit[16];
        memset(buf,0,sizeof(buf));
        memset(digit,0,sizeof(digit));
                                                                
        char *str=out;
                                                                
        int n;
        char c;
        char *s;
            while(*fmt!='\0'){
                if(*fmt=='%'){
                    fmt++;
            	switch(*fmt){
            		case 'd':{
            		    n=va_arg(ap,int);
        			    if(n < 0)
                            {
                                *str = '-';
                                str++;
                                n = -n;
            		    count++;
                            }
            		    itoa(n,buf);
            		     memcpy(str, buf, strlen(buf));
            		     str += strlen(buf);
            		     count+=strlen(buf);
            		     break;
            			 }
            		case 'c':{
            	 	c = va_arg(ap, int);
                            *str = c;
                            str++;
                            break;
            			 }
            		case 's':{
                            s = va_arg(ap, char *);
                            memcpy(str, s, strlen(s));
                            str += strlen(s);
            		count+=strlen(s);
                            break;			 
            			 }
                    }
            }else{
            	    *str = *fmt;
                        str++;
            }
            fmt++;
            }
                                                                
        *str='\0';
        return count;
}

int sprintf(char *out, const char *fmt, ...) {
     va_list ap;
     va_start(ap, fmt);
     int res=vsprintf(out,fmt,ap);
     va_end(ap);
     return res;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
