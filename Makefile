all:client server

server: server.o makeMsg.o weatherMsg.o findCity.o
	gcc -o server server.o makeMsg.o weatherMsg.o findCity.o

server.o: Server/server.c Server/server.h
	gcc -Wall -pedantic -std=c99 -g -c Server/server.c

makeMsg.o: Server/makeMsg.c Server/server.h
	gcc -Wall -pedantic -std=c99 -g -c Server/makeMsg.c

weatherMsg.o: Server/weatherMsg.c Server/server.h
	gcc -Wall -pedantic -std=c99 -g -c Server/weatherMsg.c

findCity.o: Server/findCity.c Server/server.h
	gcc -Wall -pedantic -std=c99 -g -c Server/findCity.c	



client: client.o resolve.o fresh.o
	gcc -o client client.o resolve.o fresh.o

client.o:  Client/client.c Client/client.h
	gcc -Wall -pedantic -std=c99 -g -c Client/client.c

resolve.o: Client/resolve.c Client/client.h
	gcc -Wall -pedantic -std=c99 -g -c Client/resolve.c

fresh.o: Client/fresh.c Client/client.h
	gcc -Wall -pedantic -std=c99 -g -c Client/fresh.c


clean: 
	rm -f client server *.o

