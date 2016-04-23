/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef PTHREAD_H
#define PTHREAD_H

#include "../src/thd.h"

#define pthread_mutex_t thd_mutex;
#define pthread_cond_t thd_condition;
#define pthread_t thd_thread;
#define pthread_attr_t int;
#define PTHREAD_CREATE_JOINABLE 1
#define PTHREAD_CREATE_DETACHED 2

#define pthread_attr_init(attr) *attr=0
#define pthread_attr_setdetachstate(attr, flag) *attr=flag
#define pthread_create(t, attr, m, data) thd_thread_detach(t, m, data)
#define pthread_attr_destroy(attr) *attr=0
#define pthread_join(t, attr) (attr); thd_thread_join(&t)

#define pthread_mutex_init(mutex, attr) thd_mutex_init(mutex)
#define pthread_mutex_lock(mutex) thd_mutex_lock(mutex)
#define pthread_mutex_unlock(mutex) thd_mutex_unlock(mutex)
#define pthread_mutex_destroy(mutex) thd_mutex_destroy(mutex)


#define pthread_cond_init(cond, attr) thd_condition_init(cond)
#define pthread_cond_signal(cond) thd_condition_signal(cond)
#define pthread_cond_wait(cond) thd_condition_wait(cond)
#define pthread_cond_destroy(cond) thd_condition_destroy(cond)


#endif // PTHREAD_H

