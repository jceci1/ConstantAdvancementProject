#ifndef GLOBAL_COUNTER_H
#define GLOBAL_COUNTER_H

#include <pthread.h>

class global_counter {
    private: 
        int m_value;                //the count value
        pthread_mutex_t m_lock;     //mutex to protect access to m_value

    public:
        global_counter();           //constructor
        ~global_counter();          //destructor
        int incrementAndGet();      //increments the counter and returns the new value
        int getValue();             //returns the current counter value
};


#endif