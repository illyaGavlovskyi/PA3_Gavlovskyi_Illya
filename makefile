make: sched.o
	gcc -o sched -Wall -pthread sched.c
clean: 
	rm *o sched