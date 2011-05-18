#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokenizer.h"
#include "parser.h"
#include "common.h"
#include "execute.h"

#define MAXLINE 4096

int main(void){
    char buffer[MAXLINE];
    
    printf("$ ");
    while( fgets(buffer, MAXLINE, stdin) != NULL ){
        if( buffer[strlen(buffer) - 1] == '\n' )
            buffer[strlen(buffer) - 1] = '\0';

        struct tokenized_node* tokenized_command = tokenize(buffer);
        command* parsed_command =
            create_parse_tree(tokenized_command);

        command* command_iterator;
        for (iterator(command_iterator, parsed_command))
        {
            printf("New command:\n");
            commanditem* command_contents = command_iterator->contents;
            for (iter(command_contents))
            {
                printf("%s\n", (char*)(command_contents->contents));
            }
            if (command_iterator->input != NULL)
                printf("\tInput: %s\n", command_iterator->input);
            if (command_iterator->output != NULL)
                printf("\tOutput: %s\n", command_iterator->output);
            if (command_iterator->output_append)
                printf("\tAppend\n");
            if (command_iterator->background)
                printf("\tBackground this\n");
        }

        execute(parsed_command);

        printf("$ ");
    }

    return 0;
}

