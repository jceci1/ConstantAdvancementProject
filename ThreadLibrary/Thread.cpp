#include <iostream>
#include "thread.h"
#include "runnable.h"

static int id_counter = 1;
static pthread_mutex_t id_mutex = PTHREAD_MUTEX_INITIALIZER;



thread::thread(runnable* task) {
    m_task = task;
    m_started = false;
    m_joined = false;
    m_stop_requested = false;
    m_can_proceed = false;
    
    pthread_mutex_lock(&id_mutex);
    m_unique_id = id_counter++;
    pthread_mutex_unlock(&id_mutex);
    

    pthread_mutex_init(&m_stateMutex, nullptr);
    pthread_cond_init(&m_cond, nullptr);
}



thread::~thread() {
    if(m_started && !m_joined) {
        join();
    }
    
    pthread_mutex_destroy(&m_stateMutex);
    pthread_cond_destroy(&m_cond);
}



int thread::getId() const {
    return m_unique_id;
}



void* thread::threadEntry(void* arg) {
    thread* curr_thread = static_cast<thread*>(arg);
    if (curr_thread && curr_thread->m_task) curr_thread->m_task->run();
    return nullptr;
}



void thread::start() {
    pthread_mutex_lock(&m_stateMutex);
    
    if (m_started) {
        std::cout << "Double start" << std::endl;
        pthread_mutex_unlock(&m_stateMutex);
        return; 
    }
    
    pthread_create(&m_os_id, nullptr, threadEntry, this);

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
    pthread_t id_to_join = m_os_id;

    pthread_mutex_unlock(&m_stateMutex);
    
    if(pthread_join(id_to_join, nullptr) == 0) {
        if (m_task) {
            m_task->done();
        }
    } 
}



void thread::requestStop() {
    pthread_mutex_lock(&m_stateMutex);
    
    m_stop_requested = true; 
    if (m_task) {
        m_task->requestStop(); 
    }
    
    pthread_mutex_unlock(&m_stateMutex);
}



bool thread::getStopRequested() {
    pthread_mutex_lock(&m_stateMutex);
    bool stop_requested = m_stop_requested;
    pthread_mutex_unlock(&m_stateMutex);
    return stop_requested;
}



void thread::waitTurn() {
    pthread_mutex_lock(&m_stateMutex);

    while (!m_can_proceed) {
        pthread_cond_wait(&m_cond, &m_stateMutex);
    }
    
    m_can_proceed = false;
    pthread_mutex_unlock(&m_stateMutex);
}



void thread::notifyTurn() {
    pthread_mutex_lock(&m_stateMutex);
    m_can_proceed = true;
    pthread_cond_signal(&m_cond);
    pthread_mutex_unlock(&m_stateMutex);
}