/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "thd.h"

#ifdef THD_WINDOWS_NATIVE

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
    params->i_method(params->i_data);
    free(params);
    return 0;
}

int thd_thread_detach(thd_thread* thread, thd_thread_method method, void* data)
{
    t_internal_parameters* params = (t_internal_parameters *)malloc(sizeof(t_internal_parameters));
    if(params)
    {
        params->i_method = method;
        params->i_data   = data;
        *thread = CreateThread(NULL, 0, internal_method_ptr, params, 0, NULL);
        if(*thread == NULL)
        {
            free(params);
            return 1;
        }
        return 0;
    }
    return 1;
}

int thd_thread_join(thd_thread* thread)
{
    if(WaitForSingleObject(*thread, INFINITE) != WAIT_FAILED)
    {
        if(CloseHandle(*thread))
        {
            return 0;
        }
    }
    return 1;
}

int thd_mutex_init(thd_mutex* mutex)
{
    InitializeCriticalSection(mutex);
    return 0;
}

int thd_mutex_lock(thd_mutex* mutex)
{
    EnterCriticalSection(mutex);
    return 0;
}

int thd_mutex_trylock(thd_mutex* mutex)
{
    return !TryEnterCriticalSection(mutex);
}

int thd_mutex_unlock(thd_mutex* mutex)
{
    LeaveCriticalSection(mutex);
    return 0;
}

int thd_mutex_destroy(thd_mutex* mutex)
{
    DeleteCriticalSection(mutex);
    return 0;
}

int thd_condition_init(thd_condition* cond)
{
    InitializeConditionVariable(cond);
    return 0;
}

int thd_condition_signal(thd_condition* cond)
{
    WakeConditionVariable(cond);
    return 0;
}

int thd_condition_wait(thd_condition* cond, thd_mutex* mutex)
{
    return !SleepConditionVariableCS(cond, mutex, INFINITE);
}

int thd_condition_destroy(thd_condition* cond)
{
    return 0;
}

#else

int thd_thread_detach(thd_thread* thread, thd_thread_method method, void* data)
{
    return pthread_create(thread, 0, (void *)method, data);
}

int thd_thread_join(thd_thread* thread)
{
    return pthread_join(*thread, NULL);
}

int thd_mutex_init(thd_mutex* mutex)
{
    return pthread_mutex_init(mutex, NULL);
}

int thd_mutex_lock(thd_mutex* mutex)
{
    return pthread_mutex_lock(mutex);
}

int thd_mutex_trylock(thd_mutex* mutex)
{
    return pthread_mutex_trylock(mutex);
}

int thd_mutex_unlock(thd_mutex* mutex)
{
    return pthread_mutex_unlock(mutex);
}

int thd_mutex_destroy(thd_mutex* mutex)
{
    return pthread_mutex_destroy(mutex);
}

int thd_condition_init(thd_condition* cond)
{
    return pthread_cond_init(cond, NULL);
}

int thd_condition_signal(thd_condition* cond)
{
    return pthread_cond_signal(cond);
}

int thd_condition_wait(thd_condition* cond, thd_mutex* mutex)
{
    return pthread_cond_wait(cond, mutex);
}

int thd_condition_destroy(thd_condition* cond)
{
    return pthread_cond_destroy(cond);
}

#endif