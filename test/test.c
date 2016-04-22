/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "../src/thd_mutex.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    printf("test thread...");
    thd_mutex mutex;
    
    thd_mutex_init(&mutex);
    
    thd_mutex_lock(&mutex);
    
    thd_mutex_unlock(&mutex);
    
    thd_mutex_delete(&mutex);
    
    printf("ok\n");
    return 0;
}