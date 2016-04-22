/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "../src/thd.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define NLOOPS 100

static thd_mutex mutex;
static size_t increment;

static void test_method()
{
    thd_mutex_lock(&mutex);
    size_t temp = increment;
    increment = temp + 1;
    printf("%i ", (int)increment);
    thd_mutex_unlock(&mutex);
}

int main(int argc, char** argv)
{
    size_t i, j;
    thd_thread threads[NLOOPS];
    printf("test thread...");
    thd_mutex_init(&mutex);
    
    for(j = 0; j < NLOOPS; j++)
    {
        increment = 0;
        for(i = 0; i < NLOOPS; i++)
        {
            thd_thread_launch(threads+i, (thd_thread_method)test_method, NULL);
        }
        
        for(i = 0; i < NLOOPS; i++)
        {
            thd_thread_join(threads+i);
        }
        if(increment != NLOOPS)
            printf("error ");
    }
    
    thd_mutex_delete(&mutex);
    
    printf("ok\n");
    return 0;
}