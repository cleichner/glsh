#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "tokenize.h"

#define MAXLINE 4096

int main(void){
    char buffer[MAXLINE];
    pid_t pid;
    int status;
    
    printf("$ ");
    while( fgets(buffer, MAXLINE, stdin) != NULL ){
        if( buffer[strlen(buffer) - 1] == '\n' )
            buffer[strlen(buffer) - 1] = '\0';

        struct tokenized_tree_node* tree = tokenize(buffer);

        if (tree == NULL)
            printf("INVALID\n");
        else
            print_tree(tree, 0);

        char* command = tree->contents;

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

