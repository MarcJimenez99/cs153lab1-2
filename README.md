# CS153 Lab1 & Lab2 (Exit, Wait, Waitpid & WNOHANG // Priority Scheduling & Aging Priority)
## Lab 1 - Exit, Wait, Waitpid, WNOHANG
### a) Changing the exit system call signature to void exit(int status)

In order to make our necessary changes to the exit system call we will alter the following files: 
```
user.h 
defs.h
sysproc.c 
proc.c 
```
and all user space programs that uses the exit system call. In these user space programs we will simply change the parameter of `exit()` to `exit(0)`. In the following images are the changes made in the previously listed files.

| user.h | defs.h |
|--------|--------|
|<img src="https://github.com/MarcJimenez99/cs153labs/blob/master/cs153pictures/lab1/pic1.1.JPG">|<img src="https://github.com/MarcJimenez99/cs153labs/blob/master/cs153pictures/lab1/pic1.2.JPG">|

| sysproc.c | proc.c |
|-----------|--------|
|<img src="https://github.com/MarcJimenez99/cs153labs/blob/master/cs153pictures/lab1/pic1.3.JPG">|<img src="https://github.com/MarcJimenez99/cs153labs/blob/master/cs153pictures/lab1/pic1.4.JPG">|

In both `user.h` and `defs.h` the only changes we needed to make was to now add the `int status` parameter. 

Originally in `sysproc.c` the function would just return the process's exit function as no input was needed. Now that we want to pass in `int status` we make use of the helper function `argint` in order to receive the input from the user stack. This is due to the fact when the process calls the exit function the event causes an interrupt which traps the processs to the kernel, raising the priviledge to kernel mode. Since the `int status` variable only exists on the user stack we need to use `argint` helper function to point toward the user stack to retrieve the argument.

For our changes in `proc.c` we now pass in the `int status` within the parameter. This status variable will now be held within our `proc` structure, thus we need to make sure when we create our proc pointer `*curproc` we set it's status to the status passed in. This added line will be `curproc->status = status`. 

### b) Changing the wait system call signature to int wait(int* status)

In order to make our necessary changes to the wait system call we will alter the following files: 
```
user.h 
defs.h
sysproc.c 
proc.c 
```
Similarly to our changes in the exit system call signature, we will change all cases of `wait()` to `wait(0)`. In the following images are the changes made in the previously listed files.

| user.h | defs.h |
|--------|--------|
|<img src="https://github.com/MarcJimenez99/cs153labs/blob/master/cs153pictures/lab1/waituser.JPG">|<img src="https://github.com/MarcJimenez99/cs153labs/blob/master/cs153pictures/lab1/waitdefs.JPG">|

| sysproc.c | proc.c |
|-----------|--------|
|<img src="https://github.com/MarcJimenez99/cs153labs/blob/master/cs153pictures/lab1/waitsysproc.JPG">|<img src="https://github.com/MarcJimenez99/cs153labs/blob/master/cs153pictures/lab1/waitproc.JPG">|

In both `user.h` and `defs.h` we will change the parameter of `wait()` to take in an `int*` thus changing the function to `int wait(int*)`. 

Originally in our `sysproc.c` file the function `sys_wait` would be called when the process would call the `wait()` function. This would then just return the `wait()` function due to no parameters being passed by the user. Now that we are passing in an `int*` we need to use the helper function known as `argptr()` in order to receive the argument from the user stack. Once receiving the argument we will then just return the user process function `wait(int*)`.

For our `proc.c` function we want to make sure we reset the `int* status` of the process once the first child process has finished running and is ready to terminate. Our `wait()` function currently iterates through all the given processes through the `ptable` structure. Once we've found a process that is a child of the `curproc` and is in the `zombie` state, then we will want to deallocate the memory that the `zombie` process is in possession of. Since we are adding the `int* status` parameter to our processes, we need to also deallocate the status as well. So in our `zombie` processwe want to change the passed in `*status` to equal the iterating `proc *p` status. 

