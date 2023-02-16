#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 50 // maximum number of arguments allowed

// function declarations
void parseInput(char *input, char *splitWords[]);
int executeCommand(char *args[]);
void changeDirectories(char *path);

int main() {
    char input[100]; // input buffer
    char *args[MAX_ARGS + 1]; // argument buffer, +1 for the NULL terminator
    char *netid = "<your_netid>"; // replace with your own netid
    int status;

    while (1) {
        printf("_exampleShell%s$ ", netid);
        fgets(input, sizeof(input), stdin); // read user input
        parseInput(input, args); // split input into separate arguments

        if (args[0] == NULL) {
            continue; // empty input, prompt again
        }

        if (strcmp(args[0], "cd") == 0) {
            // change directory command
            if (args[1] == NULL) {
                fprintf(stderr, "Error: no directory specified\n");
            } else {
                changeDirectories(args[1]); // change to specified directory
            }
        } else if (strcmp(args[0], "exit") == 0) {
            // exit command
            break; // exit the loop and end the program
        } else {
            // all other commands
            status = executeCommand(args); // execute the command
            if (status != 0) {
                fprintf(stderr, "Error: command failed\n");
            }
        }
    }

    return 0;
}

void parseInput(char *input, char *splitWords[]) {
    int wordInd = 0;
    splitWords[0] = strtok(input, " \n");
    while (splitWords[wordInd] != NULL && wordInd < MAX_ARGS) {
        splitWords[++wordInd] = strtok(NULL, " \n");
    }
    splitWords[wordInd] = NULL; // add NULL terminator
}

int executeCommand(char *args[]) {
    pid_t pid = fork(); // fork a child process
    if (pid == -1) {
        // fork failed
        fprintf(stderr, "Error: fork failed\n");
        return 1;
    } else if (pid == 0) {
        // child process
        if (execvp(args[0], args) == -1) {
            // execvp failed
            fprintf(stderr, "Error: execvp failed\n");
            exit(EXIT_FAILURE);
        }
    } else {
        // parent process
        int childStatus;
        waitpid(pid, &childStatus, 0); // wait for child to finish
        if (WIFEXITED(childStatus)) {
            return WEXITSTATUS(childStatus); // return exit status of child process
        } else {
            return 1; // child process terminated abnormally
        }
    }
}

void changeDirectories(char *path) {
    if (chdir(path) == -1) {
        // chdir failed
        fprintf(stderr, "Error: path not found\n");
    }
}
