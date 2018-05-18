#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <ifaddrs.h>
#include "decrypt.h"
#include "processhandling.h"
#include "memwatch.h"


int main(int argc, char ** arg1) {
// Parameter Error Handling
   if( argc != 3) {
        perror("Usage: ./lyrebird  <IP ADDRESS>  <PORT>");
        exit(EXIT_FAILURE);
    }
// Variable Intitalization for Socket and Server
  int socket_client;
  struct sockaddr_in server_address;
  int port = atoi(arg1[2]);
// Variable Intialization for Pipes and Children
  char status[2050];
  char buffer[2050];
  int numChildProcess = sysconf(_SC_NPROCESSORS_CONF)-1;
  int job = numChildProcess;
  int sendpid =0;
  int parentp[numChildProcess][2]; // Parent Pipe = Parent Writes to the Child
  int childp[numChildProcess][2];  // Child Pipe = Child Writes back to the parent
  pid_t children[numChildProcess];
  int val;
// Connecting to the Server and Socket - Establishing Communication. 
  inet_pton(AF_INET,arg1[1], &server_address.sin_addr);
  server_address.sin_addr.s_addr = inet_addr(arg1[1]); // < IP ADDRESS> from Lyrebird Server
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port); // < PORT> IP ADDRESS from lyrebird Server. 
  socket_client = socket(AF_INET, SOCK_STREAM, 0);
  int address_size = sizeof(server_address);

  if(connect(socket_client,(struct sockaddr *)&server_address, address_size) != -1 ) { // ONLY if connection to server is successful, then we will set up pipes/ children
    PrintTime();
    printf("Lyrebird client PID : %d connected to server  %s  Port : %d \n", getpid(), inet_ntoa(server_address.sin_addr),(int) ntohs(server_address.sin_port));
    // Set up the Pipes for Parent Child Communcation // Re-used from Assignment 3. 
    int i = 0;
     while(i < numChildProcess) { // Creates all the Pipes
             if(pipe(parentp[i]) < 0) {
                 PrintTime();
                 printf("Pipe Error. Parent Process ID # %d", getpid());
                 exit(1);
             }
             if(pipe(childp[i]) < 0) {
                 PrintTime();
                 printf("Pipe Error. Parent Process ID # %d", getpid());
                 exit(1);
             }
             i++;
         }
         // Set up for all the Children Processes and Code for decryption. 
         int j = 0;
         while(j < numChildProcess) { // Creates all Children
             children[j] = fork() ;
             if(children[j] < 0) { // Fork Failed
                 PrintTime();
                 printf("Fork Error. Parent Process ID # %d", getpid());
                 CleanUP(j);
                 exit(1);
             } 

// Child Code --------------------------------------------------------------------------------------------------------------------------------------- 

             if(children[j] == 0) {   
              char child_buffer[2050];
              char child_status[2050];
              close(socket_client); // each child needs to close its end to the file
                 int i = 0; // Closing all unncesssary Pipe ends
                 while(i < numChildProcess) {
                     if(i != j) {
                         close(parentp[i][0]);
                         close(childp[i][1]);
                     }
                     close(parentp[i][1]);
                     close(childp[i][0]);
                     i++;
                 }
               
               sendpid = getpid(); // Sending the Parent, the pid of the "ready" Child. 
               sprintf(child_status, "ready"); // Initalizing the FCFS, will not be written to log file, Server will test against the status first.
               write(childp[j][1], child_status, sizeof(status)); // FCFS Intialization - each child is ready to begin with. 
               while(read(parentp[j][0], child_buffer, sizeof(buffer))){
                  int ok = Decryption(child_buffer); // Algorithm can be found in decrypt.c
                  char file_nameb[2050]; // Retrieving File name for sprintf.
                  int k = 0;
                while(child_buffer[k] != ' ') {
                    file_nameb[k] = child_buffer[k];
                    ++k;//
                   }
                   file_nameb[k]= '\0';
                   
                  if(ok == 0) { // successful decryptiong
                    sprintf(child_status, "has successfully decrypted %s in Process %d", file_nameb, getpid());
                    write(childp[j][1], child_status, sizeof(status));   
                    
                 } else if (ok == -2) { // file retreival errors
                    sprintf(child_status, "is unable to Open %s in Process %d", file_nameb, getpid());
                    write(childp[j][1], child_status, sizeof(status));
                 } 
                  else { // unsuccessful decryption, major error,
                     PrintTime();
                     printf("Lyrebird Child Process %d has detected a major error and is Terminating \n", getpid());
                     CleanUP(j);
                     exit(1);
                 }
                  
               } // Close all parent pipes - Reused from Assignment 3
                   close(parentp[j][0]);
                   close(childp[j][1]);
                   exit(0);
               }    
        ++j;
      }   

 // Parent Code ..................................................................................................................................
 // Reused- from assignment 3. 
   int k = 0; 
    while(k < numChildProcess) { // close all uneccessary pipe ends
        close(parentp[k][0]);
        close(childp[k][1]);
        ++k;
    }
    int ready;
    int pid;
    fd_set check;
    int flag = 1;

  while(1) { // will run continously until the flag is reached
       int x;
       if(flag == 0) {
        int i = 0;
        while(i < (numChildProcess - 1))  {
            if(i != x) {
            read(childp[i][0],status, 2050);
            write(socket_client, status, 2050);  // sends the status back to the socket to log. 
          }
          ++i;
         }
        break;
      } 
       int size = strlen(buffer);
       buffer[size] = '\0';
       FD_ZERO(&check);
       int set_pipe = 0;
       while(set_pipe < numChildProcess) {
           FD_SET(childp[set_pipe][0], &check);
           ++set_pipe;
        }
        ready = select(FD_SETSIZE +1, &check, NULL ,NULL, NULL);
        
        if(ready < 0) {
          CleanUP(numChildProcess);
          PrintTime();
          printf("Select Function Call has Detected an Error. Parent ID #%d Terminating ", getpid());
          exit(0);
        } else if(ready) { // There is a child process ready for a job
         
           x = 0;
          while(x < numChildProcess) {

            if(FD_ISSET(childp[x][0], &check)) { // Finds the child 
                pid = children[x];
                int bytes = read(childp[x][0],status, 2050); // Reads the Status from the Child
                write(socket_client, status, bytes);  // sends the status back to the socket to log. 

                int read2 = read(socket_client,buffer, sizeof(buffer));
                if(read2 == 0 || strcmp(buffer, "+") == 0)  flag = 0;// recv = 0, means that connection is closed or the flag is received, therefore no more work to do.
                else {
                  write(parentp[x][1], buffer, read2);
                }
                break;
            }  
            ++x;
          }
       } // end of while each line read from

     
  }
  } else { // If connection to Server is unsuccessful, Exit With Error Notifying User. 
    perror("Failed Connection to Lyrebird.Server");
    exit(1);    
  }
      int z = 0;
      while(z < numChildProcess) {
        close(parentp[z][1]);
        close(childp[z][0]);
         ++z;       
       }
       CleanUP(numChildProcess); //Algorithm Found in processhandling.c
       PrintTime();
       printf("Lyrebird client : PID %d, completed its tasks and is exiting succesfully\n", getpid());
       close(socket_client);
	     exit(0);
}