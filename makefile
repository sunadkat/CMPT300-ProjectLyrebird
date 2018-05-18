# 
#/* 
#onal Unadkat 	
#Assignment 2				
#301110334
#sunadkat@sfu.ca
#CMPT 300  D100
#Instructor: Brian G Booth
#TA: Scott Kristjanson
#
#
#
.SUFFIXES: .h .o .c

CCMAIN = client.c server.c
CC = gcc
CCOPTS = -g
LIBS = -lm
OBJS1 = client.o decrypt.o processhandling.o memwatch.o
OBJS2 = server.o memwatch.o
CCEXEC2 = lyrebird.server
CCEXEC1 = lyrebird.client
MEMWATCH = -DMEMWATCH
MW_STDIO = -DMW_STDIO

all:	$(CCEXEC1) $(CCEXEC2) 

$(CCEXEC1):	$(OBJS1) makefile 
	
	$(CC) $(CCOPTS) $(OBJS1) -o $@ $(LIBS)

$(CCEXEC2):	$(OBJS2) makefile 
	
	$(CC) $(CCOPTS) $(OBJS2) -o $@ $(LIBS)



%.o:	%.c
	
	$(CC) -c $(CCOPTS) $(MEMWATCH) $(MW_STDIO) $<

run:	all
	./$(CCEXEC)


clean:
	rm -f $(OBJS1)
	rm -f $(OBJS2)
	rm -f $(CCEXEC2)
	rm -f $(CCEXEC1)
	rm -f core
