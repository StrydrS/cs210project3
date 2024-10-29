#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};


char **parseArg(const char *cmd) {
  int len = strlen(cmd);
  int numArgs = 1;
  char *cmdCopy = (char *)malloc(sizeof(char) * len);
  strcpy(cmdCopy, cmd);

  for(int i = 0; i < len; i++) {
    if((cmd[i] == ' ') & (cmd[i+1] != '\0')) {
      numArgs++;
    }
  }

  //allocate mem based on number of arguments
  char **argV = malloc(numArgs * sizeof(char *));
  for(int q = 0; q < numArgs; q++) { argV[q] = (char *)malloc(sizeof(char) * len); }

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
  int len = N;

  for(int i = 0; i < len; i++) { 
      if(!strcmp(cmd, allowed[i])) {
        return 1;
      }
    }

	return 0;
}

int main() {

    // TODO
    // Add variables as needed

    char line[256];

    while (1) {

	fprintf(stderr,"rsh>");

	if (fgets(line,256,stdin)==NULL) continue;

	if (strcmp(line,"\n")==0) continue;

	line[strlen(line)-1]='\0';
  
  char **argV = parseArg(line);

  if(isAllowed(argV[0]) == 0) { 
    printf("NOT ALLOWED!\n");
  } else if(isAllowed(argV[0]) == 1) {
    //POSIX STUFF
    }

	// TODO
	// Add code to spawn processes for the first 9 commands
	// And add code to execute cd, exit, help commands
	// Use the example provided in myspawn.c

    }
    return 0;
}
