#ifndef THREAD_H
#define THREAD_H

#include "runnable.h"
#include <pthread.h>

/*
A Java-like abstraction for managing pthread lifecycle and synchronization.
*/

class thread {
    private:
        pthread_t m_os_id;                      //OS-level thread identifier
        int m_unique_id;                        //unique identifier for this thread object        
        runnable* m_task;                       //task to be executed 
        bool m_started;                         //indicates if thread has been started
        bool m_joined;                          //indicates if thread has been joined                 
        bool m_stop_requested;                  //indicates if a stop has been requested
        pthread_mutex_t m_stateMutex;           //mutex to protect thread state
        pthread_cond_t m_cond;                  //condition variable for wait/notify    
        bool m_can_proceed;                     //flag for wait/notify

        static void* threadEntry(void* arg);    //entry point for the thread
        
    public:

        thread(runnable* task);                 //constructor
        ~thread();                              //destructor
        void start();                           //starts the thread
        void join();                            //joins the thread
        int getId() const;                      //returns unique thread ID 
        void requestStop();                     //signals the thread to stop
        bool getStopRequested();                //checks if stop has been requested

        void waitTurn();                        //blocks until notifyTurn() is called
        void notifyTurn();                      //grants permission for one waiting thread to proceed
};

#endif