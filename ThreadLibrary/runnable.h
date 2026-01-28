#ifndef RUNNABLE_H
#define RUNNABLE_H

class thread;

class runnable {
public:
    virtual void run() = 0;
    virtual ~runnable() {};
    virtual void done() = 0;
    virtual void requestStop() = 0;
    virtual bool stopRequested() const = 0;
};

#endif