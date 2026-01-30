#ifndef COUNTER_H
#define COUNTER_H
#include "runnable.h"
#include "global_counter.h"
#include "thread_scheduler.h"
#include <vector>
#include <random>
#include <set>

class counter : public runnable {
    private:
        global_counter* m_counter;              //shared counter incremented by all threads
        std::vector<int> m_processed_values;    //values this thread has processed
        bool m_stop_requested;                  //indicates if a stop has been requested
        int m_thread_id;                        //unique identifier for this thread
        std::mt19937 m_rng;                     //random number generator for delays
        bool m_use_delays;                      //whether to add random delays during execution
        std::set<int> m_assigned_values;        //values assigned to this thread for processing
        thread_scheduler* m_scheduler;          //scheduler for coordinating thread execution
    
    public:
        counter(global_counter* counter, int thread_id, unsigned int seed, bool use_random_delays, thread_scheduler* scheduler);     
        ~counter();                                             
        void run() override;                    //main work loop
        void done() override;                   //callback executed on joining thread
        void requestStop() override;            //signal task to stop
        bool stopRequested() const override;    //check if stop requested
        
        const std::vector<int>& getProcessedValues() const;
        void assignValue(int value);
};


#endif