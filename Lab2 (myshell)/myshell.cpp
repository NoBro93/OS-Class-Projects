/* myshell - a simple unix shell program for Unix
 * Nora Brown
 * Reads and executes shell commands
 * If called with an argument, the program will run a script
 * '#' character may be used for comments
 * known bug: comments on the same line as a command not processed correctly
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
using namespace std;

const int MAX_LINE_LEN=1024; //each command line is at most 1023 character long 
const int WORD_LEN=51; //each word in the command line is at most 50 character long 
const int MAX_WORD_NUM=20; //each command has at most 20 words in it 


int main(int argc, char*argv[])
{
	pid_t  pid;
	char * pwd, * host, * usr;
	char cmdline[MAX_LINE_LEN];
    
    //Calling getenv to find the current values of envrironment variables 
	pwd = getenv ("PWD");
	host = getenv ("HOSTNAME");
	usr = getenv ("USER");
	ifstream script;

    if(argc == 2){
		script.open(argv[1]);
		if (!script.is_open()){
			cerr<<"File not found";
			exit(1);
		}
	}

	while (1)
	{
		// Generate a prompt message, using : to end, and () instead of [] 
		if (argc!=2){
			cout <<"(" << usr <<"@"<<host<<" "<<pwd<<"):";
			cin.getline (cmdline,256);
		}
		else {
			script.getline(cmdline, 256);
			if (strlen(cmdline) == 0)
				break;
		}

		char * Word[MAX_WORD_NUM]; //array for command and arguments

		// break cmd lines into an array of words
		int wordCount = 1;
		for (int i=0; i<strlen(cmdline); i++){
			if (cmdline[i]=='#'){ 
            //ignore everything after #
				Word[wordCount-1] = NULL;
				break;
			}
			if (i == 0){ 
                //inside the loop so that # is ignored
                Word[0] = cmdline;
			}
			if (cmdline[i]==' ') {
			// terminate each word at the space
				cmdline[i] = '\0';
				Word[wordCount] = &cmdline[i+1];
				wordCount++;
			}
			if(cmdline[i]=='&'){ //don't include the & or the space before it
				Word[wordCount-1] = NULL;
			}
		}
		//use NULL to indicate the end of Word array
		Word[wordCount] = NULL;

		if (!strcmp (cmdline,"exit"))
			exit(0);

		/* create a child process */
		pid = fork(); //should give 0 in parent, id number in child
		if (pid < 0) { 
			fprintf(stderr, "Fork Failed");
			exit(-1);
		}
		else if (pid == 0) { /* child process */
			execvp(Word[0], Word);
			
			//If we are here, that means execvp fails
			cout <<"Command not found\n";
			exit (1);
		
		}
		else if (cmdline[strlen(cmdline)-1]!='&'){ 
			/* parent will wait for the child to complete */
			wait (NULL);
		}
	}
	if (script)
        script.close();
}
