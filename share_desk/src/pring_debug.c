#include "source.h"
void print_debug (const char *func,size_t line,const char *fmt, ...)
{
    static int count=0;
    char buf[4096]={0};
    va_list ap;
    va_start (ap, fmt);
    vsnprintf(buf,4096,fmt,ap);
    va_end (ap);
    printf("[%d %s %d] ",count++,func,line);
    printf("%s",buf);
    return;
}