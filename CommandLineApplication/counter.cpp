#include "counter.h"
#include <iostream>
#include <unistd.h>

/*Counter constructor: Initializes a counter task for a specific thread with reproducible random behavior.*/
counter::counter(global_counter* counter, int thread_id, unsigned int seed, bool use_delays, thread_scheduler* scheduler) {
    m_counter = counter;
    m_thread_id = thread_id;
    m_stop_requested = false;
    m_use_delays = use_delays;
    m_scheduler = scheduler;
    
    m_rng.seed(seed + thread_id);
}

/*Counter destructor: Cleanup handled automatically.*/
counter::~counter() {
}


/*Assign Value: Adds a value to the set of values this thread will process.*/
void counter::assignValue(int value) {
    m_assigned_values.insert(value);
}


/*Run: Main work loop executed on the worker thread. Processes all assigned values in order.*/
void counter::run() {
    for(int value : m_assigned_values) {
        if(stopRequested()) {
            break;
        }
        
        //wait for scheduler permission in deterministic mode
        m_scheduler->waitForTurn(m_thread_id);
        
        if(m_use_delays) {
            //add random delay between 0-100 microseconds for normal mode
            int delay_us = m_rng() % 101;
            usleep(delay_us);
        }
        
        m_counter->incrementAndGet();
        m_processed_values.push_back(value);
        
        m_scheduler->signalTurnComplete();
    }
}


/*Done: Callback executed on the joining thread after worker completes.*/
void counter::done() {
    std::cout << "Thread " << m_thread_id << " processed: " 
              << m_processed_values.size() << std::endl;
}


/*Request Stop: Signals the task to stop gracefully.*/
void counter::requestStop() {
    m_stop_requested = true;
}


/*Stop Requested: Returns whether a stop has been requested.*/
bool counter::stopRequested() const {
    return m_stop_requested;
}


/*Get Processed Values: Returns the list of values this thread processed.*/
const std::vector<int>& counter::getProcessedValues() const {
    return m_processed_values;
}