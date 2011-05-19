/********************
 * builtins.h
 *
 * Headers for built-in functions.
 */

int builtin_chdir(char** argv);
int builtin_exit(char** argv);

int run_builtin(char* command, char** argv);
