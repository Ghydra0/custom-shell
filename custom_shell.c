#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#define MAX 100

// parse input line into arguments
void parse_command(char *line, char **args) {
    for (int i = 0; i < 64; i++) {
        args[i] = strsep(&line, " ");

        if (args[i] == NULL) {
            break;
        }
    }
}

//main loop
int main() 
{
    // grab current PID and PPID;
    char *args[64];
    int status;

    int myPID = getpid();
    int parentPID = getppid();
    int pid = 0;
    char userInput[MAX];

    while(1) 
    {

        //custom color and prompt
        printf("\033[1;35mZion's Shell> \033[0m");

        // look for input
        fgets (userInput, MAX, stdin);

        // Remove the newline character added by fgets
        userInput[strcspn(userInput, "\n")] = '\0';

        // parse input into command and arguments
        parse_command(userInput, args);

        // skip if not command entered
        if (args[0] == NULL) {
            continue;
        }

        // input logic for custom commands
        if (strcmp(args[0], "help" ) == 0) {
            printf("Custom shell available commands:\n");
            printf("help - Shows this help message\n");
            printf("exit - Exits the custom shell\n");
            printf("pid - Prints the current process ID of the shell\n");
            printf("ppid - Prints the parent process ID of the shell\n");
            printf("cwd - Prints the current working directory\n");
        }
        else if (strcmp(args[0], "exit" ) == 0) {
            printf("user said exit\n");
            exit(0);
        }
        else if (strcmp(args[0], "pid" ) == 0) {
            printf("Shell PID: %d\n", myPID);
        }
        else if (strcmp(args[0], "ppid" ) == 0) {
            printf("Shell Parent PID: %d\n", parentPID);
        }
        else if (strcmp(args[0], "cwd" ) == 0) { // same as pwd
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("Current Working Directory: %s\n", cwd);
            } else {
                perror("getcwd() error");
            }
        }      
         // standard commands handled by fork
        else {
            pid = fork();
            // validate fork
            if (pid < 0)
                perror("Fork failed");

            else if (pid == 0) {
                // child executes command
                if (execvp(args[0], args) < 0) {
                    perror("Command execution failed");
                }
                exit(EXIT_FAILURE); //exit and signal failure
            }
            else {
                // Parent waits for child to finish
                waitpid(pid, &status, 0);
            }
        }
    }


    return 0;
}