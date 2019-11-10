all:	programa.out

programa.out:
	gcc *.c -o programa.out

run:	programa.out
	./programa.out
