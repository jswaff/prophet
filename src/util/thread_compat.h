#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef _WIN32
#include <windows.h>
#include <process.h>

typedef HANDLE prophet_thread_t;
typedef CRITICAL_SECTION prophet_mutex_t;
#else
#include <pthread.h>

typedef pthread_t prophet_thread_t;
typedef pthread_mutex_t prophet_mutex_t;
#endif

void prophet_mutex_init(prophet_mutex_t* mutex);
void prophet_mutex_lock(prophet_mutex_t* mutex);
void prophet_mutex_unlock(prophet_mutex_t* mutex);

int prophet_thread_create(prophet_thread_t* thread, void* (*fn)(void*), void* arg);
int prophet_thread_join(prophet_thread_t thread);
