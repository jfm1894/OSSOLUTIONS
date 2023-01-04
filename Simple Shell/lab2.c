#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define PRMTSIZ 255
#define MAXARGS 63
#define EXITCMD "exit"

void print_arg(char *arg);

int main(void)
{
    int argc = 0;
    int index = 0;
    int pid_array[5] = {0,0,0,0,0};
    while (1)
    {
        char input[PRMTSIZ + 1] = {0x0};
        char *ptr = input;
        char *args[MAXARGS + 1] = {NULL};

        // prompt
        printf("JeffShell$ ");
        fgets(input, PRMTSIZ, stdin);

        // ignore empty input
        if (*ptr == '\n')
            continue;

        // convert input line to list of arguments
        for (int i = 0; i < sizeof(args) && *ptr; ptr++)
        {
            if (*ptr == ' ')
                continue;
            if (*ptr == '\n')
                break;
            for (args[i++] = ptr; *ptr && *ptr != ' ' && *ptr != '\n'; ptr++)
                ;
            *ptr = '\0';

            argc = i;
        }

        printf("Received user commands: ");
        for (int i = 0; i < argc; i++)
        {
            print_arg(args[i]);
        }
        printf("\n");



	//code to execute when exit command is recieved
       if (strcmp(args[0],"exit") == 0)
       {	
	       return 0;
       }
       //code to execute if showpid is recieved
       else if(strcmp(args[0],"showpid") == 0)
	 {
		//code for showing process ids
		for (int i=0; i < 5; i++)
		{
			printf("%d ",pid_array[i]);
			printf("\n");
		}
	 }
       //code to create and exec child process
	int return_value;
	int pidrvalue;
	return_value = fork();
	if (return_value == 0) 
	{
		//in child execute command or print error if it is not a valid command
		pidrvalue = execvp(args[0],args);
		exit(return_value);
		if( pidrvalue < 1)
		{
			printf("Error command could not be executed");
			printf("\n");
			exit(return_value);
		}	
	}
	else
	{
		//in parent assign pid array at current index to child pid from when we called fork()
		pid_array[index] = return_value;
		waitpid(return_value,0,0);
	}	
	//increment index that stores child pids
	index = index + 1;
	if ( index > 5){index = 0;}
		
    }	
}

void print_arg(char *arg)
{
    int i = 0;
    while (arg[i] && arg[i] != '\0' && arg[i] != ' ' && arg[i] != '\n')
    {
        printf("%c", arg[i++]);
    }
    printf(" ");
}
