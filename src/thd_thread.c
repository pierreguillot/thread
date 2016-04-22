/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "thd_thread.h"

#ifdef _WIN32

struct internal_parameters
{
    thd_thread_method i_method;
    void*             i_data;
};

typedef void (*thd_thread_method_ptr)(void *);

static DWORD WINAPI internal_method_null(LPVOID arg)
{
    internal_parameters *params = (internal_parameters *)arg;
    params->i_method();
    return 0;
}

static DWORD WINAPI internal_method_ptr(LPVOID arg)
{
    internal_parameters *params = (internal_parameters *)arg;
    ((thd_thread_method_ptr)params->i_method)(params->i_data);
    return 0;
}

void thd_thread_launch(thd_thread* thread, thd_thread_method method, void* data)
{
    internal_parameters params;
    params.i_method = method;
    params.i_data   = data;
    if(data)
    {
        thread = CreateThread(NULL, 0, internal_method, &params, 0, NULL);
    }
    else
    {
        thread = CreateThread(NULL, 0, internal_method, &params, 0, NULL);
    }
}

void thd_thread_join(thd_thread* thread)
{
    WaitForSingleObject(thread, INFINITE);
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