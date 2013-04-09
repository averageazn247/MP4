# makefile

all: dataserver simpleclient 

reqchannel.o: reqchannel.H reqchannel.C
	g++ -c -g reqchannel.C

dataserver: dataserver.C reqchannel.o 
	g++ -o dataserver dataserver.C reqchannel.o 

simpleclient: simpleclient.C reqchannel.o semaphore
	g++ -w -o simpleclient simpleclient.C reqchannel.o  semaphore.c 
	
semaphore: semaphore.c
	g++ -w -c -g semaphore.c

clean:
	make
	rm *.o