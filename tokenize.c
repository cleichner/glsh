#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tokenize.h"

char delimeters[] = { ' ', '`', '"', '\0' };

int is_delimeter(char to_test);
char* find_first_delimeter(char* string);

/**
 * char* find_first_delimeter(char*)
 *
 * Finds and returns the address of the first character after string
 * that is a delimeter. This method assumes that there will eventually
 * be a delimeter, even if it is only the null character.
 */
char* find_first_delimeter(char* string)
{
    char* string_iterator = string;

    while (!is_delimeter(*string_iterator))
    {
        string_iterator++;
    }

    return string_iterator;
}

/**
 * int is_delimeter(char)
 *
 * Returns whether or not the given char is one of the delimeters
 * as specified in the delimeters array.
 */
int is_delimeter(char to_test)
{
    char* delimeter_iterator = delimeters;

    while (*delimeter_iterator)
    {
        if (to_test == *delimeter_iterator)
            return 1;

        delimeter_iterator++;
    }

    if (to_test == '\0')
        return 1;

    return 0;
}

/**
 *
 * end_character is assumed to be a valid delimeter.
 * token_ended_at is an output.
 */
struct tokenized_tree_node* tokenize_recursive(
    char* string,
    char end_character,
    char** token_ended_at)
{

    char* token_start = string;
    char* token_end;

    // Fake first node to avoid special casing later
    struct tokenized_tree_node fake_node;
    struct tokenized_tree_node* current_node = &fake_node;

    while (1)
    {

        token_end = find_first_delimeter(token_start);

        int token_size = (token_end - token_start) / sizeof(char);

        // Create new node
        struct tokenized_tree_node* new_node
            = malloc(sizeof(struct tokenized_tree_node));

        if (token_size == 0)
        {

            if (*token_start == ' ')
            {
                token_start++;
                continue;
            }

            else if (*token_start == end_character)
            {
                new_node->next = NULL;
                break;
            }

            // Ending our command prematurely is bad.
            else if (*token_start == '\0')
            {
                // TODO free allocated memory.
                return NULL;
            }

            // Recurse!
            else
            {

                new_node->is_container = 1;
                new_node->delimeter = *token_start;

                struct tokenized_tree_node* child_node = tokenize_recursive(
                    (++token_start),
                    new_node->delimeter,
                    &token_end);

                if (child_node == NULL)
                    return NULL;

                new_node->contents = child_node;
            }
        }

        else
        {

            new_node->is_container = 0;

            new_node->contents = malloc((token_size + 1) * sizeof(char));
            strncpy(new_node->contents, token_start, token_size);
            ((char*)(new_node->contents))[token_size] = '\0';
            
        }

        current_node->next = new_node;
        current_node = new_node;
        token_start = token_end + 1;

        if (*token_end == end_character)
        {
            break;
        }

    }

    *token_ended_at = token_end;
    return fake_node.next;

}

struct tokenized_tree_node* tokenize(char* string)
{
    char* junk;
    return tokenize_recursive(string, '\0', &junk);
}

void print_tree(struct tokenized_tree_node* node_to_print, int depth)
{
    struct tokenized_tree_node* node_iterator = node_to_print;

    while (node_iterator)
    {

        if (node_iterator->is_container)
        {
            print_tree((struct tokenized_tree_node*)(node_iterator->contents),
                depth + 1);
        }

        else
        {

            int i;
            for (i = 0; i < depth; i++)
            {
                printf("\t");
            }

            printf("%s\n", (char*)(node_iterator->contents));

        }

        node_iterator = node_iterator->next;
    }
}

void free_tree(struct tokenized_tree_node* node_to_free)
{
    // TODO
    // OM NOM NOM
}
