#include <input.h>
#include <output.h>
#include <ptext.h>
#include <stdafx.h>
#include <utils.h>

void setStatusMessage(const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  vsnprintf(conf.statusmsg, sizeof(conf.statusmsg), fmt, ap);
  va_end(ap);
  conf.statusmsg_time = time(NULL);
}

char *rowsToString(int *buflen) {
  int totlen = 0;
  int j;
  for (j = 0; j < conf.numrows; j++)
    totlen += conf.rows[j].len + 1;
  *buflen = totlen;
  char *buf = malloc(totlen);
  char *p = buf;
  for (j = 0; j < conf.numrows; j++) {
    memcpy(p, conf.rows[j].chars, conf.rows[j].len);
    p += conf.rows[j].len;
    *p = '\n';
    p++;
  }
  return buf;
}

char *getPrompt(char *promt, void (*callback)(char *, int)) {
  size_t bufcap = 100;
  size_t bufsize = 0;
  const int ADD_SIZE = 16;
  char *buf = malloc(100);
  buf[0] = '\0';

  while (1) {
    setStatusMessage(promt, buf);
    refresh();

#ifdef _WIN32
    KEY_EVENT_RECORD key = readKey();
    int c = key.wVirtualKeyCode;

    if (c == VK_RETURN)
#else
    int c = readKey();
    if (c == '\r')
#endif
    {
      if (bufsize != 0) {
        setStatusMessage("");
        if (callback)
          callback(buf, c);
        return buf;
      }
    }
#ifdef _WIN32
    else if (c == DEL_KEY ||
             ((key.dwControlKeyState &
               (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) &&
              (key.uChar.AsciiChar == 'H')) ||
             c == BACKSPACE) {
#else
    else if (c == DEL_KEY || c == CTRL_KEY('h') || c == BACKSPACE) {
#endif
      if (bufsize != 0) {
        buf[--bufsize] = '\0';
      }
    } else if (c == ESC_KEY) {
      if (callback)
        callback(buf, c);
      setStatusMessage("");
      free(buf);
      return NULL;
    } else if (!iscntrl(c) && c < 128) {
      if (bufcap == bufsize - 1) {
        bufcap += ADD_SIZE;
        buf = realloc(buf, bufsize);
        if (buf == NULL) {
          die("realloc");
        }
      }

      buf[bufsize + 1] = '\0';
      buf[bufsize] = c;
      bufsize++;
    }
    if (callback)
      callback(buf, c);
  }
}

char *rtrim(char *s) {
  for (size_t i = strlen(s) - 1; i > 0; i--) {
    if (!isspace(s[i])) {
      break;
    }
    s[i] = '\0';
  }
  return s;
}

char *token(char *command) {
  int inq = 0;
  if (*command == '\0') {
    return NULL;
  }
  int i = 0;
  while (isspace(command[i])) {
    i++;
  }
  command = &command[i];
  for (; command[i] != '\0'; i++) {
    if (command[i] == '"' || command[i] == '\'') {
      inq = !inq;
    }
    if (inq) {
      continue;
    }
    if (isspace(command[i])) {
      command[i] = '\0';
      break;
    }
  }
  return command;
}
