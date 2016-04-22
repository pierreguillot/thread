/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#ifndef THD_MUTEX_H
#define THD_MUTEX_H

#include "thd_def.h"

//! @brief The mutex.
#ifdef _WIN32
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

#endif // THD_MUTEX_H
