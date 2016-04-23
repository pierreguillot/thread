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


#define TESTNLOOPS 100
#define TESTNTHDS 10

//! @brief A sample data structure for threads to use.
//! @details The structure is passed by void pointer to the thread so it can be any data type.
//! The structure owns a mutex to synchronize the access to the data.
typedef struct _data
{
    thd_mutex   mutex;  //!< The mutex.
    size_t      inc1;   //!< The first increment.
    size_t      inc2;   //!< The second increment.
}t_data;

//! @brief The function exceuted by the thread.
//! @details The function uses the mutex to ensure the synchronization of the access to
//! increment values of the data.
static void test_call(t_data* t)
{
    size_t temp1, temp2;
    //! Locks the access to the data
    thd_mutex_lock(&t->mutex);
    //! Stores the increment values in tempory variables.
    temp1   = t->inc1;
    temp2   = t->inc2;
    //! Assignes the increment values with the tempory variables.
    t->inc1 = temp1 + 1;
    t->inc2 = temp2 + 100;
    //! Unlocks the access to the data
    thd_mutex_unlock(&t->mutex);
}

int main(int argc, char** argv)
{
    size_t i, j;
    //! The data structure that will be accessed by the threads
    t_data data;
    //! The set of threads
    thd_thread  threads[TESTNTHDS];
    printf("test thread... ");
    
    //! Initializes the mutex of the data structure
    thd_mutex_init(&data.mutex);
    for(j = 0; j < TESTNLOOPS; j++)
    {
        data.inc1 = 0;
        data.inc2 = 0;
        //! Detaches all the threads
        for(i = 0; i < TESTNTHDS; i++)
        {
            thd_thread_detach(threads+i, (thd_thread_method)test_call, &data);
        }
        
        //! Joins all the threads
        for(i = 0; i < TESTNTHDS; i++)
        {
            thd_thread_join(threads+i);
        }
        assert(data.inc1 == TESTNTHDS);
        assert(data.inc2 == TESTNTHDS * 100);
    }
    //! Deletes the mutex of the data structure
    thd_mutex_delete(&data.mutex);
    
    printf("ok\n");
    return 0;
}


