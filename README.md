# CS153labs
## CS153lab1
### a) Changing the exit system call signature to void exit(int status)

In order to make our necessary changes to the exit system call we will alter the following files: 
'''
user.h 
defs.h
sysproc.c 
proc.c 
'''
and all user space programs that uses the exit system call. In these user space programs we will simply change the parameter of 'exit()' to 'exit(0)'. 
