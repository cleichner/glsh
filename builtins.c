/********************
 * builtins.c
 *
 * Implementations of shell built-ins.
 */

#include <string.h>
#include <unistd.h>

#include "builtins.h"

int builtin_chdir(char* argv)
{

    char* directory = argv[1];

    // TODO get home from environment (?)
    if (!directory)
        chdir("/home");
    else
        chdir(directory);

    return 0;

}

int builtin_exit(char** argv)
{

    char* exit_code = argv[1];

    if (exit_code != NULL)
        exit(atoi(exit_code));
    else
        exit(0);
    
    return 0;
}

int run_builtin(char* command, char** argv)
{
    if (strcmp("chdir", command) == 0 ||
        strcmp("cd", command) == 0)
    {
        return builtin_chdir(argv);
    }

    if (strcmp("exit", command) == 0)
    {
        return builtin_exit(argv);
    }

    return -1;
}

