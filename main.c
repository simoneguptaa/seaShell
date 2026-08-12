#include <string.h>
#include <sys/wait.h>

int main(int argc, char** argv){
  // Load config files, if any.

  // Run command loop.
  seaSh_loop();

  // Perform any shutdown / cleanup.

  return EXIT_SUCCESS;
}

// A void argument inside the parentheses means that the function explicitly takes no arguments.
// Accidental passing of argument(s) throws a compiler error.
// In versions of C >= 23, specifying void inside () is redundant, but still allowed.

void seaSh_loop(void){ 
  char *line;
  char **args;
  int status;

  do {
    printf(">");
    line = seaSh_read_line();
    args = seaSh_split_line(line);
    status = seaSh_execute(args);

    // free() deallocates blocks of memory that were previously allocated dynamically on the heap using malloc(), calloc(), or realloc().
    free(line);
    free(args);

  } while (status);
  
}

// This is a preprocessor macro. It tells the compiler to replace every use of SEASH_RL_BUFSIZE with the literal 1024.
// It is not a variable - no memory is allocated for SEASH_RL_BUFSIZE itself.
// RL stands for read line.
// It means that the buffer size for reading a line of input is 1024 bytes.
// Buffer = staging area/ container for temporary storage.

#define SEASH_RL_BUFSIZE_1024 
char* seaSh_read_line(void){
  int bufsize = SEASH_RL_BUFSIZE;
  int position = 0;
  char* buffer = malloc(sizeof(char) * bufsize);
  int c;

  if(!buffer){
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while(1){

    // Read a character
    c = getchar();

    // If we hit EOF, replace it with a null character and return.
    if(c == EOF || c == '\n'){
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if(position >= bufsize){
      bufsize += SEASH_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if(!buffer){
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }

  // Alternate way to implement the above function: getline()
}

// Simplification: do not allow quoting or backslash escaping in command line arguments.
// Remove this simplification in the future.
// Tokenise the string using whitespace as delimiters.

#define SEASH_TOK_BUFSIZE 64
// \r : carriage return - moves the cursor back to the start of the current line.
// \a : triggers an alert sound/ audible beep on your system speaker.
#define SEASH_TOK_DELIM " \t\r\n\a" 
char** seaSh_split_line(char* line){
  int bufsize = SEASH_TOK_BUFSIZE;
  int position = 0;
  char** tokens = malloc(bufsize * sizeof(char*));
  char* token;

  if(!tokens){
    fprintf(stderr, "lsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  // strtok: splits a string into smaller pieces called tokens based on specified delimiter characters.
  // it returns a pointer to the next token found, and NULL when there are no tokens left.
  token = strtok(line, SEASH_TOK_DELIM); // CHECK: strtok
  while (token != NULL){
    tokens[position] = token;
    position++;

    if (position >= bufsize){
      bufsize += SEASH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if(!tokens){
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }

    // for strtok to continue tokenising the same string, pass NULL os the first argument.
    token = strtok(NULL, SEASH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

int seaSh_launch(char** args){
  pid_t pid;
  pid_t wpid;
  int status;

  pid = fork(); // fork returns 0 to the child process and the pid of the child process to the parent process
  
  if(pid == 0){
    // child process
    // execvp - expects a program name and an array of string arguments (also called vector - hence the 'v' in execvp)
    // p - instead of providing the full file path of the program to run, we're going to give its name, and let the OS search for the program in the path.
    // if the exec command returns -1 (or actually, if it returns at all) -> error.
    if(execvp(args[0], args) == -1){
      perror("seaSh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0){
    // error forking
    perror("seaSh");
  } else{
    // parent process
    do {
      // waitpid - suspends the execution of the calling process until a specific child process changes its state (eg: terminates, stops or resumes)
      // the basic wait() function pauses for any child process.
      // pid: specifies which child process to wait for.
      // > 0: waits for the specific child process whose PID matches this value.
      // -1: waits for any child process (behaves exactly like wait())
      // 0: waits for any child process in the same process group as the parent.
      // < -1: waits for any child process in the process group whose id is the absolute value of pid.
      // status: pointer to an integer where the system stores the child's exit status information.
      // WUNTRACED: returns if a child process has been stopped by a signal.
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));

    // Status evaluation macros:
    // WIFEXITED(status) - evaluates to true if the child process terminated normally
    // WIFSIGNALED(status) - evaluates to true if the child process was terminated by an unhandled signal
  }

  return 1; // returning 1 acts as a signal to the calling function that we should prompt for input again.
  
}
