/*
Sonal Unadkat  
Assignment 2	 	 	 	 
301110334
sunadkat@sfu.ca
CMPT 300 D100
Instructor: Brian G Booth
TA: Scott Kristjanson
 
*/
 
#include "decrypt.h"
#include "memwatch.h"
#include "processhandling.h"

void PrintTime () {
   time_t parent_fail;
   time(&parent_fail);
   char * date = ctime(&parent_fail);
   int legnth = strlen(date);
   date[legnth-1] = '\0';
   printf("[%s]", date );

}
void PrintTimeFile(FILE * file) {
   time_t parent_fail;
   time(&parent_fail);
   char * date = ctime(&parent_fail);
   int legnth = strlen(date);
   date[legnth-1] = '\0';
   fprintf(file, "[%s ]", date);
}

void CleanUP(int count) {
  int status;   
  pid_t returnstatus;
  while(count > 0) {
         returnstatus = waitpid(0, &status, 0);
         if(WEXITSTATUS(status) != 0) {
      time_t child_error;
      time(&child_error);
      char * date = ctime(&child_error);
      int legnth = strlen(date);
      date[legnth-1] = '\0';
      printf("[%s] Child Process ID: #%d did not terminate successfully  \n",date, returnstatus);       
         }
         --count;
   }
  }
