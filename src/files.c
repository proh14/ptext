#define _BSD_SOURCE
#define _DEFAULT_SOURCE
#define _GNU_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <files.h>
#include <ptext.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <utils.h>

void save(void) {
  if (conf.filename == NULL)
    return;
  int len;
  char *buf = rowsToString(&len);
  int fd = open(conf.filename, O_RDWR | O_CREAT, 0644);
  if (fd != -1) {
    if (ftruncate(fd, len) != -1) {
      if (write(fd, buf, len) == len) {
        close(fd);
        free(buf);
        conf.dirty = 0;
        setStatusMessage("%d bytes written to disk", len);
        return;
      }
    }
    close(fd);
  }
  free(buf);
  setStatusMessage("Can't save! I/O error: %s", strerror(errno));
}

void openFile(const char *s) {
  FILE *file = fopen(s, "r");
  if (!file) {
    die("fopen");
  }
  free(conf.filename);
  conf.filenamelen = strlen(s) + 1;
  conf.filename = malloc(conf.filenamelen);
  snprintf(conf.filename, conf.filenamelen, "%s", s);
  char *line = NULL;
  size_t cap = 0;
  int len;
  while ((len = getline(&line, &cap, file)) != -1) {
    while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
      len--;
    }
    rowAppend(line, len, conf.numrows);
  }
  free(line);
  (void)fclose(file);
  conf.dirty = 0;
}
