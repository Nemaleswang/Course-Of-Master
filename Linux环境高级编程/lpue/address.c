//readelf -h  ./address 


#include <stdio.h>
#include <stdlib.h>

int bss_var;

int data_var0 = 1;

int main()

{

printf("Test location:\n");

printf("\tAddress of main(Code Segment)：%p\n",main);

printf("_____________________________________\n");

int stack_var0 = 2;

printf("Stack location:\n");

printf("\tInitial end of stack:%p\n",&stack_var0);

int stack_var1 = 3;

printf("\tNew end of stack:%p\n",&stack_var1);

printf("_____________________________________\n");

printf("Data location:\n");

printf("\tAddress of data_var(Data Segment)：%p\n",&data_var0);

static int data_var1 = 4;

printf("\tNew end of data_var(Data Segment)：%p\n",&data_var1);

printf("_____________________________________\n");

printf("BSS location:\n");

printf("\tAddress of bss_var:%p\n",&bss_var);

printf("_____________________________________\n");

printf("Heap location:\n");

char *p = (char *)malloc(10);

printf("\tAddress of head_var:%p\n",p);

return 0;

}
