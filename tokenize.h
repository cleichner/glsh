

#ifndef __TOKENIZE_H_GUARD__
#define __TOKENIZE_H_GUARD__

struct tokenized_tree_node {
    char is_container;
    char delimeter;
    void* contents;
    struct tokenized_tree_node* next;
};

struct tokenized_tree_node* tokenize(char* string);

void free_tree(struct tokenized_tree_node* node_to_free);

#endif
