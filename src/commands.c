#define _BSD_SOURCE
#define _DEFAULT_SOURCE
#define _GNU_SOURCE

#include <commands.h>
#include <ctype.h>
#include <files.h>
#include <stdafx.h>
#include <stdio.h>
#include <userfuncs.h>
#include <utils.h>

commandBind commands[] = {{"set-status", &user_setStatus},
                          {"set", &user_set},
                          {"goto-buffer", &user_gotoBuffer},
                          {NULL, NULL}};
char *getFunctionName(char *command, int *len);

void doLine(char *line) {
  int i;
  line = rtrim(line);
  int len;
  char *fname = getFunctionName(line, &len);
  if (fname == NULL) {
    return;
  }
  for (i = 0; commands[i].name != NULL; i++) {
    if (strcmp(commands[i].name, fname) == 0) {
      break;
    }
  }

  if (commands[i].name == NULL) {
    setStatusMessage("Function %s doesn't exist", fname);
    return;
  }

  args = &fname[len + 1];
  (void)(*commands[i].func)();
}

void execCommand(void) {
  char *command = getPrompt(":%s", NULL);
  if (command == NULL) {
    return;
  }
  doLine(command);
  free(command);
}

void doFile(char *filename) {
  if (access(filename, F_OK) == -1) {
    return;
  }
  FILE *fp = fopen(filename, "r");
  char *line = NULL;
  int len = 0;
  size_t size = 0;
  while ((len = (int)getline(&line, &size, fp)) != -1) {
    doLine(line);
  }
  fclose(fp);
  free(line);
}

char *getFunctionName(char *command, int *len) {
  command = &command[0];
  char *fname = token(command, len);
  return fname;
}
