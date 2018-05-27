prog=eval

$(prog): main.o struct.h
	gcc -g -Wall main.o -o $(prog)

main.o: main.c
	gcc -g -Wall -c main.c

editeur:
	geany *.c *.h makefile &
	
clean:
	rm -f *.o $(prog)
