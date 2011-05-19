

#ifndef __TOKENIZER_H_GUARD__
#define __TOKENIZER_H_GUARD__

struct tokenized_node {
    char* contents;
    struct tokenized_node* next;
};

struct tokenized_node* tokenize(char* string);

void free_token_list(struct tokenized_node* node_to_free);

#endif
