#define _BSD_SOURCE
#define _DEFAULT_SOURCE
#define _GNU_SOURCE

#include <files.h>
#include <ptext.h>
#include <stdafx.h>
#include <utils.h>

void save(void) {
  if (curbuf.filename == NULL) {
    char *fname = getPrompt("Save as: %s", NULL);
#ifdef _WIN32
    if (GetFileAttributesA(fname) != INVALID_FILE_ATTRIBUTES) {
#else
    if (access(fname, F_OK) == 0) {
#endif
      char *yorn = getPrompt("File exists. Overwrite? (y/n) %s", NULL);
      if (yorn[0] != 'y') {
        free(fname);
        free(yorn);
        return;
      }
      free(yorn);
    }
    curbuf.filename = fname;
  }

  int len;
  char *buf = rowsToString(&len);

#ifdef _WIN32
  HANDLE hFile = CreateFileA(curbuf.filename, GENERIC_WRITE, 0, NULL,
                             CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

  if (hFile != INVALID_HANDLE_VALUE) {
    DWORD bytesWritten;
    if (WriteFile(hFile, buf, len, &bytesWritten, NULL) &&
        bytesWritten == len) {
      CloseHandle(hFile);
      free(buf);
      curbuf.dirty = 0;
      setStatusMessage("%d bytes written to disk", len);
      return;
    }
    CloseHandle(hFile);
  }
#else
  int fd = open(curbuf.filename, O_RDWR | O_CREAT, 0644);
  if (fd != -1) {
    if (ftruncate(fd, len) != -1) {
      if (write(fd, buf, len) == len) {
        close(fd);
        free(buf);
        curbuf.dirty = 0;
        setStatusMessage("%d bytes written to disk", len);
        return;
      }
    }
    close(fd);
  }
#endif

  free(buf);
  setStatusMessage("Can't save! I/O error: %s", strerror(errno));
}
#ifdef _WIN32
size_t getline(char **line, size_t *len, FILE *stream) {
  if (!*line) {
    *len = 0;
  }

  size_t capacity = *len + 128;
  char *temp = (char *)xrealloc(*line, capacity);

  if (!temp)
    return -1;

  *line = temp;

  int c;
  size_t i = 0;

  while ((c = fgetc(stream)) != EOF && c != '\n') {
    if (i == capacity - 1) {
      capacity *= 2;
      char *temp = (char *)xrealloc(*line, capacity);

      if (!temp) {
        return -1;
      }

      *line = temp;
    }

    (*line)[i++] = (char)c;
  }

  if (c == EOF && i == 0)
    return -1;

  (*line)[i] = '\0';
  *len = i;

  return i;
}
#endif // _WIN32

void openFile(char *s) {
  FILE *file = fopen(s, "r");
  if (!file && errno != ENOENT) {
    die("fopen");
  } else if (errno == ENOENT) {
#ifdef _WIN32
    curbuf.filename = _strdup(s);
#else
    curbuf.filename = strdup(s);
#endif
    return;
  }

  free(curbuf.filename);
  curbuf.numrows = 0;

  curbuf.filename = xmalloc(strlen(s) + 1);
  snprintf(curbuf.filename, strlen(s) + 1, "%s", s);

  char *line = NULL;
  size_t cap = 0;

  int len;
  while ((len = (int)getline(&line, &cap, file)) != -1) {
    while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
      len--;
    }
    rowAppend(line, len, curbuf.numrows);
  }
  free(line);

  (void)fclose(file);
  curbuf.dirty = 0;
}
