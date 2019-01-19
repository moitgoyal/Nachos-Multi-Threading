# Nachos-Multi-Threading

•	Objective:
Assignment must implement multithreading in nachos by creating processes which will provide encapsulation to threads and kernel will deal with processes only and process will deal with threads having separate scheduler for threads and processes where processes are scheduled according to their priority and threads within a process are scheduled according FIFO. There will be context switch between processes based on the time quantum.
•	What Has been Developed:
To implement this assignment there has been multiple changes which are summarized below
•	A process class was created to run the processes.
•	In process class exactly, same functions are added which are in thread class  such as Fork, sleep, finish, yield etc. and added few new functions to implement Join, Terminate, create child processes, fork new threads for the process.
•	These functions are added in the process class to schedule the processes and maintain the life cycle of the processes just like threads and processes are managed by kernel and threads are managed by process.
•	A new scheduler class was created to schedule processes according to priority for which sorted list is used to store the processes.
•	In process class a new list of type process in included to keep track of all the child processes of the parent process and priority variable is added to add the processes in the process scheduler according to priority
•	In sleep function of thread class if there are no thread remaining to run for current process then current process is switched with the next process in the scheduler of kernel.
•	For Termination of the process a new function is added in the process class which will terminate all the threads in that process scheduler’s ready list and after termination of all threads that process is terminated.
•	For Join in process class a list is created to keep ids of all the child process and every child process will have a variable which will have the id of the parent process.
•	When join is called on  a process then this process is blocked by calling sleep and will be blocked until all child processes of this process are completed and after all child processes are finished this process will start running again.
•	For time quantum there has been change in kernel, timer and alarm class and in kernel class we are getting the value of the quantum from command line argument and this quantum is passed to alarm class and from alarm class it is passed to timer class where it used for time ticks to context switch between process.
•	In create child process function we are getting the priority and function ptr of the parent process and creating child process.
•	In threadtest class multiple test cases are created to demonstrate the multithreading of the process with priority scheduling with Terminate and Join functionality on processes.
•	How to Test:
To test it user, need to go to nachos/code/build.linux directory and run below mentioned commands.
1.	make clean
2.	make depend
3.	make
4.	./nachos -K
•	Output
Output of the assignment will show multiple test cases showing Process forking, Joining, Termination, Thread forking, child process creation, priority scheduling, context switch based on time quantum.
