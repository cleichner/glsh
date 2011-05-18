/********************
 * execute.c
 *
 * For executing things.
 */

#include <stdio.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "execute.h"
#include "parser.h"
/**************************
 * char** build_argv(commanditem* command)
 * takes a command and creates an array of strings containing its arguments
 *
 * caution: will leak hellaciously if it's returned array isn't freed
 */
char** build_argv(commanditem* command){
    commanditem* current = command;
    int size = 1;
    char** argv = malloc(sizeof(char*));
    *argv = (char*)(current->contents);

    while( (current = current->next) ){
        size++;
        argv = realloc(argv, size * sizeof(char*));
        argv[size-1] = (char*)(current->contents);
    }

    ++size;
    argv = realloc(argv, size * sizeof(char*));
    argv[size-1] = NULL;

    return argv;
}
            
/***********************
 * int execute(command*)
 *
 * Given a command struct from parser.c, executes it.
 */
int execute(command* command_to_execute){

    int status;
    pid_t pid;

    if(!command_to_execute)
        return 0;

    commanditem* command = command_to_execute->contents;

    if( (pid = fork()) < 0 ){
        fprintf(stderr, "fork error\n");
    }

    else if( pid == 0 ) {
        char** argv = build_argv(command);
        execvp(argv[0], argv);
        free(argv);
        fprintf(stderr, "couldn't execute: %s\n", argv[0]);
        return 127;
    }

    if( (pid = waitpid( pid, &status, 0)) < 0)
        fprintf(stderr, "waitpid error\n");

    return 0;

}
