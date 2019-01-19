#include "kernel.h"
#include "main.h"
#include "thread.h"
#include "debug.h"

void TestThreadRunning(int which){

	int num;
	cout << "Current Running Process Name is :" << kernel->currentProcess->getProcessName() << " and Current Running Thread is: " << kernel->currentProcess->currentThread->getName() << " and Priority of the Process is:" << kernel->currentProcess->getProcessPriority() << "\n";                             	
    
    for (num = 0; num < 2; num++) {
		cout << "*** thread " << which << " looped " << num << " times\n";                         
    }
    cout <<"\n";
}

void TestProcessTermination(int which){

	int num;
	cout << "Current Running Process Name is :" << kernel->currentProcess->getProcessName() << " and Current Running Thread is: " << kernel->currentProcess->currentThread->getName() << " and Priority of the Process is:" << kernel->currentProcess->getProcessPriority() << "\n";                             	
	for (num = 0; num < 2; num++) {
		cout << "*** thread " << which << " looped " << num << " times\n";                         
    }
	cout << "\n****************************************  Now Terminating The Process  ***********************************************\n\n";
	kernel->currentProcess->Terminate();
}

void TestProcessJoinAndChildProcess(int which){

	int num;
	cout << "Current Running Process Name is :" << kernel->currentProcess->getProcessName() << " and Current Running Thread is: " << kernel->currentProcess->currentThread->getName() << " and Priority of the Process is:" << kernel->currentProcess->getProcessPriority() << "\n";                             	
	for (num = 0; num < 2; num++) {
		cout << "*** thread " << which << " looped " << num << " times\n";                         
    }
    cout <<"\n";	
}

void TestPrioritySchedulingAndSwitching(int which){	
	cout << "Current Running Process Name is :" << kernel->currentProcess->getProcessName() << " and Current Running Thread is: " << kernel->currentProcess->currentThread->getName() << " and Priority of the Process is:" << kernel->currentProcess->getProcessPriority() << "\n";                             	
	kernel->currentProcess->createThread("Thread 1111",(VoidFunctionPtr) TestThreadRunning, (void *) 1111);
    kernel->currentProcess->createThread("Thread 2222",(VoidFunctionPtr) TestThreadRunning, (void *) 2222);  
    kernel->currentProcess->createThread("Thread 3333",(VoidFunctionPtr) TestThreadRunning, (void *) 3333);  
    cout <<"\n"; 		
}

void
SimpleProcess(int which)
{     	   		
    	cout << "Current Running Process Name is :" << kernel->currentProcess->getProcessName() << " and Current Running Thread is: " << kernel->currentProcess->currentThread->getName() << " and Priority of the Process is:" << kernel->currentProcess->getProcessPriority() << "\n";                             	
    	cout << "Now Forking 3 Threads.\n";
        kernel->currentProcess->createThread("Thread 1",(VoidFunctionPtr) TestThreadRunning, (void *) 1);
    	kernel->currentProcess->createThread("Thread 2",(VoidFunctionPtr) TestThreadRunning, (void *) 2);  
    	kernel->currentProcess->createThread("Thread 3",(VoidFunctionPtr) TestThreadRunning, (void *) 3);                           		          
		cout <<  "Thread Forking is complete.\n";  	
}

void TerminateProcess(int which){
		cout << "Current Running Process Name is :" << kernel->currentProcess->getProcessName() << " and Current Running Thread is: " << kernel->currentProcess->currentThread->getName() << " and Priority of the Process is:" << kernel->currentProcess->getProcessPriority() << "\n";                             	
		cout << "Now Forking 3 Threads\n";
        kernel->currentProcess->createThread("Thread 11",(VoidFunctionPtr) TestProcessTermination, (void *) 11);        
    	kernel->currentProcess->createThread("Thread 22",(VoidFunctionPtr) TestProcessTermination, (void *) 22);  
    	kernel->currentProcess->createThread("Thread 33",(VoidFunctionPtr) TestProcessTermination, (void *) 33);   
    	cout <<  "Thread Forking is complete.\n";
}


