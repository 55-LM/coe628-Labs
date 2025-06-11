/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Alvi Alam
 *
 * Created on February 7, 2025, 12:56 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define INPUT_SIZE 100
/*
 * 
 */
int main(int argc, char** argv) {
    char input[INPUT_SIZE]; //used to store user input
    char *args1[10]; //used to store first set of arguments
    char *args2[10]; //used to store second set of arguments 

    while (1) {
        printf("Command> ");
        //if the input isn't formatted properly, exit program
        if (fgets(input, INPUT_SIZE, stdin) == NULL) {
            exit(1); 
        }
        
        input[strcspn(input, "\n")] = '\0'; //the newline char is removed from the input by replacing it with a null character

        char *pipePos = strchr(input, '|'); //locate the position of the pipe character
        //if pipe char is not found print error message
        if (pipePos == NULL) {
            fprintf(stderr, "pipe symbol not found \n");
            continue;
        }

        *pipePos = '\0';  //split the input into two commands at the position of the pipe
        char *cmd1 = input; //where the first command is located before the pipe
        char *cmd2 = pipePos + 1; //and the second after the pipe

        while (*cmd1 == ' ') cmd1++; //remove the space preceeding the first command
        while (*cmd2 == ' ') cmd2++; //remove the space preceeding the second command
        
        char *end1 = cmd1 + strlen(cmd1) - 1; //point to the last character of the first command
        while (end1 > cmd1 && *end1 == ' ') { //while there are spaces at the end of the command 
            *(end1--) = '\0'; //replace each following space with a null character
        }

        char *end2 = cmd2 + strlen(cmd2) - 1; //point to the last character of the second command
        while (end2 > cmd2 && *end2 == ' ') { //while there are spaces at the end of the command
            *(end2--) = '\0'; //replace each following space with a null character
        }

        //using tokens to retrieve the arguments from the commands of the user input
        int i = 0;
        char *token = strtok(cmd1, " "); //scan the input (first command) for a space character, which retrieves the first token (argument)
        while (token) { //
            args1[i++] = token; //each token(argument) is stored in the first argument array
            token = strtok(NULL, " "); //terminate the current token by replacing the space char with a null char and get the next token
        }
        args1[i] = NULL; //once there are no more tokens in the first command, the last index of the first argument array is set as NULL
        
        i = 0;
        token = strtok(cmd2, " "); //scan the input (second command) for a space character, which retrieves the first token (argument) in the command after the pipe
        while (token) {
            args2[i++] = token; //each token for the second command (after pipe) is stored in the second argument array
            token = strtok(NULL, " ");
        }
        args2[i] = NULL; //once there are no more tokens in the second command, the last index of the second argument array is set as NULL

        int pipefd[2]; //used to create an array of two integers that are used to store file descriptors that the pipe call creates
        pipe(pipefd); //create new pipe
        
        //Write output of the first command line arguments to the write end of the pipe (first child process)
        if (fork() == 0) { //create a child process by forking 
            dup2(pipefd[1], STDOUT_FILENO); //the standard output of this process is written to the pipe
            close(pipefd[0]); //the read end of the pipe is closed
            close(pipefd[1]); //the write end of the pipe is closed
            execvp(args1[0], args1); //executes the arguments specified by the first command line arguments
            perror("execution failed"); 
            exit(EXIT_FAILURE);
        }
        
        //Read the input (output of the first command line arguments) from the read end of the pipe and execute the second command line arguments (second child process)
        if (fork() == 0) { //used to create another child process
            dup2(pipefd[0], STDIN_FILENO); //the standard input of this process is read from the pipe
            close(pipefd[1]); //the write end of the pipe is closed
            close(pipefd[0]); //the read end of the pipe is closed
            execvp(args2[0], args2); //executes the arguments specified by the second argument array which reads from the standard input and processes the output of the first command
            perror("execution failed"); 
            exit(EXIT_FAILURE);
        }
        close(pipefd[0]); //close the read end of the pipe
        close(pipefd[1]); //close the write end of the pipe
        wait(NULL); //wait until first child process is completed before returning to parent process
        wait(NULL); //wait until second child process is completed before returning to parent process
    }
}

