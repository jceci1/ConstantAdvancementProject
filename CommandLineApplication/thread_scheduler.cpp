#include "thread_scheduler.h"
#include <random>


/*Thread Scheduler constructor: Initializes scheduler in disabled state.*/
thread_scheduler::thread_scheduler() : m_enabled(false), m_current_index(0) {
    m_enabled = false;
    m_current_index = 0;
    pthread_mutex_init(&m_mutex, nullptr);
    pthread_cond_init(&m_cond, nullptr);
}


/*Thread Scheduler destructor: Cleans up synchronization primitives.*/
thread_scheduler::~thread_scheduler() {
    pthread_mutex_destroy(&m_mutex);
    pthread_cond_destroy(&m_cond);
}


/*Set Enabled: Enables or disables deterministic scheduling mode.*/
void thread_scheduler::setEnabled(bool enabled) {
    pthread_mutex_lock(&m_mutex);
    m_enabled = enabled;
    pthread_mutex_unlock(&m_mutex);
}


/*Set Execution Order: Defines the sequence in which threads should execute.*/
void thread_scheduler::setOrder(const std::vector<int>& order) {
    pthread_mutex_lock(&m_mutex);
    m_execution_order = order;
    m_current_index = 0;
    pthread_mutex_unlock(&m_mutex);
}


/*Wait For Turn: Blocks until it's this thread's turn to execute in deterministic mode.*/
void thread_scheduler::waitForTurn(int threadId) {
    //handles normal OS scheduling
    if(!m_enabled) {
        
        return;
    }
    
    pthread_mutex_lock(&m_mutex);
    
    //wait until it's this thread's turn
    while (m_current_index < m_execution_order.size() && 
           m_execution_order[m_current_index] != threadId) {
        pthread_cond_wait(&m_cond, &m_mutex);
    }
    
    pthread_mutex_unlock(&m_mutex);
}


/*Signal Turn Complete: Advances to next position and wakes waiting threads.*/
void thread_scheduler::signalTurnComplete() {
    if(!m_enabled) {
        return;
    }
    
    pthread_mutex_lock(&m_mutex);
    
    if(m_current_index < m_execution_order.size()) {
        m_current_index++;
    }
    
    //allow all threads to check if it's their turn
    pthread_cond_broadcast(&m_cond);
    
    pthread_mutex_unlock(&m_mutex);
}


/*Generate Execution Order: Creates a reproducible pseudo-random sequence of thread IDs.*/
std::vector<int> thread_scheduler::generateOrder(unsigned int seed) {
    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> dist(0, 4);
    
    std::vector<int> order;
    order.reserve(100);
    
    for(int i = 0; i < 100; i++) {
        order.push_back(dist(rng));
    }
    
    return order;
}