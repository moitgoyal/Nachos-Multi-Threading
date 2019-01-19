#include "utility.h"
#include "scheduler.h"
//#include "kernel.h"
//#include "thread.h"




//enum ProcessStatus { JUST_CREATED, RUNNING, READY, BLOCKED };

class Process{
	private:
		char* 	processName;
		int 	processPriority;


		ThreadStatus status;	// ready, running or blocked
		
	public:

		Process(char* Name,int priority);		// initialize a Process    
				
    	~Process();	


		void Fork(VoidFunctionPtr func, void *arg); 		
		void Begin();
		void Finish();
		void Yield();
		void Sleep(bool finishing);
		void Terminate();
		void Join();
		void CreateChildProcess(char* childName, void *arg);

		void createThread(char * name,VoidFunctionPtr func, void *arg); 

		void ThreadFinish(Thread * t);
		void ThreadSleep(Thread * t,bool finishing);

		bool isTerminated;

		Thread *currentThread;	// the thread holding the CPU

		void setStatus(ThreadStatus st) { status = st; }

		Scheduler *scheduler;	// the ready list
			
		void Print() { cout << processName; }

		List<Process *> *joinList;

		Process * parentProcessID;

		VoidFunctionPtr processFunction;


    	int getProcessPriority();

    	char* getProcessName() { return (processName); }

};

extern void ProcessPrint(Process *process);	 
