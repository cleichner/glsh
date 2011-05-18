#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include "tokenizer.h"
#include "parser.h"
#include "common.h"

#define MAXLINE 4096

int main(void){
    char buffer[MAXLINE];
    pid_t pid;
    int status;
    
    printf("$ ");
    while( fgets(buffer, MAXLINE, stdin) != NULL ){
        if( buffer[strlen(buffer) - 1] == '\n' )
            buffer[strlen(buffer) - 1] = '\0';

        struct tokenized_node* tokenized_command = tokenize(buffer);
        command* parsed_command =
        create_parse_tree(tokenized_command);

        for (iter(parsed_command))
        {
            printf("New command:\n");
            commanditem* command_contents = parsed_command->contents;
            for (iter(command_contents))
            {
                printf("%s\n", (char*)(command_contents->contents));
            }
            if (parsed_command->input != NULL)
                printf("\tInput: %s\n", parsed_command->input);
            if (parsed_command->output != NULL)
                printf("\tOutput: %s\n", parsed_command->output);
            if (parsed_command->output_append)
                printf("\tAppend\n");
            if (parsed_command->background)
                printf("\tBackground this\n");
        }

        char* command = tokenized_command->contents;

        if( (pid = fork()) < 0 ){
            fprintf(stderr, "fork error\n");
        }

        else if( pid == 0 ) {
            execlp( command, command, NULL);
            fprintf(stderr, "couldn't execute: %s\n", command);
            exit(127);
        }

        if( (pid = waitpid( pid, &status, 0)) < 0)
            fprintf(stderr, "waitpid error\n");

        printf("$ ");
    }

    return 0;
}

