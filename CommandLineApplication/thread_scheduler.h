#ifndef THREAD_SCHEDULER_H
#define THREAD_SCHEDULER_H

#include <vector>
#include <pthread.h>

class thread_scheduler {
    private:

        
        bool m_enabled;                         //indicates if deterministic scheduling is active
        std::vector<int> m_execution_order;     //predefined thread execution sequence
        size_t m_current_index;                 //current position in the execution order  
        pthread_mutex_t m_mutex;                //mutex for synchronizing access
        pthread_cond_t m_cond;                  //condition variable for thread coordination
        
        
        
    public:
        thread_scheduler();     
        ~thread_scheduler();                               
        void setEnabled(bool enabled);                          //enables or disables deterministic scheduling
        void setOrder(const std::vector<int>& order);           //sets the execution order
        void waitForTurn(int threadId);                         //blocks until it's this thread's turn  
        void signalTurnComplete();                              //signals that the thread has completed its turn
        
        static std::vector<int> generateOrder(unsigned int seed);  //generates a pseudo-random execution order
};

#endif