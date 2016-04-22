/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "thd_thread.h"

#ifdef _WIN32

void thd_thread_launch(thd_thread* thread, thd_thread_method method, void* data)
{
 
}

void thd_thread_join(thd_thread* thread)
{
    //WaitForSingleObject(thread, INFINITE);
}

#else

void thd_thread_launch(thd_thread* thread, thd_thread_method method, void* data)
{
    pthread_attr_t attributes;
    pthread_attr_init(&attributes);
    pthread_attr_setdetachstate(&attributes, PTHREAD_CREATE_JOINABLE);
    pthread_create(thread, &attributes, (void *)method, data);
    pthread_attr_destroy(&attributes);
}

void thd_thread_join(thd_thread* thread)
{
    pthread_join(*thread, NULL);
}

#endif