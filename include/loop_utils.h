#ifndef LOOP_UTILS_H
#define LOOP_UTILS_H

// A void argument inside the parentheses means that the function explicitly takes no arguments.
// Accidental passing of argument(s) throws a compiler error.
// In versions of C >= 23, specifying void inside () is redundant, but still allowed.

void seaSh_loop(void);
char* seaSh_read_line(void);
char** seaSh_split_line(char*);
int seaSh_launch(char**);
int seaSh_execute(char**);

#endif