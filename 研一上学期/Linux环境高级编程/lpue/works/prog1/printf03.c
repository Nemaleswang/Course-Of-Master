/*
 * =====================================================================================
 *
 *       Filename:  printf03.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/16/13 17:09:43
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:   (), 
 *        Company:  NDSL UESTC
 *
 * =====================================================================================
 */


#include <stdarg.h>
#include <string.h>
#include <unistd.h>

char *myitoa(int, char *, int);//把数字转换为字符
void myprintf(const char *format, ...) {

    char buf[256];
    char buf1[256];
    char buf2[2];
    va_list argptr;
    va_start(argptr, format);        //va_start         
    char ch;
    while (ch = *(format++))
    {
       if (ch == '%')
       {
           ch = *(format++);
           if (ch == 's')
           {
               char *name = va_arg(argptr, char *);    //va_arg
               //cout<<name;
               strcat(buf, name);
           }
           else if (ch == 'd')
           {
               int age = va_arg(argptr, int);    //va_arg
               //cout<<age;
               myitoa(age, buf1, 10);
               strcat(buf, buf1);
           }
       }
       else
       {
           //cout<<ch;
           *buf2 = ch;
           *(buf2 + 1) = '\0';
           strcat(buf, buf2);
       }
    }
    int n = strlen(buf);
    write(1, buf, n);
    va_end(argptr);        //va_end
}

int main()
{
    int age = 24;
    char *name = "ChengHua";
    myprintf("My name is %s, age %d.\n", name, age);
    return 0;
}


char *myitoa(int n, char* str, int radix) {
    int a = n;
    int b = 0;
    int c = 1;
    char *p = str;

    if (0 == a) {
        strcpy(str, "0");
        return str;
    }
    else if (a < 0) {
        c = -1;
        a = -a;
    }

    while(a > 0) {
        b = a % radix;
        a = a / radix;
        if (b < 10)
            *p = b + '0';
        else 
            *p = b - 10 + 'A';
    
        ++p;
    }

    if (c < 0) *p++ = '-';
    *p = 0;

    int len = strlen(str);
    int i = 0;
    char ch = 0;

    for (i = 0; i < len / 2; ++i) {
        c = str[i];
        str[i] = str[len - i -1];
        str[len - i -1] = c;
    }

    return str;
}





