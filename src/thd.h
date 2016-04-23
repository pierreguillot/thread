/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef THD_H
#define THD_H

#ifdef _WIN32
#define WINDOWS_NATIVE
#include <windows.h>
#else
#include <pthread.h>
#endif

#ifdef THD_LIB_EXPORT
#ifdef WINDOWS_NATIVE
#define THD_EXTERN __declspec(dllexport) extern
#else
#define THD_EXTERN extern
#endif
#elif THD_LIB_IMPORY
#ifdef WINDOWS_NATIVE
#define THD_EXTERN __declspec(dllimport) extern
#else
#define THD_EXTERN extern
#endif
#else
#define THD_EXTERN
#endif

//! @brief The thread method.
typedef void (*thd_thread_method)(void *);

//! @brief The thread.
#ifdef WINDOWS_NATIVE
typedef HANDLE thd_thread;
#else
typedef pthread_t thd_thread;
#endif


//! @brief Lauches a thread.
THD_EXTERN void thd_thread_launch(thd_thread* thread, thd_thread_method method, void* data);

//! @brief Joins a thread.
THD_EXTERN void thd_thread_join(thd_thread* thread);


//! @brief The mutex.
#ifdef WINDOWS_NATIVE
typedef CRITICAL_SECTION thd_mutex;
#else
typedef pthread_mutex_t thd_mutex;
#endif


//! @brief Initializes a mutex.
THD_EXTERN void thd_mutex_init(thd_mutex* mutex);

//! @brief Locks a mutex.
THD_EXTERN void thd_mutex_lock(thd_mutex* mutex);

//! @brief Unlocks a mutex.
THD_EXTERN void thd_mutex_unlock(thd_mutex* mutex);

//! @brief Deletes a mutex.
THD_EXTERN void thd_mutex_delete(thd_mutex* mutex);


#endif // THD_H

