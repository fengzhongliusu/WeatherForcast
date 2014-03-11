client: main.o resolve.o fresh.o
	gcc -o client main.o resolve.o fresh.o


main.o: main.c client.h
	gcc -Wall -pedantic -std=c99 -g -c main.c

resolve.o: resolve.c client.h
	gcc -Wall -pedantic -std=c99 -g -c resolve.c

fresh.o: fresh.c client.h
	gcc -Wall -pedantic -std=c99 -g -c fresh.c

clean:
	rm -f client *.o