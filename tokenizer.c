#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "common.h"
#include "tokenizer.h"

char delimeters[] = { ' ', '`', '"', '(', ')', '{', '}', '|', '&', '>', '<', '\0' };

static bool is_delimeter(char to_test);
static char* find_next_delimeter(char* string);
struct tokenized_node* create_tokenized_node(char* token_string, int token_length);

/**
 * char* find_next_delimeter(char*)
 *
 * Finds and returns the address of the first character after string
 * that should be delimeted at.
 *
 * This is a little hackish, since we're doing this without properly
 * implementing/using regular expressions and simply assuming that
 * whitespace is whitespace and non-delimeters can be clumped together,
 * etc.
 *
 * Ignores characters after a backslash.
 */
static char* find_next_delimeter(char* string)
{
    /**
     * Special case as a result of not using regular expressions:
     * If first character is a delimeter, we just return the next
     * character rather than looking for a future delimeter.
     */
    if (is_delimeter(*string))
        return string + 1;

    char* string_iterator = string;

    while (!is_delimeter(*string_iterator))
    {
        if (*string_iterator == '\\' &&
            (*(string_iterator+1) != '\0'))
        {
            string_iterator++;
        }

        string_iterator++;
    }

    return string_iterator;
}

/**
 * bool is_delimeter(char)
 *
 * Returns whether or not the given char is one of the delimeters
 * as specified in the delimeters array.
 */
static bool is_delimeter(char to_test)
{
    char* delimeter_iterator = delimeters;

    while (*delimeter_iterator)
    {
        if (to_test == *delimeter_iterator)
            return true;

        delimeter_iterator++;
    }

    if (to_test == '\0')
        return true;

    return false;
}

/**
 * Tokenize a string given the defined set of tokens in this file.
 * Returns a pointer to the head node of a list of tokens.
 */
struct tokenized_node* tokenize(char* string)
{

    char* token_start = string;
    char* token_end;

    // Fake first node to avoid special casing later
    struct tokenized_node fake_node;
    fake_node.next = NULL; fake_node.contents = NULL;
    struct tokenized_node* current_node = &fake_node;

    while (1)
    {

        token_end = find_next_delimeter(token_start);

        int token_size = (token_end - token_start) / sizeof(char);

        // Skip spaces
        if (token_size == 1 && *token_start == ' ')
        {
            token_start++;
            continue;
        }

        // Avoid an unnecessary "word"
        if (token_size == 1 && *token_start == '\0')
        {
            break;
        }

        else
        {

            struct tokenized_node* new_node
                = create_tokenized_node(token_start, token_size);
            
            current_node->next = new_node;
            current_node = new_node;
        }

        if (*token_end == '\0') break;

        token_start = token_end;

    }

    return fake_node.next;

}

struct tokenized_node* create_tokenized_node(char* token_string, int token_length)
{
    struct tokenized_node* new_node
        = malloc(sizeof(struct tokenized_node));

    new_node->next = NULL;

    new_node->contents = malloc((token_length + 1) * sizeof(char));
    strncpy(new_node->contents, token_string, token_length);
    new_node->contents[token_length] = '\0';

    return new_node;
}

void print_list(struct tokenized_node* list_to_print)
{
    struct tokenized_node* node_iterator;

    for (iterator(node_iterator, list_to_print))
    {
        printf("%s\n", (char*)(node_iterator->contents));
    }

}

void free_list(struct tokenized_node* list_to_free)
{
    // TODO
    // OM NOM NOM


}
