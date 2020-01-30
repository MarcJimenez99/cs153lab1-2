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
and all user space programs that uses the exit system call. In these user space programs we will simply change the parameter of `exit()` to `exit(0)`. In the following images are where changes have been made in the previously listed files.

- user.h
![user.h](https://github.com/MarcJimenez99/cs153labs/blob/master/cs153pictures/lab1/pic1.1.JPG)
- defs.h
- sysproc.c
- proc.c 

