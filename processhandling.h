/* 
Sonal Unadkat 	
Assignment 1				
301110334
sunadkat@sfu.ca
CMPT 300  D100
Instructor: Brian G Booth
TA: Scott Kristjanson
 
*/



#ifndef _PROCESSHANDLING_H_

#define _PROCESSHANDLING_H_


#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "memwatch.h"

void RoundRobin (char* fstring, FILE* encrypted_filename1, int numChildProcess, int childp[][2], int parent[][2], pid_t children[], int sendpid);
void fcfs(char* fstring, FILE* encrypted_filename1, int numChildProcess,int childp[][2], int parentp[][2], pid_t children[], int sendpid);
void PrintTime () ;
void CleanUP(int count);
void PrintTimeFile(FILE * file) ;


#endif