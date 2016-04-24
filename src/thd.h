/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef THD_H
#define THD_H

#ifdef _WIN32
#define THD_WINDOWS_NATIVE
#include <windows.h>
#else
#include <pthread.h>
#endif

#ifdef THD_LIB_EXPORT
#ifdef THD_WINDOWS_NATIVE
#define THD_EXTERN __declspec(dllexport) extern
#else
#define THD_EXTERN extern
#endif
#elif THD_LIB_IMPORY
#ifdef THD_WINDOWS_NATIVE
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
#ifdef THD_WINDOWS_NATIVE
typedef HANDLE thd_thread;
#else
typedef pthread_t thd_thread;
#endif


//! @brief Detaches a thread.
THD_EXTERN int thd_thread_detach(thd_thread* thread, thd_thread_method method, void* data);

//! @brief Joins a thread.
THD_EXTERN int thd_thread_join(thd_thread* thread);







//! @brief The mutex.
#ifdef THD_WINDOWS_NATIVE
typedef CRITICAL_SECTION thd_mutex;
#else
typedef pthread_mutex_t thd_mutex;
#endif


//! @brief Initializes a mutex.
THD_EXTERN int thd_mutex_init(thd_mutex* mutex);

//! @brief Locks a mutex.
THD_EXTERN int thd_mutex_lock(thd_mutex* mutex);

//! @brief Tries to locks a mutex.
THD_EXTERN int thd_mutex_trylock(thd_mutex* mutex);

//! @brief Unlocks a mutex.
THD_EXTERN int thd_mutex_unlock(thd_mutex* mutex);

//! @brief Destroy a mutex.
THD_EXTERN int thd_mutex_destroy(thd_mutex* mutex);





//! @brief The condition.
#ifdef THD_WINDOWS_NATIVE
typedef CONDITION_VARIABLE thd_condition;
#else
typedef pthread_cond_t thd_condition;
#endif


//! @brief Initializes a condition.
THD_EXTERN int thd_condition_init(thd_condition* cond);

//! @brief Restarts one of the threads that are waiting on the condition.
THD_EXTERN int thd_condition_signal(thd_condition* cond);

//! @brief Unlocks the mutex and waits for the condition to be signalled.
THD_EXTERN int thd_condition_wait(thd_condition* cond, thd_mutex* mutex);

//! @brief Destroy a condition.
THD_EXTERN int thd_condition_destroy(thd_condition* cond);



#endif // THD_H

