/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "../src/thd.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>

#define NLOOPS 100

static thd_mutex mutex;
static size_t increment;

static void test_method()
{
    thd_mutex_lock(&mutex);
    increment++;
    thd_mutex_unlock(&mutex);
}

int main(int argc, char** argv)
{
    size_t i;
    thd_thread threads[NLOOPS];
    printf("test thread...");
    thd_mutex_init(&mutex);
    
    for(i = 0; i < NLOOPS; i++)
    {
        thd_thread_launch(threads+i, (thd_thread_method)test_method, NULL);
    }
    
    for(i = 0; i < NLOOPS; i++)
    {
        thd_thread_join(threads+i);
    }
    thd_mutex_delete(&mutex);
    assert(i == NLOOPS);
    printf("ok\n");
    return 0;
}