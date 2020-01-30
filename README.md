# CS153labs
## CS153lab1
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
