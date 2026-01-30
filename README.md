# Thread Library and Counter Application

# Overview
This project implements a Java-like threading abstraction over POSIX threads (pthreads), then uses it through a command-line application that counts from 1 to 100 using 5 worker threads.

# Build/Execution Instructions
- cd CommandLineApplication
- make 
- ./counter_app

The application will prompt for two inputs:
1. Counter type (1: Normal OS scheduling, 2: Deterministic scheduling)
2. Random seed (Any unsigned integer)

# Thread Library Features
The 'thread' class provides the following features:
- start(): can only be called once per thread object
- requestStop(): signals the task to stop cooperatively
- join(): blocks until worker completes and executes the 'done()' callback exactly once on the joining thread
- Each thread object gets a stable, unique ID via 'getId()'
- waitTurn() and notifyTurn() provide condition variable synchronization


The 'runnable' interface must be implemented by any task that runs on a thread since it is a purely virtual class:
- run(): The main work loop executed by the worker thread
- done(): Callback executed on the joining thread after completion
- requestStop(): Handles stop signals gracefully
- stopRequested(): Check if a stop has been requested

## How the Application Works
The application distributes the values 1-100 across 5 worker threads in a pseudo-random but reproducible manner based on the provided seed. Each value is assigned to exactly one thread before execution begins.

### Normal Mode (Option 1)
In normal mode, the operating system's scheduler determines the order in which threads execute their assigned work. While the distribution of values to threads is reproducible (same seed = same assignments), the actual execution order varies between runs because threads race to process their assigned values. 

### Deterministic Mode (Option 2)
In deterministic mode, a custom thread scheduler enforces a strict execution order. Not only is the distribution of values reproducible, but the exact sequence of which thread processes which value and when is also controlled. This guarantees identical output AND execution order between runs with the same seed.

Both modes use the same work distribution algorithm, but deterministic mode adds explicit synchronization to control execution order, while normal mode allows natural OS scheduling.


# Output
Below is an example output from running the application in Deterministic mode with the seed '123':
Select counter type (1:Normal, 2: Deterministic): 2
Enter random seed (unsigned integer): 123

Deterministic scheduling enabled with seed: 123

Thread 0 processed: 12
Thread 1 processed: 20
Thread 2 processed: 31
Thread 3 processed: 28
Thread 4 processed: 9

thread execution summary:

Thread 0 (ID: 1):
  Processed 12 values
  Values: 18, 27, 33, 35, 36, 61, 64, 66, 68, 70, 83, 92

Thread 1 (ID: 2):
  Processed 20 values
  Values: 3, 5, 19, 21, 24, 26, 29, 42, 48, 51, 53, 54, 55, 56, 57, 71, 84, 85, 91, 96

Thread 2 (ID: 3):
  Processed 31 values
  Values: 4, 7, 10, 11, 14, 16, 17, 20, 25, 30, 34, 37, 39, 40, 44, 50, 63, 65, 67, 69,
          73, 78, 79, 80, 82, 86, 87, 93, 94, 97, 98

Thread 3 (ID: 4):
  Processed 28 values
  Values: 1, 2, 6, 8, 9, 12, 15, 22, 23, 28, 31, 32, 38, 41, 45, 46, 47, 49, 52, 59,
          60, 62, 74, 76, 81, 88, 99, 100

Thread 4 (ID: 5):
  Processed 9 values
  Values: 13, 43, 58, 72, 75, 77, 89, 90, 95

Total values processed across all threads: 100

Verification results:
VERIFICATION PASSED
All values 1-100 present exactly once
Total count: 100

