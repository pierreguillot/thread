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

#ifdef _WIN32
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

#define MAX_THREADS 3
#define BUF_SIZE 255
static size_t increment;
static thd_mutex mutex;

DWORD WINAPI MyThreadFunction( LPVOID lpParam );
void ErrorHandler(LPTSTR lpszFunction);

// Sample custom data structure for threads to use.
// This is passed by void pointer so it can be any data type
// that can be passed using a single void pointer (LPVOID).
typedef struct MyData {
    int val1;
    int val2;
} MYDATA, *PMYDATA;


int _tmain()
{
    PMYDATA pDataArray[MAX_THREADS];
    DWORD   dwThreadIdArray[MAX_THREADS];
    HANDLE  hThreadArray[MAX_THREADS];
    
    // Create MAX_THREADS worker threads.
    
    for( int i=0; i<MAX_THREADS; i++ )
    {
        // Allocate memory for thread data.
        
        pDataArray[i] = (PMYDATA) HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY,
                                            sizeof(MYDATA));
        
        if( pDataArray[i] == NULL )
        {
            // If the array allocation fails, the system is out of memory
            // so there is no point in trying to print an error message.
            // Just terminate execution.
            ExitProcess(2);
        }
        
        // Generate unique data for each thread to work with.
        
        pDataArray[i]->val1 = i;
        pDataArray[i]->val2 = i+100;
        
        // Create the thread to begin execution on its own.
        
        hThreadArray[i] = CreateThread(
                                       NULL,                   // default security attributes
                                       0,                      // use default stack size
                                       MyThreadFunction,       // thread function name
                                       pDataArray[i],          // argument to thread function
                                       0,                      // use default creation flags
                                       &dwThreadIdArray[i]);   // returns the thread identifier
        
        
        // Check the return value for success.
        // If CreateThread fails, terminate execution.
        // This will automatically clean up threads and memory.
        
        if (hThreadArray[i] == NULL)
        {
            ErrorHandler(TEXT("CreateThread"));
            ExitProcess(3);
        }
    } // End of main thread creation loop.
    
    // Wait until all threads have terminated.
    
    WaitForMultipleObjects(MAX_THREADS, hThreadArray, TRUE, INFINITE);
    
    // Close all thread handles and free memory allocations.
    
    for(int i=0; i<MAX_THREADS; i++)
    {
        CloseHandle(hThreadArray[i]);
        if(pDataArray[i] != NULL)
        {
            HeapFree(GetProcessHeap(), 0, pDataArray[i]);
            pDataArray[i] = NULL;    // Ensure address is not reused.
        }
    }
    
    if(increment != MAX_THREADS)
        printf("error ");
    
    return 0;
}


DWORD WINAPI MyThreadFunction( LPVOID lpParam )
{
    size_t temp;
    HANDLE hStdout;
    PMYDATA pDataArray;
    
    TCHAR msgBuf[BUF_SIZE];
    size_t cchStringSize;
    DWORD dwChars;
    
    // Make sure there is a console to receive output results.
    
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if( hStdout == INVALID_HANDLE_VALUE )
        return 1;
    thd_mutex_lock(&mutex);
    temp = increment;
    increment = temp + 1;
    thd_mutex_unlock(&mutex);
    // Cast the parameter to the correct data type.
    // The pointer is known to be valid because
    // it was checked for NULL before the thread was created.
    
    pDataArray = (PMYDATA)lpParam;
    
    // Print the parameter values using thread-safe functions.
    
    StringCchPrintf(msgBuf, BUF_SIZE, TEXT("Parameters = %d, %d\n"),
                    pDataArray->val1, pDataArray->val2);
    StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);
    WriteConsole(hStdout, msgBuf, (DWORD)cchStringSize, &dwChars, NULL);
    
    return 0;
}



void ErrorHandler(LPTSTR lpszFunction)
{
    // Retrieve the system error message for the last-error code.
    
    LPVOID lpMsgBuf;
    LPVOID lpDisplayBuf;
    DWORD dw = GetLastError();
    
    FormatMessage(
                  FORMAT_MESSAGE_ALLOCATE_BUFFER |
                  FORMAT_MESSAGE_FROM_SYSTEM |
                  FORMAT_MESSAGE_IGNORE_INSERTS,
                  NULL,
                  dw,
                  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  (LPTSTR) &lpMsgBuf,
                  0, NULL );
    
    // Display the error message.
    
    lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
                                      (lstrlen((LPCTSTR) lpMsgBuf) + lstrlen((LPCTSTR) lpszFunction) + 40) * sizeof(TCHAR));
    StringCchPrintf((LPTSTR)lpDisplayBuf, 
                    LocalSize(lpDisplayBuf) / sizeof(TCHAR),
                    TEXT("%s failed with error %d: %s"), 
                    lpszFunction, dw, lpMsgBuf); 
    MessageBox(NULL, (LPCTSTR) lpDisplayBuf, TEXT("Error"), MB_OK); 
    
    // Free error-handling buffer allocations.
    
    LocalFree(lpMsgBuf);
    LocalFree(lpDisplayBuf);
}


#else

#define TESTNLOOPS 100
#define TESTNTHDS 10
#define BUF_SIZE 255

static thd_mutex mutex;
static size_t increment;

static void test_method()
{
#ifdef _WIN32
    HANDLE hStdout;
    TCHAR msgBuf[BUF_SIZE];
    size_t cchStringSize;
    DWORD dwChars;
#endif
    size_t temp;
    thd_mutex_lock(&mutex);
    temp = increment;
    increment = temp + 1;
#ifdef _WIN32
    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    StringCchPrintf(msgBuf, BUF_SIZE, TEXT("test_method\n"));
    StringCchLength(msgBuf, BUF_SIZE, &cchStringSize);
    WriteConsole(hStdout, msgBuf, (DWORD)cchStringSize, &dwChars, NULL);
#else
    printf("%i ", (int)increment);
#endif
    thd_mutex_unlock(&mutex);
}

int main(int argc, char** argv)
{
    size_t i, j;
    thd_thread threads[TESTNTHDS];
    printf("test thread...");
    thd_mutex_init(&mutex);
    
    for(j = 0; j < TESTNLOOPS; j++)
    {
        increment = 0;
        for(i = 0; i < TESTNTHDS; i++)
        {
            thd_thread_launch(threads+i, (thd_thread_method)test_method, NULL);
        }
        
        for(i = 0; i < TESTNTHDS; i++)
        {
            thd_thread_join(threads+i);
        }
        if(increment != TESTNTHDS)
            printf("error ");
    }
    
    thd_mutex_delete(&mutex);
    
    printf("ok\n");
    return 0;
}

#endif

