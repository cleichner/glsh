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

// opens the file named input if input is non-null
// redirects the input of the current process from the file
void redirect_stdin(char* input){
    if(input){
        int input_fd = open(input, O_RDONLY);
        if( input_fd == -1 ){
            fprintf(stderr, "Couldn't open input file \"%s\" for reading\n", input);
        }

        if((dup2(input_fd, STDIN_FILENO) == -1) ){
            fprintf(stderr, "Couldn't assign input file \"%s\" to stdin\n", input);
        }

        close(input_fd);
    }
}

// opens a new file named output if output is non-null
// redirects the output of the current process to the new file
void redirect_stdout(char* output){
    if(output){
        int output_fd = open(output, O_WRONLY | O_CREAT,  S_IRUSR | S_IWUSR | 
                                                            S_IRGRP | S_IWGRP | 
                                                            S_IROTH | S_IWOTH );
        if( output_fd == -1 )
            fprintf(stderr, "Couldn't open output file \"%s\" for writing\n", output);
        if((dup2(output_fd, STDOUT_FILENO) == -1) ){
            fprintf(stderr, "Couldn't assign output file \"%s\" to stdout\n", output);
        }

        close(output_fd);
    }
}
            
/***********************
 * int execute(command*)
 *
 * Given a command struct from parser.c, executes it.
 */
int execute(command* command_to_execute){

//    int status;
    pid_t pid;

    if(!command_to_execute)
        return 0;

    // Fork. Parent returns.
    if( (pid = fork()) < 0 ){
        fprintf(stderr, "fork error\n");
    }
    if (pid > 0) return 0;

    int fd[2];
    bool first = true;

    while (command_to_execute)
    {

        // If has next, we need to set up pipes
        if (command_to_execute->piped_to)
        {
            bool tmp_first = false;
            if (first)
            {
                first = false; tmp_first = true;
            }

            if (pipe(fd) < 0)
            {
                fprintf(stderr, "pipe failed A\n");
            }
            if( (pid = fork()) < 0 ){
                fprintf(stderr, "fork error\n");
            }

            if (pid == 0)
            {
                close(fd[1]);
                if((dup2(fd[0], STDIN_FILENO) == -1) ){
                    fprintf(stderr, "couldn't redirect stdin to pipe A\n");
                }
                command_to_execute = command_to_execute->piped_to;
                continue;
            }

            if (pid > 0)
            {
                close(fd[0]);
                if((dup2(fd[1], STDOUT_FILENO) == -1) ){
                    fprintf(stderr, "couldn't redirect stdout to pipe A\n");
                }
            }

        }



             //redirect_stdin( command_to_execute->input );
            //redirect_stdout( command_to_execute->output );

            char** argv = build_argv( command_to_execute->contents );
        printf("My pid is %d and command to execute is %s\n", pid, argv[0]);

        // Execute if: we are A parent process, or we are the last process.
        if (pid > 0 || !(command_to_execute->piped_to))
        {

            printf("I am PID %d and am executing\n", pid);

            execvp(argv[0], argv);
            run_builtin(argv[0], argv);
            exit(0);

            free(argv);
            return 127;
            
        }
//        if( (pid = waitpid( pid, &status, 0)) < 0)
//            fprintf(stderr, "waitpid error\n");

        command_to_execute = command_to_execute->piped_to;

    }

    return 0;

}
