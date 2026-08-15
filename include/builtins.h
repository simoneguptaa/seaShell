#ifndef BUILTINS_H
#define BUILTINS_H

extern char* builtin_str[];
extern int (*builtin_func[]) (char**);

// function declarations for built in shell commands:
int seaSh_cd(char** args);
int seaSh_help(char** args);
int seaSh_exit(char** args);
int seaSh_num_builtins();

#endif

// ifndef, define, ndef = inlcude guards. they prevent the file from being included more than once, which can cause errors during compilation.