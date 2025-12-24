#include "thread_compat.h"

#include <stdlib.h>

#ifdef _WIN32
typedef struct prophet_thread_start {
    void* (*fn)(void*);
    void* arg;
} prophet_thread_start_t;

static unsigned __stdcall prophet_thread_start(void* arg)
{
    prophet_thread_start_t* start = (prophet_thread_start_t*)arg;
    void* ret = start->fn(start->arg);
    free(start);
    return (unsigned)(uintptr_t)ret;
}

void prophet_mutex_init(prophet_mutex_t* mutex)
{
    InitializeCriticalSection(mutex);
}

void prophet_mutex_lock(prophet_mutex_t* mutex)
{
    EnterCriticalSection(mutex);
}

void prophet_mutex_unlock(prophet_mutex_t* mutex)
{
    LeaveCriticalSection(mutex);
}

int prophet_thread_create(prophet_thread_t* thread, void* (*fn)(void*), void* arg)
{
    prophet_thread_start_t* start = (prophet_thread_start_t*)malloc(sizeof(prophet_thread_start_t));
    if (!start) {
        return -1;
    }
    start->fn = fn;
    start->arg = arg;

    uintptr_t handle = _beginthreadex(NULL, 0, prophet_thread_start, start, 0, NULL);
    if (!handle) {
        free(start);
        return -1;
    }

    *thread = (HANDLE)handle;
    return 0;
}

int prophet_thread_join(prophet_thread_t thread)
{
    DWORD wait_result = WaitForSingleObject(thread, INFINITE);
    DWORD err = (wait_result == WAIT_OBJECT_0) ? 0 : GetLastError();
    CloseHandle(thread);
    return (int)err;
}
#else
void prophet_mutex_init(prophet_mutex_t* mutex)
{
    pthread_mutex_init(mutex, NULL);
}

void prophet_mutex_lock(prophet_mutex_t* mutex)
{
    pthread_mutex_lock(mutex);
}

void prophet_mutex_unlock(prophet_mutex_t* mutex)
{
    pthread_mutex_unlock(mutex);
}

int prophet_thread_create(prophet_thread_t* thread, void* (*fn)(void*), void* arg)
{
    return pthread_create(thread, NULL, fn, arg);
}

int prophet_thread_join(prophet_thread_t thread)
{
    return pthread_join(thread, NULL);
}
#endif
