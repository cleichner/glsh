/********************
 * execute.c
 *
 * For executing things.
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "execute.h"
#include "parser.h"

/***********************
 * int execute(command*)
 *
 * Given a command struct from parser.c, executes it.
 */
int execute(command* command_to_execute)
{

    int status;
    pid_t pid;

    commanditem* command = command_to_execute->contents;

    char* command_name = (char*)command->contents;

    if( (pid = fork()) < 0 ){
        fprintf(stderr, "fork error\n");
    }

    else if( pid == 0 ) {
        execlp( command_name, command_name, NULL);
        fprintf(stderr, "couldn't execute: %s\n", command_name);
        return 127;
    }

    if( (pid = waitpid( pid, &status, 0)) < 0)
        fprintf(stderr, "waitpid error\n");

    return 0;

}
