/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef THD_THREAD_H
#define THD_THREAD_H

#include "thd_def.h"

//! @brief The thread method without any parameter.
typedef void (*thd_thread_method)();
//! @brief The thread method with a pointer as parameter.
typedef void (*thd_thread_method_ptr)(void *);

//! @brief The thread.
#ifdef _WIN32
typedef HANDLE thd_thread;
#else
typedef pthread_t thd_thread;
#endif


//! @brief Lauches a thread.
CPD_EXTERN void thd_thread_launch(thd_thread* thread, thd_thread_method method, void* data);

//! @brief Joins a thread.
CPD_EXTERN void thd_thread_join(thd_thread* thread);

#endif // THD_THREAD_H
