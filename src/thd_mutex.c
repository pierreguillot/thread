/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include "thd_mutex.h"

#ifdef _WIN32

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

