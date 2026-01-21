#include <iostream>
#include "thread.h"
#include "runnable.h"


thread::~thread() {

    
    pthread_mutex_destroy(&m_stateMutex);
}


thread::thread(runnable* task) {
    m_task = task;
    m_id = 0;
    m_started = false;
    m_joined = false;
    pthread_mutex_init(&m_stateMutex, nullptr);
}



//entry point for new thread. Circumvents C++ member function pointer issue.
void* thread::threadEntry(void* arg) {
    thread* curr_thread = static_cast<thread*>(arg);
    if (curr_thread && curr_thread->m_task) curr_thread->m_task->run();
    return nullptr;
}



void thread::start() {
    
    pthread_mutex_lock(&m_stateMutex);
    
    if (m_started) {
        pthread_mutex_unlock(&m_stateMutex);
        return; 
        std::cout << "Double start" << std::endl;

    }
    
    pthread_create(&m_id, nullptr, threadEntry, this);

    m_started = true;
    pthread_mutex_unlock(&m_stateMutex);
}



void thread::join() {
    pthread_mutex_lock(&m_stateMutex);
    
    if(!m_started || m_joined) {
        pthread_mutex_unlock(&m_stateMutex);
        std::cout << "Double join or join before start detected" << std::endl;
        return;
    }
    

    m_joined = true;
    pthread_t curr_id = m_id;
    m_id = 0;
    pthread_mutex_unlock(&m_stateMutex);
    pthread_join(curr_id, nullptr);    
}