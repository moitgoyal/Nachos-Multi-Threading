#include "debug.h"
#include "kernel.h"
#include "main.h"
#include "string"
#include <iostream>
using namespace std;

Process::Process(char* process_name, int priority)
{
	scheduler = new Scheduler();	// initialize the ready queue
	processName = process_name;
 	processPriority = priority;  
 	joinList = new List<Process*>;  
 	isTerminated = false;
 	parentProcessID = NULL;	
}


Process::~Process()
{	
}


void Process::createThread(char * name,VoidFunctionPtr func, void *arg){

	Thread *newThread = new Thread(name);
    
    Interrupt *interrupt = kernel->interrupt;
    Scheduler *scheduler = this->scheduler;
    IntStatus oldLevel;
    
    DEBUG(dbgThread, "Forking thread: " << newThread->getName() << " Thread ID: " << newThread);
    
    newThread->StackAllocate(func, arg);    

    oldLevel = interrupt->SetLevel(IntOff);
    this->scheduler->ReadyToRun(newThread);	// ReadyToRun assumes that interrupts 
					// are disabled!

    cout << "Forked Thread with Name :" << name << "\n";
    (void) interrupt->SetLevel(oldLevel);

}

void Process::Join(){		

	IntStatus oldLevel = kernel->interrupt->SetLevel(IntOff);		
    while(this->joinList->NumInList() > 0){
    	this->Sleep(false);
    }       
    (void) kernel->interrupt->SetLevel(oldLevel);
}

void Process::CreateChildProcess(char* childName, void *arg){	

	Process *p = new Process(childName,this->getProcessPriority());
	p->Fork(this->processFunction, arg);		

	p->parentProcessID = this;

	this->joinList->Append(p);		

}

void Process::Terminate(){

	IntStatus oldLevel = kernel->interrupt->SetLevel(IntOff);	

	ListIterator<Thread*> *iter = new ListIterator<Thread*>(this->scheduler->readyList); 	

	//ThreadFinish(this->currentThread);

	for (; !iter->IsDone(); iter->Next()) {			    
	    ThreadFinish(iter->Item());
    }     

    this->isTerminated = true;
	this->Sleep(true);

	(void) kernel->interrupt->SetLevel(oldLevel);		

}


void
Process::ThreadFinish (Thread * t)
{
    (void) kernel->interrupt->SetLevel(IntOff);		            
    
    DEBUG(dbgThread, "Finishing thread: " << t->getName());
    
    ThreadSleep(t, TRUE);				// invokes SWITCH
    // not reached
}

void
Process::ThreadSleep (Thread * t,bool finishing)
{          
    ASSERT(kernel->interrupt->getLevel() == IntOff);
    
    DEBUG(dbgThread, "Sleeping thread: " << t->getName());    

    t->status = BLOCKED; 

    delete t;        
}


void 
Process::Fork(VoidFunctionPtr func, void *arg)
{
	Interrupt *interrupt = kernel->interrupt;
    MyScheduler *myscheduler = kernel->scheduler;
    IntStatus oldLevel;
    
    DEBUG(dbgThread, "Forking Process: " << processName << " Process ID: " <<  this);      

    oldLevel = interrupt->SetLevel(IntOff);

    processFunction = func;

    myscheduler->ReadyToRun(this);	// ReadyToRun assumes that interrupts 
					// are disabled!

    cout << "Forked Process with Name :" << processName << "\n";
    
    this->createThread("First Thread",(VoidFunctionPtr) func, (void *) arg);
    this->currentThread = this->scheduler->FindNextToRun();    

    
    (void) interrupt->SetLevel(oldLevel);
}


void
Process::Begin ()
{    
    ASSERT(this == kernel->currentProcess);   

    DEBUG(dbgThread, "Beginning Process: " << processName);
    
    kernel->scheduler->CheckToBeDestroyed();
    kernel->interrupt->Enable();
}

void
Process::Finish ()
{
    (void) kernel->interrupt->SetLevel(IntOff);		
    
    ASSERT(this == kernel->currentProcess);    
    
    DEBUG(dbgThread, "Finishing Process: " << processName);     

    Sleep(TRUE);				// invokes SWITCH
    // not reached
}

void
Process::Yield ()
{
    Process *nextProcess;
    IntStatus oldLevel = kernel->interrupt->SetLevel(IntOff);
        
    ASSERT(this == kernel->currentProcess);    
    
    DEBUG(dbgThread, "Yielding Process: " << processName);
    
    nextProcess = kernel->scheduler->FindNextToRun();
    if (nextProcess != NULL) {
	kernel->scheduler->ReadyToRun(this);
	kernel->scheduler->Run(nextProcess, FALSE);
    }
    (void) kernel->interrupt->SetLevel(oldLevel);
}

void
Process::Sleep (bool finishing)
{
    Process *nextProcess;

    ASSERT(this == kernel->currentProcess);   

    ASSERT(kernel->interrupt->getLevel() == IntOff);
    
    DEBUG(dbgThread, "Sleeping Process: " << processName);  

    status = BLOCKED;    
	       	     
    if(this->parentProcessID != NULL){ 	
    	if(this->parentProcessID->isTerminated == false) {
		    if(this->parentProcessID->joinList->IsInList(this)){
		    	this->parentProcessID->joinList->Remove(this);   
		    }    	 	
	    	if(!kernel->scheduler->readyList->IsInList(this->parentProcessID)){    		
	    		kernel->scheduler->ReadyToRun(this->parentProcessID);
	    	}    	
    	}	    	
    }


    while ((nextProcess = kernel->scheduler->FindNextToRun()) == NULL)
	kernel->interrupt->Idle();	// no one to run, wait for an interrupt    
	
	if(finishing){		
		delete this;
	}
    // returns when it's time for us to run
    kernel->scheduler->Run(nextProcess, false); 
}

void ProcessPrint(Process *p) { p->Print(); }


int Process::getProcessPriority(){
	return processPriority;
}
