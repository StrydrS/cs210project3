#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp", "touch", "mkdir", "ls", "pwd", "cat", 
                    "grep", "chmod", "diff", "cd", "exit", "help"};

char **parseArg(const char *cmd) {
  int len = strlen(cmd);
  int numArgs = 1;
  char *cmdCopy = (char *)malloc(sizeof(char) * len);
  strcpy(cmdCopy, cmd);
  
  //get number of arguments
  for(int i = 0; i < len; i++) { 
    if((cmd[i] == ' ') & (cmd[i+1] != '\0')) {
      numArgs++;
    }
  }

  char **argV;

  argV = malloc((numArgs) * sizeof(char *));
  for(int q = 0; q < numArgs; q++) { argV[q] = (char *)malloc(sizeof(char) * len); }
  argV[numArgs] = NULL;
  
  char *token = strtok(cmdCopy, " ");
  int tokenNum = 0;

  while(token != NULL) {
    strcpy(argV[tokenNum], token);
    token = strtok(NULL, " ");
    tokenNum++;
  }

  return argV;
}

int isAllowed(const char *cmd) {
  for(int i = 0; i < N; i++) { 
    if(!strcmp(cmd, allowed[i])) {
      return 1;
    }
  }
  return 0;
}

int main() {

  posix_spawnattr_t pAttr;
  pid_t pid;
  char line[256];
  int status;

  while(1) { 

    fprintf(stderr, "rsh>");

    if(fgets(line, 256, stdin) == NULL) continue;
    if(!strcmp(line, "\n")) continue;

    line[strlen(line) - 1] = '\0';

    char** argV = parseArg(line);

    if(!isAllowed(argV[0])) printf("NOT ALLOWED!\n");
    else if(!strcmp(argV[0], "exit")) return 0;
    else if(!strcmp(argV[0], "help")) {
      for(int i = 0; i < N; i++) {
        printf("%d: %s\n", i + 1, allowed[i]);
      }
    } else if(!strcmp(argV[0], "cd")) {
        if(argV[2] != NULL) { printf("-rsh: cd: too many arguments\n"); } 
        else { chdir(argV[1]); }
    } else if(isAllowed(argV[0])) {
      posix_spawnattr_init(&pAttr);

      if(posix_spawnp(&pid, argV[0], NULL, &pAttr, argV, environ) != 0) {
        perror("spawn failed");
        exit(EXIT_FAILURE);
      }
      
      if(waitpid(pid, &status, 0) == -1) {
        perror("waitpid failed");
        exit(EXIT_FAILURE);
      }

      posix_spawnattr_destroy(&pAttr);
    }
  }
  return EXIT_SUCCESS;

}