### c) Adding a waitpid system call: int waitpid(int pid, int *status, int options)

In order to add this system call we need to make the necessary changes to the following files in order to create the command. These files are:
```
sysproc.c
proc.c
defs.h
user.h
syscall.h
syscall.c
usys.S
```
Similar to the previous changes we've made on both the `wait` and `exit` function, we will simply insert a new line in `defs.h` and `user.h` declaring our new system call `waitpid`.

| user.h | defs.h |
|--------|--------|
|<img src="https://github.com/MarcJimenez99/cs153labs/blob/master/cs153pictures/lab1/waitpiduser.JPG">|<img src="https://github.com/MarcJimenez99/cs153labs/blob/master/cs153pictures/lab1/waitpiddefs.JPG">|

In `sysproc.c` we will use `argint` and `argptr` in order to receive the three necessary arguments from the user stack. These variables are `int pid`, `int *status`, and `int options`. Once receiving the arguments we will return our new `waitpid` function in `proc.c`. 

In `proc.c` we will use most of the same code as our function `wait`. However, `waitpid` does not wait for the first terminated child process, instead it waits for a specific child process to end. So instead of checking if our iterative process `p` is the child process of the `curproc`, we will instead look to see if `p->pid == pid`. If it does not then we will continue to iterate through the `ptable`. We also will change the `havekids` variable to `matchingPID` to match the functionality of `waitpid`.  

| sysproc.c | proc.c |
|--------|--------|
|<img src="https://github.com/MarcJimenez99/cs153labs/blob/master/cs153pictures/lab1/waitpidsysproc.JPG">|<img src="https://github.com/MarcJimenez99/cs153labs/blob/master/cs153pictures/lab1/waitpidproc.JPG">|

In the following files: `syscall.c`, `syscall.h`, and `usys.S`. We will simply implement the necessary code to have our `waitpid` become a system call.

| usys.S |
|--------|
<img src="https://github.com/MarcJimenez99/cs153labs/blob/master/cs153pictures/lab1/waitpidusys.JPG">|

| syscall.c | syscall.h |
|--------|--------|
|<img src="https://github.com/MarcJimenez99/cs153labs/blob/master/cs153pictures/lab1/waitpidsyscallc.JPG"><img src="https://github.com/MarcJimenez99/cs153labs/blob/master/cs153pictures/lab1/waitpidsyscallc1.JPG">|<img src="https://github.com/MarcJimenez99/cs153labs/blob/master/cs153pictures/lab1/waitpidsyscallh.JPG">|

### d) Example program: lab1tests and test bench

In order to create an example program, we have to make similar changes to the makefile as we did for `waitpid`. These changes include:

<img src="https://github.com/MarcJimenez99/cs153labs/blob/master/cs153pictures/lab1/lab1testsmakefile0.JPG">
<img src="https://github.com/MarcJimenez99/cs153labs/blob/master/cs153pictures/lab1/lab1testsmakefile1.JPG">

The example code itself is in the following picture:

<img src="https://github.com/MarcJimenez99/cs153labs/blob/master/cs153pictures/lab1/lab1testsexamplecode.JPG">

The example code will create an array of type int called `int pid[5]`. It will run through a loop and call fork 5 times to create 5 child processes. Then in the following loop it will iterate once more and reap each child process in order. 

<img src = "https://github.com/MarcJimenez99/cs153labs/blob/master/cs153pictures/lab1/lab1examplesuccess.JPG">

For the test bench we ran a given program and was able to receive the necessary output:

<img src = "https://github.com/MarcJimenez99/cs153labs/blob/master/cs153pictures/lab1/lab1testbench.JPG">

### e) WNOHANG and CELEBW02 test

In order to add the WNOHANG flag to our `waitpid` function we added an if statement that checked if options had a value of 1 which we set defined WNOHANG to be. If options did equal 1 we then released the `lock` on the `ptable` in order to allow future iterations on the ptable and returned 0 to prevent the suspension of the parent process. 

