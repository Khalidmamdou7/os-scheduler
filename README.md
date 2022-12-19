# Operating System Scheduler Project
This project is a simulation of a simple operating system scheduler. It is written in C to be compiled and run on a Linux machine. The project was written for "CMPN303 - Operating Systems" course at Cairo University.
The project is still under development and is not yet complete.

## How to run
To run the project, you need to have a Linux machine with gcc compiler installed, and `make` utility (optional to use the Makefile). 

First, clone the repository:
```
git clone https://github.com/Khalidmamdou7/os-scheduler.git
```
Then, compile the project using the Makefile:
```
cd os-scheduler
make -B
```
if you don't have `make` installed, you can compile the project manually by running the commands in the Makefile build section.

Finally, run the project:
```
make run
```