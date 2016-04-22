/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef THREAD_MUTEX_H
#define THREAD_MUTEX_H

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif

#ifdef _WIN32
#ifdef PD_INTERNAL
#define CPD_EXTERN __declspec(dllexport) extern
#else
#define CPD_EXTERN __declspec(dllimport) extern
#endif
#else
#define CPD_EXTERN extern
#endif

#if defined(_MSC_VER) && !defined(_LANGUAGE_C_PLUS_PLUS) && !defined(__cplusplus)
#define CPD_EXTERN_STRUCT extern struct
#else
#define CPD_EXTERN_STRUCT struct
#endif

//! @brief The mutex.
#ifdef _WIN32
typedef CRITICAL_SECTION thd_mutex;
#else
typedef pthread_mutex_t thd_mutex;
#endif


//! @brief Initializes a mutex.
CPD_EXTERN void thd_mutex_init(thd_mutex* mutex);

//! @brief Locks a mutex.
CPD_EXTERN void thd_mutex_lock(thd_mutex* mutex);

//! @brief Unlocks a mutex.
CPD_EXTERN void thd_mutex_unlock(thd_mutex* mutex);

//! @brief Deletes a mutex.
CPD_EXTERN void thd_mutex_delete(thd_mutex* mutex);

#endif // THREAD_MUTEX_H
