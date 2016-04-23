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

typedef struct _test
{
    thd_mutex mutex;
    size_t increment;
}t_test;

static void test_call(t_test* t)
{
    size_t temp;
    thd_mutex_lock(&t->mutex);
    temp = t->increment;
    t->increment = temp + 1;
    thd_mutex_unlock(&t->mutex);
}

int main(int argc, char** argv)
{
    size_t i, j;
    t_test data;
    thd_thread  threads[TESTNTHDS];
    printf("test thread... ");
    thd_mutex_init(&data.mutex);
    for(j = 0; j < TESTNLOOPS; j++)
    {
        data.increment = 0;
        for(i = 0; i < TESTNTHDS; i++)
        {
            thd_thread_detach(threads+i, (thd_thread_method)test_call, &data);
        }
        
        for(i = 0; i < TESTNTHDS; i++)
        {
            thd_thread_join(threads+i);
        }
        assert(data.increment == TESTNTHDS);
    }
    thd_mutex_delete(&data.mutex);
    
    printf("ok\n");
    return 0;
}


