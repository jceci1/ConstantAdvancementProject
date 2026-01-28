#ifndef THREAD_H
#define THREAD_H

#include "runnable.h"
#include <pthread.h>

class thread {
    private:
        pthread_t m_os_id;
        int m_unique_id;
        runnable* m_task;
        bool m_started;
        bool m_joined;
        bool m_stop_requested;
        pthread_mutex_t m_stateMutex;
        pthread_cond_t m_cond;
        bool m_can_proceed;

        static void* threadEntry(void* arg);
        
    public:
        ~thread();
        thread(runnable* task);
        void start();
        void join();
        int getId() const;
        void requestStop();
        bool getStopRequested();

        void waitTurn();
        void notifyTurn();
};

#endif