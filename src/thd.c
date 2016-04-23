/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "thd.h"

#ifdef WINDOWS_NATIVE

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef struct _internal_parameters
{
    thd_thread_method i_method;
    void*             i_data;
}t_internal_parameters;

static DWORD WINAPI internal_method_ptr(LPVOID arg)
{
    t_internal_parameters *params = (t_internal_parameters *)arg;
    ((thd_thread_method_ptr)params->i_method)(params->i_data);
    free(params);
    return 0;
}

void thd_thread_detach(thd_thread* thread, thd_thread_method method, void* data)
{
    t_internal_parameters* params = (t_internal_parameters *)malloc(sizeof(t_internal_parameters));
    params->i_method = method;
    params->i_data   = data;
    *thread = CreateThread(NULL, 0, internal_method_ptr, params, 0, NULL);
}

void thd_thread_join(thd_thread* thread)
{
    WaitForSingleObject(*thread, INFINITE);
    CloseHandle(*thread);
}

void thd_mutex_init(thd_mutex* mutex)
{
    InitializeCriticalSection(mutex);
}

void thd_mutex_lock(thd_mutex* mutex)
{
    EnterCriticalSection(mutex);
}

void thd_mutex_unlock(thd_mutex* mutex)
{
    LeaveCriticalSection(mutex);
}

void thd_mutex_destroy(thd_mutex* mutex)
{
    DeleteCriticalSection(mutex);
}

void thd_condition_init(thd_condition* cond)
{
    InitializeConditionVariable(cond);
}

void thd_condition_signal(thd_condition* cond)
{
    WakeConditionVariable(cond);
}

void thd_condition_wait(thd_condition* cond, thd_mutex* mutex)
{
    SleepConditionVariableCS(cond, mutex, INFINITE);
}

void thd_condition_destroy(thd_condition* cond)
{
    ;
}

#else

void thd_thread_detach(thd_thread* thread, thd_thread_method method, void* data)
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

void thd_mutex_init(thd_mutex* mutex)
{
    pthread_mutex_init(mutex, NULL);
}

void thd_mutex_lock(thd_mutex* mutex)
{
    pthread_mutex_lock(mutex);
}

void thd_mutex_unlock(thd_mutex* mutex)
{
    pthread_mutex_unlock(mutex);
}

void thd_mutex_destroy(thd_mutex* mutex)
{
    pthread_mutex_destroy(mutex);
}

void thd_condition_init(thd_condition* cond)
{
    pthread_cond_init(cond, NULL);
}

void thd_condition_signal(thd_condition* cond)
{
    pthread_cond_signal(cond);
}

void thd_condition_wait(thd_condition* cond, thd_mutex* mutex)
{
    pthread_cond_wait(cond, mutex);
}

void thd_condition_destroy(thd_condition* cond)
{
    pthread_cond_destroy(cond);
}

#endif