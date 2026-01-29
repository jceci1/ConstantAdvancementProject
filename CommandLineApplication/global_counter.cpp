#include "global_counter.h"


/*Constructor: Initializes the counter to zero and initializes the mutex*/
global_counter::global_counter() {
    m_value = 0;
    pthread_mutex_init(&m_lock, nullptr);
}


/*Destructor: Destroys the mutex*/
global_counter::~global_counter() {
    pthread_mutex_destroy(&m_lock);
}


/*Increment and Return value: Increments the counter and returns the new value*/
int global_counter::incrementAndGet() {
    pthread_mutex_lock(&m_lock);
    m_value++;
    int curr_value = m_value;
    pthread_mutex_unlock(&m_lock);
    return curr_value;
}


/*Value Getter: Returns the current counter value*/
int global_counter::getValue() {
    pthread_mutex_lock(&m_lock);
    int curr_value = m_value;
    pthread_mutex_lock(&m_lock);
    return curr_value;
}