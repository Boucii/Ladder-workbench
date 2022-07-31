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

void xtoa(unsigned int n, char * buf)
{
        int i;
        
        if(n < 16)
        {
                if(n < 10)
                {
                        buf[0] = n + '0';
                }
                else
                {
                        buf[0] = n - 10 + 'a';
                }
                buf[1] = '\0';
                return;
        }
        xtoa(n / 16, buf);
        
        for(i = 0; buf[i] != '\0'; i++);
        
        if((n % 16) < 10)
        {
                buf[i] = (n % 16) + '0';
        }
        else
        {
                buf[i] = (n % 16) - 10 + 'a';
        }
        buf[i + 1] = '\0';
}
int isDigit(unsigned char c)
{
    if (c >= '0' && c <= '9')
        return 1;
    else
        return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
	int count=0;
        char buf[65];
        char digit[16];


        int len=0;
	int index = 0;
        int ret = 2;
        int num = 0;
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
                        case '0': /*位不足的左补0*/
                        {
                                index = 0;
                                num = 0;
                                memset(digit, 0, sizeof(digit));
                                
                                while(1)
                                {
                                        fmt++;
                                        ret = isDigit(*fmt);
                                        if(ret == 1) //是数字
                                        {
                                                digit[index] = *fmt;
                                                index++;
                                        }
                                        else
                                        {
                                                num = atoi(digit);
                                                break;
                                        }
                                }
                                switch(*fmt)
                             {
                                        case 'd': /*整型*/
                                        {
                                                n = va_arg(ap, int);
                                                if(n < 0)
                                                {
                                                    *str = '-';
                                                    str++;
                                                    n = -n;
                                                }    
                                                itoa(n, buf);
                                                len = strlen(buf);
                                                if(len >= num)
                                                {
                                                        memcpy(str, buf, strlen(buf));
                                                        str += strlen(buf);
                                                }
                                                else
                                                {
                                                        memset(str, '0', num-len);
                                                        str += num-len;
                                                        memcpy(str, buf, strlen(buf));
                                                        str += strlen(buf);
                                                }
                                                break;
                                        }    
                                        case 'x': /*16进制*/
                                        {
                                                n = va_arg(ap, int);
                                                xtoa(n, buf);
                                                len = strlen(buf);
                                                if(len >= num)
                                                {
                                                        memcpy(str, buf, len);
                                                        str += len;
                                                }            
                                                else
                                                {
                                                        memset(str, '0', num-len);
                                                        str += num-len;
                                                        memcpy(str, buf, len);
                                                        str += len;
                                                }
                                                break;
                                        }
                                        case 's': /*字符串*/
                                        {
                                                s = va_arg(ap, char *);
                                                len = strlen(s);
                                                if(len >= num)
                                                {
                                                        memcpy(str, s, strlen(s));
                                                        str += strlen(s);
                                                }
                                                else
                                                {
                                                        memset(str, '0', num-len);
                                                        str += num-len;
                                                        memcpy(str, s, strlen(s));
                                                        str += strlen(s);
                                                }
                                                break;
                                        }
                                        default:
                                                break;
                                }
                }
                                        default:
                                                break;
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

int printf(const char *fmt, ...) {
  char buf1[65536];
  memset(buf1,0,sizeof(buf1));
  va_list ap;
  va_start(ap,fmt);
  int res=vsprintf(buf,fmt,ap);
  va_end(ap);
  putstr(buf1);
  free(buf1);
  return res;

}

int sprintf(char *out, const char *fmt, ...) {
  char buf2[65536];
  memset(buf2,0,sizeof(buf2));
     va_list ap;
     va_start(ap, fmt);
     int res=vsprintf(out,fmt,ap);
     va_end(ap);
     free(buf);
     return res;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
