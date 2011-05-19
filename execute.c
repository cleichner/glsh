/********************
 * execute.c
 *
 * For executing things.
 */

#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "parser.h"
#include "execute.h"
#include "builtins.h"

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

    if( (pid = fork()) < 0 ){
        fprintf(stderr, "fork error\n");
    }

    else if( pid == 0 ) {
        commanditem* command = command_to_execute->contents;

        char* input = command_to_execute->input;
        if(input){
            int input_fd = open(input, O_RDONLY);
            if( input_fd == -1 ){
                fprintf(stderr, "Couldn't open input file \"%s\" for reading", input);
            }

            if((dup2(input_fd, STDIN_FILENO) == -1) ){
                fprintf(stderr, "Couldn't assign input file \"%s\" to stdin", input);
            }

            close(input_fd);
        }

        char* output = command_to_execute->output;
        if(output){
            int output_fd = open(output, O_WRONLY | O_CREAT,  S_IRUSR | S_IWUSR | 
                                                              S_IRGRP | S_IWGRP | 
                                                              S_IROTH | S_IWOTH );
            if( output_fd == -1 )
                fprintf(stderr, "Couldn't open output file \"%s\" for writing", output);
            if((dup2(output_fd, STDOUT_FILENO) == -1) ){
                fprintf(stderr, "Couldn't assign output file \"%s\" to stdout", output);
            }

            close(output_fd);
        }

        char** argv = build_argv(command);
        execvp(argv[0], argv);
        run_builtin(argv[0], argv);
        exit(0);

        free(argv);
        return 127;
    }

    if( (pid = waitpid( pid, &status, 0)) < 0)
        fprintf(stderr, "waitpid error\n");

    return 0;

}
