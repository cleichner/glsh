#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAXLINE 4096

int main(void){
    char buffer[MAXLINE];
    pid_t pid;
    int status;
    
    printf("$ ");
    while( fgets(buffer, MAXLINE, stdin) != NULL ){
        if( buffer[strlen(buffer) - 1] == '\n' )
            buffer[strlen(buffer) - 1] = '\0';
        
        if( (pid = fork()) < 0 ){
            fprintf(stderr, "fork error\n");
        }

        else if( pid == 0 ) {
            execlp( buffer, buffer, '\0' );
            fprintf(stderr, "couldn't execute: %s\n", buffer);
            exit(127);
        }

        if( (pid = waitpid( pid, &status, 0)) < 0)
            fprintf(stderr, "waitpid error\n");

        printf("$ ");
    }

    return 0;
}

