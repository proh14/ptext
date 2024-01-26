#include <options.h>
#include <stdafx.h>

option options[] = {
    {"tabstop", 5},
};

int optionCount = sizeof(options) / sizeof(option);

int setOption_char(char *name, int value) {
  for (int i = 0; i < optionCount; i++) {
    if (strcmp(options[i].name, name) == 0) {
      options[i].value = value;
      return 1;
    }
  }
  return 0;
}

int getOption_char(char *name) {
  for (int i = 0; options[i].name != NULL; i++) {
    if (strcmp(options[i].name, name) == 0) {
      return options[i].value;
    }
  }
  return -1;
}

int setOption(optionIndex idx, int value) {
  if (idx < 0 || idx >= (optionIndex)optionCount) {
    return 0;
  }
  options[idx].value = value;
  return 1;
}

int getOption(optionIndex idx) {
  if (idx < 0 || idx >= (optionIndex)optionCount) {
    return -1;
  }
  return options[idx].value;
}
