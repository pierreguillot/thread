/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

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
        assert(t->buffer[i] == (char)i);
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


