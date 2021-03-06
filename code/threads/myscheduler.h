// scheduler.h 
//	Data structures for the thread dispatcher and scheduler.
//	Primarily, the list of threads that are ready to run.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

//#ifndef MYSCHEDULER_H
//#define MYSCHEDULER_H

//#include "copyright.h"
#include "list.h"
//#include "thread.h"
#include "process.h"

// The following class defines the scheduler/dispatcher abstraction -- 
// the data structures and operations needed to keep track of which 
// thread is running, and which threads are ready but not running.

class MyScheduler {
  public:
    MyScheduler();		// Initialize list of ready threads 
    ~MyScheduler();		// De-allocate ready list

    void ReadyToRun(Process* process);	
    				// Thread can be dispatched.
    Process* FindNextToRun();	// Dequeue first thread on the ready 
				// list, if any, and return thread.
    void Run(Process* nextProcess, bool finishing);
    				// Cause nextThread to start running
    void CheckToBeDestroyed();// Check if thread that had been
    				// running needs to be deleted
    void Print();		// Print contents of ready list
    
    // SelfTest for scheduler is implemented in class Thread

    SortedList<Process *> *readyList;  // queue of threads that are ready to run,
                // but not running
    
  private:
    
    Process *toBeDestroyed;	// finishing thread to be destroyed
    				// by the next thread that runs
};

//#endif // SCHEDULER_H
