#include <commands.h>
#include <ctype.h>
#include <files.h>
#include <stdafx.h>
#include <userfuncs.h>
#include <utils.h>

commandBind commands[] = {{"set-status", &user_setStatus}, {NULL, NULL}};
char *getFunctionName(char *command);
char *token(char *command);

void doLine(char *line) {
  int i;
  line = rtrim(line);
  char *fname = getFunctionName(line);
  for (i = 0; commands[i].name != NULL; i++) {
    if (strcmp(commands[i].name, fname) == 0) {
      break;
    }
  }

  if (commands[i].name == NULL) {
    setStatusMessage("Function %s doesn't exist", fname);
    return;
  }

  args = &fname[strlen(fname) + 1];
  (void)(*commands[i].func)();
}

void execCommand(void) {
  char *command = getPrompt(":%s", NULL);
  doLine(command);
  free(command);
}

char *getFunctionName(char *command) {
  command = &command[0];
  char *fname = token(command);
  return fname;
}
