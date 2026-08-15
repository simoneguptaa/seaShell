#include <string.h> // strcmp(), strtok()
#include <sys/wait.h> // waitpid() and associated macros
#include <unistd.h> // chdir(), fork(), exec(), pid_t
#include <stdlib.h> // malloc(), realloc(), free(), exit(), execvp(), EXIT_SUCCESS, EXIT_FAILURE
#include <stdio.h> // fprintf(), printf(), stderr, getchar(), perror()
#include "loop_utils.h"
#include "builtins.h"

int main(int argc, char** argv){
  // Load config files, if any.

  // Run command loop.
  seaSh_loop();

  // Perform any shutdown / cleanup.

  return EXIT_SUCCESS;
}
