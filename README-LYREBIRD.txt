Sonal Unadkat 	
Assignment 3				
301110334
sunadkat@sfu.ca
CMPT 300  D100
Instructor: Brian G Booth
TA: Scott Kristjanson

Lyrebird Files Included in submit.zip
	server.c					main c file of execution of server
	client.c 					main c file of execution of client
	decrypt.c					code for the decryption/ included character map
	decrypt.h					decryption header file					
	processhandling.c 			holds function calls for the server such as cleaning up process, and common function calls
	memwatch.h					memwatch header file, detects memory leaks(memwatch.c provided by marker)
	makefile					

About:
	Lyrebird is a program that is intended to be a cryptography program that is able read from a configuration file, intialize a scheduling type and open encrypted tweet files, and return a decrypted output to a file which is described by the user.  Due to the uncertainty of string lengths, the use of the dynamic had arrays have been used, as well as many C- functions that can be found in string.h. 

Program Requirments : 

	1. Tweets must be maximum characters of 140
	2. They must be properally encrypted tweets
	3. The program will read in a a tweet until:
						1. 140 Characters have been reached
						2. End of File Character has been reached
						3. New Line Character has been reached
						- First Come First Serve = 'fcfs'

	5. Each line must be in correct format with a space between the encrypted_filename and decrypted_filename.
	6. Each line in the configuration file must be less than 2048 characters to be ensure it will work correctly.

Relevant Makefile Commands and Execution:

	They are many relevant commands that can be used due to the inclusion of a makefile when compiling Lyrebird. The following compiling linux commands can be used to compile and build lyrebird, as well as clean and remove the older files decrypted tweets. The first step is to move into the directory that Lyrebird can be found on our terminal.

			
	1. Compiling Lyrebird : To use lyrebird, the user must compile lyrebird on their UNIX terminal using the following command:

	   Command:   	     $ make Lyrebird                 or            $ make all 

	2. After compilation, to run Lyrebird Server, we can execute this program by typing into the command line parameters the executable file along with the file you that contains and the txt file you would like to output to.  You will see if done correctly, an IP address and Port of the Lyrebird Server
		
       Command:          $ ./lyrebird.server configuration_filename.txt output.txt

    3. To Run, the client remotely or on the same machine, all that need to be done is to open a a new terminal, and type in the command lines the server's IP address and Port

    	Command:         $./lyrebird.client IP_Address PORT

     Lyrebird will have created your decrypted_text_file.txt into our working directory and will have the decrypted tweet stored. Lyrebird will no display any other message until and unless an error has occured.  If you would like to remove all the executable material off our current directory after Lyrebird has decrypted our file, you can do so by:

        Command:          $ make clean 


   													 Thank you for using Lyrebird!


Sources and References:

	The following sources have been used as an aid to help design Lyrebird Assignment 1:
	1. Horner Method: CMPT 225 - Geoffrey Tien - Hash Table Lecture: Slide: Using Horner Rule for Polynomial Evaluation
	2. Modular Exponentation: http://ww3.algorithmdesign.net/sample/ch10-crypto.pdf    	
							  http://www.mycodeschool.com/videos/modular-exponentiation-using-recursion	
							  https://portfolio2430.wordpress.com/homeworks/modular-exponentiation/

	The following sources have been used as an aid to help design Lyrebird Assignment 2:
	1. WaitPid = http://linux.die.net/man/2/waitpid
	2. Fork =  Source 1: BrianG Booth Lecture Notes Source 2: CMPT 300 Course Text , Source 3:
	3. Ctime = http://www.tutorialspoint.com/c_standard_library/c_function_ctime.htm

	The following sources have ben used as an aid to help design Lyrebird Assignment 3:
	1. Select() = http://www.tutorialspoint.com/unix_system_calls/_newselect.htm
				http://stackoverflow.com/questions/19373741/using-select-with-pipes
	2. Pipes = http://cboard.cprogramming.com/c-programming/49919-pipes-c.html
				
	The following sourches have been used as an aid to help design Lyrebird Assignment 4:
	1. http://beej.us/guide/bgnet/output/html/singlepage/bgnet.html . Setting up Sockets, and Using Select for multi-client server
	2. http://linux.die.net/man/3/getifaddrs
	3. http://www.binarytides.com/multiple-socket-connections-fdset-select-linux/
	4. http://www.cs.dartmouth.edu/~caell/cs50/socketprogramming.html
