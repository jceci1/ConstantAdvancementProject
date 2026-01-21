#ifndef THREAD_H
#define THREAD_H

#include "runnable.h"
#include <pthread.h>


class thread {
    private:
        pthread_t m_id;
        runnable* m_task;
        bool m_started;
        bool m_joined;
        static void* threadEntry(void* arg);
        pthread_mutex_t m_stateMutex;

    public:
        ~thread();
        thread(runnable* task);
        void start();
        void join();

};

#endif