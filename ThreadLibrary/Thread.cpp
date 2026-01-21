#include <iostream>
#include "thread.h"
#include "runnable.h"

//
void* thread::threadEntry(void* arg) {
    thread* curr_thread = static_cast<thread*>(arg);
    if (curr_thread && curr_thread->m_task) curr_thread->m_task->run();
    return nullptr;
}

thread::thread(runnable* task) {
    m_task = task;
    m_id = 0;
}

void thread::start() {
    pthread_create(&m_id, nullptr, threadEntry, this);
}

void thread::join() {
    //ensure join() is not called on a non-started thread
    if(m_id != 0) {
        pthread_join(m_id, nullptr);
        m_id = 0;
    }
}