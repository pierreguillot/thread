/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "thd.h"

#ifdef _WIN32

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <tchar.h>
#include <strsafe.h>

#define BUF_SIZE 255
typedef struct _internal_parameters
{
    thd_thread_method i_method;
    void*             i_data;
}t_internal_parameters;

typedef void (*thd_thread_method_ptr)(void *);

static DWORD WINAPI internal_method_null(LPVOID arg)
{
    HANDLE hStdout;
    TCHAR msgBuf[BUF_SIZE];
    size_t cchStringSize;
    DWORD dwChars;
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    StringCchPrintf(msgBuf, BUF_SIZE, TEXT("internal_method_null\n"));
    StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);
    WriteConsole(hStdout, msgBuf, (DWORD)cchStringSize, &dwChars, NULL);
    
    printf("internal_method_null\n");
    t_internal_parameters *params = (t_internal_parameters *)arg;
    params->i_method();
    free(params);
    return 0;
}

static DWORD WINAPI internal_method_ptr(LPVOID arg)
{
    t_internal_parameters *params = (t_internal_parameters *)arg;
    ((thd_thread_method_ptr)params->i_method)(params->i_data);
    free(params);
    return 0;
}

void thd_thread_launch(thd_thread* thread, thd_thread_method method, void* data)
{
    t_internal_parameters* params = (t_internal_parameters *)malloc(sizeof(t_internal_parameters));
    params->i_method = method;
    params->i_data   = data;
    if(data)
    {
        printf("call internal_method_ptr\n");
        thread = CreateThread(NULL, 0, internal_method_ptr, params, 0, NULL);
    }
    else
    {
        printf("call internal_method_null\n");
        thread = CreateThread(NULL, 0, internal_method_null, params, 0, NULL);
    }
}

void thd_thread_join(thd_thread* thread)
{
    WaitForSingleObject(thread, INFINITE);
    CloseHandle(thread);
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

void thd_mutex_delete(thd_mutex* mutex)
{
    DeleteCriticalSection(mutex);
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

void thd_mutex_delete(thd_mutex* mutex)
{
    pthread_mutex_destroy(mutex);
}

#endif