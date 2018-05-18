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

int main(int argc, char ** arg1) {
// Parameter Error Handling
   if( argc != 3) {
        perror("Usage: ./lyrebird  <configuration_file>.txt  <log_file>.txt");
        exit(EXIT_FAILURE);
    }
    FILE *configuration_file = fopen(arg1[1],"r");
    FILE *log_file = fopen(arg1[2], "w"); // open to be written on.
    if (configuration_file == NULL || log_file == NULL) { // Error-Handling
        PrintTime();
        printf("Lyreber Server Process ID: %d is Terminating \n",getpid());
        exit(EXIT_FAILURE);
     }
// Setting UP the Socket and Server Address
    char* check2;
    int socket1;
    struct sockaddr_in server_address;
    char *fstring = malloc(sizeof(char)* 2050);
    struct sockaddr_in *getIP;
    struct ifaddrs *host_computer_address;

    socket1 = socket(AF_INET,SOCK_STREAM, 0);
    if(socket1 == -1 ) {
    	PrintTime();
    	printf("Lyrebird Server PID: %d cannot create it's Socket \n", getpid());
    	exit(1);
    }
    memset(&server_address, '0', sizeof(server_address)); // clear previous memeory
  	server_address.sin_family = AF_INET; // because we are using TCP networking we can set the family for AF_INET
//								   Search for Random Port and IP address for Server using getifadders() function call
  	server_address.sin_port = 0;
    getifaddrs(&host_computer_address);
 
  	struct ifaddrs * ifa = host_computer_address;
  	while(ifa) { // Iterates through the linked list for the wildard port with the family set to AF_INET
  		if(ifa->ifa_addr->sa_family == AF_INET) {
  			getIP = (struct sockaddr_in * ) ifa->ifa_addr;
  			check2 = inet_ntoa(getIP->sin_addr);
  			server_address.sin_addr.s_addr= inet_addr(check2); // sets the server_address we want to bind to the port with our non-loopback address.
  		} ifa = ifa ->ifa_next;
  	}
  	freeifaddrs(host_computer_address);
    memset(server_address.sin_zero, '\0',sizeof(server_address.sin_zero)); // clean previous memeory 
    if(bind(socket1, (struct sockaddr *)&server_address, sizeof(server_address)) < 0 ){ // this will bind the socket and the server address
    	PrintTime();
    	printf("Lyrebird Server PID: %d cannot bind to the socket", getpid());
    	exit(1);
    }
    listen(socket1,5);
    socklen_t size = sizeof(server_address);
    getsockname(socket1, (struct sockaddr *) &server_address, &size); // allows us to get the random port number. 
    memset(fstring,'\0',sizeof(fstring));
    int job;
    PrintTime();
	printf("IP address: %s  Port : %d \n", inet_ntoa(server_address.sin_addr),(int) ntohs(server_address.sin_port) ); // http://stackoverflow.com/questions/3060950/how-to-get-ip-address-from-sock-structure-in-c		
// Start of Adding New Connections and Handing out Tasks in First Come First Server	
	fd_set master; // All the socket descripters including the main socket will be managed through the master set
	fd_set check; // Use for our while loop, to continously check for activity using the Select() function call. 
	FD_SET(socket1, &master); // Adds the main socket to the master socket set. 
	int number_client = 0;
	int fdmax = socket1; // keeps track of the last socket descripter. 
	char buffer[2050];
	fgets(fstring,2050, configuration_file); // gets the first job
	int work = 1;
	int addrlen = 0;
	char * client_IP;
	int balanced = 0;

	while(1) {
		if(work == 0 && number_client == 0 ) break; // will only clean up when there is no more work to do and all the clients have disconnected. 
		check = master;
		int ready = select(fdmax+1, &check, NULL, NULL, NULL); // waits until activity has occured. 
		int i = 0;
		while(i <= fdmax) {
			if(FD_ISSET(i, &check)) {
				if(i == socket1) {
					addrlen = sizeof(server_address);
				 	 int newfd = accept(socket1, (struct sockaddr *)&server_address, &addrlen);
				 	if(newfd == -1) {
				 		perror("Accept");
				 	} else {
				 		FD_SET(newfd,&master);
				 		if(newfd > fdmax)	{
				 			fdmax = newfd;
				 		}
				 		++number_client;
				 		PrintTimeFile(log_file);
				 		fprintf(log_file, " Successfully Connected to Lyrebird client %s\n", inet_ntoa(server_address.sin_addr));

				 	}
				} else {
					 addrlen = sizeof(server_address);
					 getpeername(i,(struct sockaddr *)&server_address, &addrlen); // Gets the incoming client 's IP address.'
					 client_IP = inet_ntoa(server_address.sin_addr);
					 int bytes;
					 int len = strlen(fstring);
					 fstring[len] = '\0';		
					 write(i, fstring, len +1 );
					if(bytes = recv(i, buffer, sizeof(buffer), 0) <= 0) {
						if(work == 0) { // work is done there fore we expect diconnections
							PrintTimeFile(log_file);
							fprintf(log_file," Lyrebird Client %s has disconnected expectedly\n",client_IP );
						} // exepected
						else {
							PrintTimeFile(log_file);
							fprintf(log_file, "Lyrebird Client %s has disconnected unexpectedly\n",client_IP );
						} // unexepcted
						close(i);
						FD_CLR(i, &master); // remove the disconnected socket descriptors
						--number_client;
					} else { //Client stuff
						if (work == 0) { // manipulate fstring to terminate flag. to send to client to notice to clean up.
							fstring[0] = '+';
							fstring[1] = '\0';
						}
						if(strcmp(buffer, "ready") != 0) { // While we are not receiving the initalization messages.filtering the status messages
							 PrintTimeFile(log_file);
							 fprintf(log_file," The Lyrebird Client %s %s\n",client_IP, buffer );// what we have read is the status message
						 
						}
						
						 char file_nameb[2050];
              			 int k = 0;
              			 while(fstring[k] != ' ') {
              			   file_nameb[k] = fstring[k];
              			   ++k;
              			 }
              			 file_nameb[k] = '\0';
              			 if(file_nameb[0] != '+') {
              			 	PrintTimeFile(log_file);
              			 	fprintf(log_file, " The Lyrebird client %s has been given the task of decrypting %s.\n",client_IP,file_nameb ); 
              			}
						if(!fgets(fstring,2050, configuration_file)) { // Check to see if the next job is available, if not then will flag the server to cleanup
							work = 0;
						}
					}	
				}
						
			} ++i;
		} 
}

 // Close the Server and free variables
	free(fstring);
    fclose(configuration_file); fclose(log_file);
    free(ifa);
    PrintTime();
    printf("Lyrebird Server: PID :%d completed its tasks and is exiting successfully \n", getpid());
    close(socket1);
    exit(0);
 }
