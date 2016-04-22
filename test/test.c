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


#define TESTNLOOPS 100
#define TESTNTHDS 10

static thd_mutex mutex;
static size_t increment;

static void test_method()
{
    size_t temp;
    thd_mutex_lock(&mutex);
    temp = increment;
    increment = temp + 1;
    thd_mutex_unlock(&mutex);
}

int main(int argc, char** argv)
{
    size_t i, j;
    thd_thread threads[TESTNTHDS];
    printf("test thread...");
    thd_mutex_init(&mutex);
    
    for(j = 0; j < TESTNLOOPS; j++)
    {
        increment = 0;
        for(i = 0; i < TESTNTHDS; i++)
        {
            thd_thread_launch(threads+i, (thd_thread_method)test_method, NULL);
        }
        
        for(i = 0; i < TESTNTHDS; i++)
        {
            thd_thread_join(threads+i);
        }
        assert(increment == TESTNTHDS);
    }
    
    thd_mutex_delete(&mutex);
    
    printf("ok\n");
    return 0;
}


