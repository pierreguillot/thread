/*
// Copyright (c) 2015 Pierre Guillot.
// For information on usage and redistribution, and for a DISCLAIMER OF ALL
// WARRANTIES, see the file, "LICENSE.txt," in this distribution.
*/

#include <pthread.h>
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
    pthread_mutex_t mutex;          //!< The mutex.
    pthread_cond_t  condwrite;      //!< The condition.
    pthread_cond_t  condread;       //!< The condition.
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
    pthread_mutex_lock(&t->mutex);
    //! Wait the condition to write the data
    verbose_printf("func_producer wait...\n");
    while(t->occupied)
    {
        pthread_cond_wait(&t->condwrite, &t->mutex);
    }
    assert(!t->occupied);
    verbose_printf("func_producer run...\n");
    //! Write to the buffer.
    for(i = 0; i < BUFSIZE; i++)
    {
        t->buffer[i] = (char)i;
    }
    t->occupied = NCONSUMER;
    //! Signal that the buffer can be read
    verbose_printf("func_producer signal\n");
    pthread_cond_signal(&t->condread);
    //! Unlocks the access to the data
    pthread_mutex_unlock(&t->mutex);
}

//! @brief The function that reads from the buffer.
//! @details The function uses the mutex to ensure the synchronization of the access to the
//! data and uses condition to to ensure that all the producer has write the data.
static void func_consumer(t_data* t)
{
    size_t i;
    //! Locks the access to the data
    pthread_mutex_lock(&t->mutex);
    //! Wait the condition to write the data
    verbose_printf("func_consumer wait...\n");
    while(!t->occupied)
    {
        pthread_cond_wait(&t->condread, &t->mutex);
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
        pthread_cond_signal(&t->condread);
    }
    else
    {
        //! Signal that the buffer can be write
        pthread_cond_signal(&t->condwrite);
    }
    //! Unlocks the access to the data
    pthread_mutex_unlock(&t->mutex);
}

int main(int argc, char** argv)
{
    size_t i, j;
    //! The data structure that will be accessed by the threads
    t_data data;
    //! The set of consumer threads
    pthread_t  producer;
    pthread_t  consumers[NCONSUMER];
    //! Note that the data is free to be filled
    data.occupied = 0;
    printf("test thread... ");
    
    //! Initializes the mutex of the data structure
    pthread_mutex_init(&data.mutex, NULL);
    //! Initializes the conditions of the data structure
    pthread_cond_init(&data.condread, NULL);
    pthread_cond_init(&data.condwrite, NULL);
    
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
            pthread_create(consumers+i, 0, (void *)func_consumer, &data);
        }
        pthread_create(&producer, 0, (void *)func_producer, &data);
        
        //! Joins all the threads
        pthread_join(producer, NULL);
        for(i = 0; i < NCONSUMER; i++)
        {
            pthread_join(consumers[i], NULL);
        }        
    }
    
    //! Destroy the conditions of the data structure
    pthread_cond_destroy(&data.condread);
    pthread_cond_destroy(&data.condwrite);
    //! Destroy the mutex of the data structure
    pthread_mutex_destroy(&data.mutex);
    
    printf("ok\n");
    return 0;
}


