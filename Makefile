
CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -lm 
LDFLAGS=
EXEC=simulation

all: simulation

simulation: main.o struct.o RR.o maxSNR.o PF.o initialisation.o distribution.o
	gcc -o simulation main.o RR.o maxSNR.o PF.o initialisation.o distribution.o $(CFLAGS) && rm -rf *.o
struct.o: struct.h
	gcc -c struct.h
RR.o: RR.h RR.c distribution.h initialisation.h
	gcc -c RR.c
maxSNR.o: maxSNR.h maxSNR.c distribution.h initialisation.h
	gcc -c maxSNR.c
PF.o: PF.h PF.c distribution.h initialisation.h
	gcc -c PF.c
initialisation.o: initialisation.c initialisation.h  
	gcc -c initialisation.c
distribution.o: distribution.h distribution.c initialisation.h
	gcc -c distribution.c
clean:
	rm -rf *.o
