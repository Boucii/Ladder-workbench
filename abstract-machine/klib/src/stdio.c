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
  panic("Not implemented");
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  panic("Not implemented");
}

int sprintf(char *out, const char *fmt, ...) {
    int count=0;
    char buf[65];
    char digit[16];
    memset(buf,0,sizeof(buf));
    memset(digit,0,sizeof(digit));

    char *str=out;

    va_list ap;
    va_start(ap, fmt);

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
	    } else
        {
            *str = *fmt;
            str++;
        }
        fmt++;
	}
    va_end(ap);
    return count;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
