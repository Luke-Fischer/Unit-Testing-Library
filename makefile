all: libutest.so SampleMain

libutest.so: functions.o
	gcc -shared -o libutest.so functions.o

functions.o: functions.c Header.h 	
	gcc -Wall -std=c11 -g -c -fpic -D_POSIX_C_SOURCE=200809L functions.c

SampleMain: SampleMain.o functions.o
	gcc -Wall -std=c11 -g SampleMain.o functions.o -D_POSIX_C_SOURCE=200809L -o SampleMain

SampleMain.o: SampleMain.c Header.h 	
	gcc -Wall -std=c11 -g -c -D_POSIX_C_SOURCE=200809L SampleMain.c -o SampleMain.o

clean:
	rm -f *.o libutest.so SampleMain