void JoinProcess(int which){
		cout << "Current Running Process Name is :" << kernel->currentProcess->getProcessName() << " and Current Running Thread is: " << kernel->currentProcess->currentThread->getName() << " and Priority of the Process is:" << kernel->currentProcess->getProcessPriority() << "\n";                             	
		cout << "\n******************************************  Now Joining The Process 3 ***********************************************\n\n";
        kernel->currentProcess->Join();   
        cout << "\n*****************************Code After all Child Process are complete of Process 3.**********************************\n\n";  
        int num;
		cout << "Current Running Process Name is :" << kernel->currentProcess->getProcessName() << " and Current Running Thread is: " << kernel->currentProcess->currentThread->getName() << " and Priority of the Process is:" << kernel->currentProcess->getProcessPriority() << "\n";
		for (num = 0; num < 2; num++) {
			cout << "*** thread " << which << " looped " << num << " times\n";                         
    	}	 
    	cout << "\n";  	 	   
}

void
ThreadTest()
{
	cout << "\n\n";

	cout <<"*************************************************************************************************************************************************************************************\n";
	cout << "Test Case to show 'Process Forking and Thread Forking' with Forking 3 new Threads and their internal running: \n"; 
	cout <<"*************************************************************************************************************************************************************************************\n";

	Process *p1 = new Process("Process 1",(rand() % 5) + 1);   
	p1->Fork((VoidFunctionPtr) SimpleProcess, (void *) 1);		

	kernel->currentProcess->Yield();	

	cout << "\n\n";

	cout <<"*************************************************************************************************************************************************************************************\n";
	cout << "Test Case to show 'Process Terminatation' with Forking 3 new Threads and Process is Terminated after 1 Thread is executed\n"; 
	cout <<"*************************************************************************************************************************************************************************************\n";

	Process *p2 = new Process("Process 2",(rand() % 5) + 1);   
	p2->Fork((VoidFunctionPtr) TerminateProcess, (void *) 1);

	kernel->currentProcess->Yield();

	cout << "\n\n";

	cout <<"*************************************************************************************************************************************************************************************\n";
	cout << "Test Case to show 'Process Join and Child Process' with 3 Child Process and Parent process having 3 new Threads to Run with Visible Priority Scheduling and Context Switching\n"; 
	cout <<"*************************************************************************************************************************************************************************************\n";

	Process *p3 = new Process("Process 3",(rand() % 5) + 1);   
	p3->Fork((VoidFunctionPtr) TestProcessJoinAndChildProcess, (void *) 1);	
	cout << "Now Creating 3 Child Process.\n";
    p3->CreateChildProcess("Child Process 1",(void*) 1);
    p3->CreateChildProcess("Child Process 2",(void*) 2);
    p3->CreateChildProcess("Child Process 3",(void*) 3);
	cout << "Now Creating 3 Threads for Process 3.\n";
    p3->createThread("Thread 111",(VoidFunctionPtr) JoinProcess, (void *) 111);        
   	p3->createThread("Thread 222",(VoidFunctionPtr) TestProcessJoinAndChildProcess, (void *) 222);  
    p3->createThread("Thread 333",(VoidFunctionPtr) TestProcessJoinAndChildProcess, (void *) 333);
	
	kernel->currentProcess->Yield();
	kernel->currentProcess->Yield();
	kernel->currentProcess->Yield();

	Process *p4 = new Process("Process 4",(rand() % 5) + 1);   
	p4->Fork((VoidFunctionPtr) TestPrioritySchedulingAndSwitching, (void *) 1);	
	Process *p5 = new Process("Process 5",(rand() % 5) + 1);   
	p5->Fork((VoidFunctionPtr) TestPrioritySchedulingAndSwitching, (void *) 1);	
	Process *p6 = new Process("Process 6",(rand() % 5) + 1);   
	p6->Fork((VoidFunctionPtr) TestPrioritySchedulingAndSwitching, (void *) 1);	
	Process *p7 = new Process("Process 7",(rand() % 5) + 1);   
	p7->Fork((VoidFunctionPtr) TestPrioritySchedulingAndSwitching, (void *) 1);	
	Process *p8 = new Process("Process 8",(rand() % 5) + 1);   
	p8->Fork((VoidFunctionPtr) TestPrioritySchedulingAndSwitching, (void *) 1);	
	    
}
