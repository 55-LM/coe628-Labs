/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.c
 * Author: Alvi Alam
 *
 * Created on January 29, 2025, 8:28 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

/* 
 *
 */

#define MAX_INPUT_SIZE 100
#define MAX_ARG_COUNT 10

int main(int argc, char** argv) {
    char command[MAX_INPUT_SIZE]; //Used to store the user command
    char *argPtr[MAX_ARG_COUNT]; //Used to store pointers to arguments in commands
    int background; //Used as a flag to check if process should run in the background
    
    //The shell is looped so that commands can be executed one after another
    do {
        //Initialize
        int index = 0; //Index of command given by user is initialized to 0
        int argPtrIndex = 0;
        int background = 0; //Background process flag is initialized to 0
        
        //Prompt the user
        printf("Your Command> ");
        
        //The input line is read/"collected" char by char by using the getChar() method
        while ((command[index] = getchar()) != '\n' && index < MAX_INPUT_SIZE - 1) { //Check first whether the last character is not a new line character and whether the index doesn't exceed the max input size
            if (index == 0) { //At the first index
                argPtr[argPtrIndex++] = &command[index]; //Store the first argument
            } else if (command[index] == ' ') { //For the index that is a space character
                command[index] = '\0'; //Replace the space in the command with null to differentiate arguments and
                argPtr[argPtrIndex++] = &command[index + 1]; //Store the following argument
            }
            index++; //Increment the index
        }
        command[index] = '\0'; //User input is terminated by \0 
        argPtr[argPtrIndex] = NULL; //Argument pointer is terminated when last item is null

        //Check if the user entered "exit" as the command argument before forking
        if (strncmp(command, "exit", 4) == 0 && command[4] == '\0') {
            printf("Goodbye!\n");
            break;
        }

        //Check if the command argument contains '&' and set background process flag
        if (index >= 1 && command[index - 1] == '&') {
            command[index - 1] = '\0'; //Remove the '&' character from the command
            background = 1;
        }

        //Create a child process
        pid_t pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Child process not be created \n");
        } else if (pid == 0) { //Child process
            execvp(argPtr[0], argPtr);//The command is executed 
            perror("Execution failed");
            exit(EXIT_FAILURE);
        } else { //Parent process
            if (!background) {
                wait(NULL); //
            }
        }
    } while (1);

    return EXIT_SUCCESS;
}