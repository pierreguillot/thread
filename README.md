### THD
A lightweight cross platform library for thread management

[![Build Status](https://travis-ci.org/pierreguillot/thread.svg?branch=master)](https://travis-ci.org/pierreguillot/thread)  
[![Build Status](https://ci.appveyor.com/api/projects/status/github/pierreguillot/thread?branch=master&svg=true)](https://ci.appveyor.com/project/pierreguillot/thread/branch/master)  

#### Platforms
* Linux
* MacOS
* Windows  

#### Installation
```
mkdir build
cd build
cmake ..  
cmake --build .
test (optional)
```

#### Documentation
* Thread   
 * Detaches a thread:  
void thd_thread_detach(thd_thread* thread, thd_thread_method method, void* data);

 * Joins a thread:  
void thd_thread_join(thd_thread* thread);

* Mutex
 * Initializes a mutex:   
void thd_mutex_init(thd_mutex* mutex);

 * Locks a mutex:  
void thd_mutex_lock(thd_mutex* mutex);

 * Unlocks a mutex:  
void thd_mutex_unlock(thd_mutex* mutex);

 * Destroy a mutex:  
void thd_mutex_destroy(thd_mutex* mutex);

* Condition
 * Initializes a condition:  
void thd_condition_init(thd_condition* cond);

 * Restarts one of the threads that are waiting on the condition:  
void thd_condition_signal(thd_condition* cond);

 * Unlocks the mutex and waits for the condition to be signalled:  
void thd_condition_wait(thd_condition* cond, thd_mutex* mutex);

 * Destroy a condition:
void thd_condition_destroy(thd_condition* cond);

#### Example
```c
#include "../src/thd.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


#define BUFSIZE 128
#define NCONSUMER 5
#define NLOOPS 10

static void verbose_printf(char const* buf)
{
#ifdef VERBOSE_TEST
    printf("%s", buf);
#endif
}

//! @brief A sample data structure for threads to use.
//! @details The structure is passed by void pointer to the thread so it can be any data type.
//! The structure owns a mutex to synchronize the access to the data.
typedef struct _data
{
    thd_mutex       mutex;          //!< The mutex.
    thd_condition   condwrite;      //!< The condition.
    thd_condition   condread;       //!< The condition.
    char            occupied;       //!< The state.
    char            buffer[BUFSIZE];//!< The buffer.
}t_data;

//! @brief The function that writes in the buffer.
//! @details The function uses the mutex to ensure the synchronization of the access to the
//! data and uses condition to to ensure that all the consumer has read the data.
static void func_producer(t_data* t)
{
    size_t i;
    //! Locks the access to the data
    thd_mutex_lock(&t->mutex);
    //! Wait the condition to write the data
    verbose_printf("func_producer wait...\n");
    while(t->occupied)
    {
        thd_condition_wait(&t->condwrite, &t->mutex);
    }
    assert(!t->occupied);
    verbose_printf("func_producer run...\n");
    //! Write to the buffer.
    for(i = 0; i < BUFSIZE; i++)
    {
        t->buffer[i] = i;
    }
    t->occupied = NCONSUMER;
    //! Signal that the buffer can be read
    verbose_printf("func_producer signal\n");
    thd_condition_signal(&t->condread);
    //! Unlocks the access to the data
    thd_mutex_unlock(&t->mutex);
}

//! @brief The function that reads from the buffer.
//! @details The function uses the mutex to ensure the synchronization of the access to the
//! data and uses condition to to ensure that all the producer has write the data.
static void func_consumer(t_data* t)
{
    size_t i;
    //! Locks the access to the data
    thd_mutex_lock(&t->mutex);
    //! Wait the condition to write the data
    verbose_printf("func_consumer wait...\n");
    while(!t->occupied)
    {
        thd_condition_wait(&t->condread, &t->mutex);
    }
    assert(t->occupied);
    verbose_printf("func_consumer run...\n");
    //! Write to the buffer.
    for(i = 0; i < BUFSIZE; i++)
    {
        assert(t->buffer[i] == i);
    }
    t->occupied--;
    verbose_printf("func_consumer signal\n");
    if(t->occupied)
    {
        //! Signal that the buffer can be read by another thread
        thd_condition_signal(&t->condread);
    }
    else
    {
        //! Signal that the buffer can be write
        thd_condition_signal(&t->condwrite);
    }
    //! Unlocks the access to the data
    thd_mutex_unlock(&t->mutex);
}

int main(int argc, char** argv)
{
    size_t i, j;
    //! The data structure that will be accessed by the threads
    t_data data;
    //! The set of consumer threads
    thd_thread  producer;
    thd_thread  consumers[NCONSUMER];
    //! Note that the data is free to be filled
    data.occupied = 0;
    printf("test thread... ");

    //! Initializes the mutex of the data structure
    thd_mutex_init(&data.mutex);
    //! Initializes the conditions of the data structure
    thd_condition_init(&data.condread);
    thd_condition_init(&data.condwrite);

    for(j = 0; j < NLOOPS; j++)
    {
        //! Fill the data's buffer with 0
        for(i = 0; i < BUFSIZE; i++)
        {
            data.buffer[i] = 0;
        }

        //! Detaches all the threads
        for(i = 0; i < NCONSUMER; i++)
        {
            thd_thread_detach(consumers+i, (thd_thread_method)func_consumer, &data);
        }
        thd_thread_detach(&producer, (thd_thread_method)func_producer, &data);

        //! Joins all the threads
        thd_thread_join(&producer);
        for(i = 0; i < NCONSUMER; i++)
        {
            thd_thread_join(consumers+i);
        }        
    }

    //! Destroy the conditions of the data structure
    thd_condition_destroy(&data.condread);
    thd_condition_destroy(&data.condwrite);
    //! Destroy the mutex of the data structure
    thd_mutex_destroy(&data.mutex);

    printf("ok\n");
    return 0;
}
```
