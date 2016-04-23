### THD
A lightweight cross platform library for thread management

[![Build Status](https://travis-ci.org/pierreguillot/thread.svg?branch=master)](https://travis-ci.org/pierreguillot/thread)  
[![Build Status](https://ci.appveyor.com/api/projects/status/github/pierreguillot/thread?branch=master&svg=true)](https://ci.appveyor.com/project/pierreguillot/thread/branch/master)   
[![Coverage Status](https://coveralls.io/repos/github/pierreguillot/thread/badge.svg?branch=master)](https://coveralls.io/github/pierreguillot/thread?branch=master)

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
* [thd interface](https://github.com/pierreguillot/thread/blob/master/test/test.c)
* [pthread interface](https://github.com/pierreguillot/thread/blob/master/test/ptest.c)

*Note: On Windows, you need to add pthreadwin folder in the header search path to use the pthread interface.
