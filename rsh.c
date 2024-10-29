#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp",   "touch", "mkdir", "ls", "pwd",  "cat",
                    "grep", "chmod", "diff",  "cd", "exit", "help"};

char **parseArg(const char *cmd) {
  int len = strlen(cmd);
  int numArgs = 1;
  char *cmdCopy = (char *)malloc(sizeof(char) * len);
  strcpy(cmdCopy, cmd);

  for (int i = 0; i < len; i++) {
    if ((cmd[i] == ' ') & (cmd[i + 1] != '\0')) {
      numArgs++;
    }
  }

  // allocate mem based on number of arguments
  char **argV = malloc(numArgs * sizeof(char *));
  for (int q = 0; q < numArgs; q++) {
    argV[q] = (char *)malloc(sizeof(char) * len);
  }

  char *token = strtok(cmdCopy, " ");
  int tokenNum = 0;

  while (token != NULL) {
    strcpy(argV[tokenNum], token);
    token = strtok(NULL, " ");
    tokenNum++;
  }

  return argV;
}

int isAllowed(const char *cmd) {
  int len = N;

  for (int i = 0; i < len; i++) {
    if (!strcmp(cmd, allowed[i])) {
      return 1;
    }
  }

  return 0;
}

int main() {

  // TODO
  // Add variables as needed
  posix_spawnattr_t attr;
  pid_t pid;
  int status; 

  char line[256];

  while (1) {

    fprintf(stderr, "rsh>");

    if (fgets(line, 256, stdin) == NULL)
      continue;

    if (strcmp(line, "\n") == 0)
      continue;

    line[strlen(line) - 1] = '\0';

    char **argV = parseArg(line);

    if (isAllowed(argV[0]) == 0) {
      printf("NOT ALLOWED!\n");
    } else if(!strcmp(argV[0], "exit")) {
      printf("exiting\n");
      return 0;
    }else if (isAllowed(argV[0]) == 1) {
      
      posix_spawnattr_init(&attr);

      if(posix_spawnp(&pid, argV[0], NULL, &attr, argV, environ) != 0) {
        perror("spawn failed");
        exit(EXIT_FAILURE);
      }
      
      if(waitpid(pid, &status, 0) == -1) {
        perror("waitpid failed");
        exit(EXIT_FAILURE);
      }

      if(WIFEXITED(status)) {
        printf("Spawned process exited with status %d\n", WEXITSTATUS(status));
      }

      posix_spawnattr_destroy(&attr);
    }

  }
  return EXIT_SUCCESS;
}
