make: sched.o
	gcc -o sched -pthread -Wall  sched.c
clean: 
	rm *o sched