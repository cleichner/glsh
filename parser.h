/**********************
 * parser.h
 */


#ifndef __PARSER_H_GUARD__
#define __PARSER_H_GUARD__

#include <stdbool.h>
#include "tokenizer.h"

enum node_type {
    COMMAND,
    ARGUMENT,
    REPLACEMENT 
};

typedef struct parsed_command_item_node {
    enum node_type type;
    struct parsed_command_item_node* next;
    void* contents;
} commanditem;

typedef struct parsed_command_node {
    struct parsed_command_node* piped_to;
    struct parsed_command_item_node* contents;
    bool background;
    char* input;
    char* output;
    bool output_append;
} command;

command* create_parse_tree(struct tokenized_node* tokenized_command);
void free_parse_tree(command* parse_tree);

#endif
