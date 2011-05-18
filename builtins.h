/********************
 * builtins.h
 *
 * Headers for built-in functions.
 */

int builtin_chdir(char* directory);

int run_builtin(char* command, char** argv);
