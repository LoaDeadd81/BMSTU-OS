task1.out : task1.c
	gcc -o $@ $?

task2.out : task2.c
	gcc -o $@ $?

stack.out : stack/*.c stack/*.h
	gcc -o $@ $?

queue.out : queue/*.c queue/*.h
	gcc -o $@ $?

task3.out : task3.c stack.out queue.out
	gcc -o $@ $<

task4.out : task4.c
	gcc -o $@ $?

task5.out : task5.c
	gcc -o $@ $?

clean :
	rm -f *.o *.out