/* mycp - a simple version of cp command
 * Nora Brown
 * Without arguments, mycp will read from standard input and write the same thing back to standard output.
 * With 1 argument, it will read from standard input and write to the file given as the argument.
 * With 2 file arguments, it will copy the first to the second, overwriting anything already there.
 */

//Need to include the following, required in order to use open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
//Need to include this, in order to use system call exit
#include <unistd.h>

using namespace std;

const int BUF_SIZE=10;//amount read at a time

int main(int argc, const char * argv[]){

	int source;//file descriptor, int that keeps track of which file it is
	int destination;
	char buffer[BUF_SIZE]; //where the read charcters will be read to
	int amount_read; //the number of chars read from the file at a time
	
	if (argc > 3){
		write(2, "Too many arguments.\n", 20);
		return(1);
	}
	else
	if (argc == 1){
		source = 0;
		destination = 1;
	}
	else 
	if (argc == 2){
		source = 0;
		destination = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	}
	else{	
		//open takes 2/3 arguments 1.file path, 2. flag(check man)
		source = open(argv[1], O_RDONLY); //open returns -1 on failure
        if (source == -1){
			write(2, "Could not open source.\n", 23);
			return(1);//end with error
		}
		//for destination, if exists overwrite it, if not create it
		//bitwise OR used when more than 1 flag to turn on all of them
		destination = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
        if (destination == -1){
            write(2, "Could not open destination.\n", 28);
        }
	}
	do {
		//read() reads raw bytes 1 at a time, takes 3 arguments
		//1. file to read from, 2. address to store bytes, 3. number of bytes to take in
		amount_read = read (source, &buffer, BUF_SIZE-1);
		if (amount_read>0) 
		{	
			write(destination, &buffer, amount_read); //same args as read
		}
   	} while (amount_read>0);

	if (source>2){
        close(source);
    }
    if (destination>2){
        close(destination);
    }
	return 0;
}
