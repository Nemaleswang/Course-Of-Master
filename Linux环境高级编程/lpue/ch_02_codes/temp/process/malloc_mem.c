/*
 * =====================================================================================
 *
 *       Filename:  malloc_mem.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  09/27/11 14:11:07
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *        Company:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>

int main(void)
{
    int total = 0;
    while(++total)
    {
        if ( NULL != malloc(1024) ){
//            if ( total / (10*1024) == 0 )
//                printf("alloced %d Kbytes\n", total);
        }else
            printf("alloced mem error\n");
    }
}

