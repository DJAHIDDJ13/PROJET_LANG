prog=eval

$(prog): utility.o eval.o main.o struct.h
	gcc -g -Wall main.o utility.o eval.o -o $(prog)

main.o: main.c
	gcc -g -Wall -c main.c

utility.o: utility.c utility.h struct.h
	gcc -g -Wall -c utility.c

eval.o: eval.c eval.h struct.h
	gcc -g -Wall -c eval.c

editeur:
	geany *.c *.h makefile &
	
clean:
	rm -f *.o $(prog)
