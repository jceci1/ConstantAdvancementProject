#ifndef RUNNABLE_H
#define RUNNABLE_H

class runnable {
public:
    virtual void run() = 0;
    virtual ~runnable() {};
    virtual bool stopRequested() const = 0;
};

#endif