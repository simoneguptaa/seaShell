#include "builtins.h"
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>

// list of builtin commands, followed by their corresponding function implementations.
char* builtin_str[] = {"cd", "help", "exit"};


// in C, a function name decays to a function pointer, so &seaSh_cd and seaSh_cd are equivalent below.
int (*builtin_func[]) (char**) = {&seaSh_cd, &seaSh_help, &seaSh_exit};

int seaSh_num_builtins(){
  return sizeof(builtin_str) / sizeof(builtin_str[0]);
}

// why a built in cd command is needed:
// running cd externally would: fork the shell process and exec /bin/cd
// the child process's current working directory would change. the child process would run and exit.
// the parent process's cwd would be unchanged.
// every subsequent command, like ls, would inherit the parent's cwd.
int seaSh_cd(char** args){
  if(args[1] == NULL){
    fprintf(stderr, "seaSh: expected argument to be \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0){ // chdir changes the current working directory of the calling process. on success, returns 0. on failure returns -1 and sets errno.
      perror("seaSh");
    }
  }
  return 1;
}

// help is built in because it's shell specific.
int seaSh_help(char** args){
  printf("simone's shell: seaShell");
  printf("type program names and arguments, and hit enter");
  printf("the following commands are built in:\n");

  for (int i = 0; i < seaSh_num_builtins(); i++){
    printf("  %s\n", builtin_str[i]);
  }

  printf("use the man command for information on other programs");
  return 1;
}

// why a built in exit is needed
// exit on a child process would exit the child process's environment, not the parent's.
int seaSh_exit(char** args){
  return 0;
}