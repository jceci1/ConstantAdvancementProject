#include <iostream>
#include "thread.h"
#include "runnable.h"

static int id_counter = 1;
static pthread_mutex_t id_mutex = PTHREAD_MUTEX_INITIALIZER;


/* Thread constructor: Initializes new thread object but does not start the thread.*/
thread::thread(runnable* task) {
    m_task = task;
    m_started = false;
    m_joined = false;
    m_stop_requested = false;
    m_can_proceed = false;
    
    //protects from race condition involving simultaneous thread instantiations
    pthread_mutex_lock(&id_mutex);
    m_unique_id = id_counter++;
    pthread_mutex_unlock(&id_mutex);
    

    pthread_mutex_init(&m_stateMutex, nullptr);
    pthread_cond_init(&m_cond, nullptr);
}


/* Thread destructor: Ensures thread is joined if not already done before destruction. */
thread::~thread() {
    if(m_started && !m_joined) {
        join();
    }
    
    pthread_mutex_destroy(&m_stateMutex);
    pthread_cond_destroy(&m_cond);
}


/* ID getter: Returns the unique identifier assigned to this thread object.*/
int thread::getId() const {
    return m_unique_id;
}


/*Thread starter: Starts the physical OS thread and begins execution of the ask.
 Ensures the thread is only started once and protects against concurrent start attempts.*/
void thread::start() {
    pthread_mutex_lock(&m_stateMutex);

    if (m_started) {
        std::cout << "Double start" << std::endl;
        pthread_mutex_unlock(&m_stateMutex);
        return; 
    }
    
    //spawn the thread and pass 'this' so that threadEntry can access member variables
    pthread_create(&m_os_id, nullptr, threadEntry, this);

    m_started = true;
    pthread_mutex_unlock(&m_stateMutex);
}


/*Thread Join: Blocks the calling thread until the worker thread finishes. Ensures done() callback is executed exactly once on the joining thread.*/
void thread::join() {
    pthread_mutex_lock(&m_stateMutex);
    
    if(!m_started || m_joined) {
        pthread_mutex_unlock(&m_stateMutex);
        std::cout << "Double join or join before start detected" << std::endl;
        return;
    }
    
    m_joined = true;
    pthread_t id_to_join = m_os_id;

    //mutex unlocked early so that the worker thread can acquire it if needed.
    pthread_mutex_unlock(&m_stateMutex);
    
    if(pthread_join(id_to_join, nullptr) == 0) {
        if (m_task) {
            m_task->done();
        }
    } 
}


/*Request Stop: Tells the worker thread to stop its execution gracefully.*/
void thread::requestStop() {
    pthread_mutex_lock(&m_stateMutex);
    
    m_stop_requested = true; 
    if (m_task) {
        m_task->requestStop(); 
    }
    
    pthread_mutex_unlock(&m_stateMutex);
}


/*Stop Requested Getter: Safely returns the contents of the stop requested flag.*/
bool thread::getStopRequested() {
    pthread_mutex_lock(&m_stateMutex);
    bool stop_requested = m_stop_requested;
    pthread_mutex_unlock(&m_stateMutex);
    return stop_requested;
}


/*Wait Turn: Blocks the calling thread until notifyTurn() is called.*/
void thread::waitTurn() {
    pthread_mutex_lock(&m_stateMutex);

    //to protect against spurious wakeups
    while (!m_can_proceed) {
        pthread_cond_wait(&m_cond, &m_stateMutex);
    }
    
    m_can_proceed = false;
    pthread_mutex_unlock(&m_stateMutex);
}


/*Notify Turn: Grants permission for one currently waiting thread to proceed. */
void thread::notifyTurn() {
    pthread_mutex_lock(&m_stateMutex);
    m_can_proceed = true;
    pthread_cond_signal(&m_cond);
    pthread_mutex_unlock(&m_stateMutex);
}


/*Thread Entry: Acts as the entry point for the thread.*/
void* thread::threadEntry(void* arg) {
    thread* curr_thread = static_cast<thread*>(arg);
    pthread_mutex_lock(&curr_thread->m_stateMutex);
    runnable* task = curr_thread->m_task;
    pthread_mutex_unlock(&curr_thread->m_stateMutex);
    
    if (task) task->run();
    return nullptr;
}