<img src="https://github.com/MarcJimenez99/cs153labs/blob/master/cs153pictures/lab1/lab1wnohangproc.JPG">

The following picture depicts the output of this on the test bench:

<img src="https://github.com/MarcJimenez99/cs153labs/blob/master/cs153pictures/lab1/lab1WNOHANG.JPG">

## Lab 2 - Priority Scheduling & Aging Priority
### a) Priority Scheduling & Aging Priority

In order to add priority scheduling we are going to make changes in the following files:
```
sysproc.c
proc.h
proc.c
defs.h
user.h
syscall.h
syscall.c
usys.S
```
First we will edit `proc.h` and add the `int priority` variable. This is so all of our processes can carry and track a priority number. We have set it up where 31 is the highest priority while 0 is the lowest. Next we added two system calls known as `int getpriority()` and `void setpriority(int)`. This is so we can test our changes in our user processes. In the following files you can see the changes made to create an `int` priority variable and our two system calls `int getpriority()` and `void setpriority(int)`.

| proc.h | usys.S |
|--------|--------|
|<img src=https://github.com/MarcJimenez99/cs153lab1-2/blob/master/lab2pictures/prochlab2.JPG>|<img src=https://github.com/MarcJimenez99/cs153lab1-2/blob/master/lab2pictures/getsetUSYS.JPG>|

| defs.h | user.h |
|--------|--------|
|<img src="https://github.com/MarcJimenez99/cs153lab1-2/blob/master/lab2pictures/getsetDefsH.JPG">|<img src=https://github.com/MarcJimenez99/cs153lab1-2/blob/master/lab2pictures/getsetUserH.JPG>|

| syscall.h | sysproc.c |
|--------|--------|
|<img src=https://github.com/MarcJimenez99/cs153lab1-2/blob/master/lab2pictures/getsetSysCallH1.JPG>|<img src=https://github.com/MarcJimenez99/cs153lab1-2/blob/master/lab2pictures/getsetSysprocC.JPG>|

| syscall.c |
|--------|
|<img src=https://github.com/MarcJimenez99/cs153lab1-2/blob/master/lab2pictures/getsetSysCallC1.JPG><img src=https://github.com/MarcJimenez99/cs153lab1-2/blob/master/lab2pictures/getsetSysCallC2.JPG>|

Now once setting that up we will now edit `proc.c` in order to enable priority scheduling and aging of priority. First we will change the `static struct proc* allocproc(void)`. This function will find unused processes and initialize their variables so now that we are giving our processes a priority variable we will want that initialized too. So in order to do that we added to the function to make it so initalized unused processes when found are given a priority of 15.

<img src ="https://github.com/MarcJimenez99/cs153lab1-2/blob/master/lab2pictures/getsetProcCAllocProc.JPG">

Next we will edit the `void schduler(void)` function. Originally the function ran infintely and found `RUNNABLE` processes within the `ptable` and ran them. This would take the first `RUNNABLE` process. We want it to take the highest priority process instead. In order to do that we created a new `struct proc *p1` in order to iterate through the table a second time. We also created a `struct proc *highP` pointer that will point at the process in the `ptable` with the highest priorirty. So once we identified the first `RUNNABLE` process and the highest priority process we will then run the highest priority process by setting `p = highP`. 

The second functionality our scheduler has is the aging of priority. We want to make it so when processes run they decrement in priority if they are context switched. In addition, processes that are `RUNNABLE` and do not get a chance to run will increment in their priority. We do that by taking all processses that are a lower priority than our highest process and our `RUNNABLE` and incrementing their priority by 1. If they are at the max priority of 31 then we set it to 31. Similarly after we have ran the process we will then take the process that ran and decrement it by 1. If it is at the lowest priority of 0 then we keep it at 0.

<img src ="https://github.com/MarcJimenez99/cs153lab1-2/blob/master/lab2pictures/LAB2SCHDULER.JPG">
