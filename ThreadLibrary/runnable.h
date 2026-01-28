#ifndef RUNNABLE_H
#define RUNNABLE_H

class thread;

/*
Interface for logic to be executed within a thread.
Implementers must provide the work loop and handle stop signals.
*/

class runnable {
public:
    //work loop. Executed on the worker thread.
    virtual void run() = 0;
    
    //destructor
    virtual ~runnable() {};
    
    //callback executed on the joining thread after run() completes.
    virtual void done() = 0;
    
    //signal the task to stop gracefully.
    virtual void requestStop() = 0;
    
    //check if a stop has been requested.
    virtual bool stopRequested() const = 0;
};

#endif