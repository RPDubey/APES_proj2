#*******************************************************************************
   #Filename:Makefile
   #Brief: makefile for main.c
   #Author:Ravi Dubey
   #Date:4/27/2018
 #*****************************************************************************/

main.elf:main.c comTask.o taskThread.o
	gcc -g -o $@ $^ -lpthread -lm -lrt

comTask.o: comTask.c tasks.h
	gcc -c -g -o $@ $< -lpthread

taskThread.o: taskThread.c tasks.h
	gcc -c -g -o $@ $< -lpthread

clean:
	rm -f *.o *.elf logfile.txt
