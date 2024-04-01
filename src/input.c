#include <buffer.h>
#include <commands.h>
#include <cursor.h>
#include <files.h>
#include <input.h>
#include <options.h>
#include <output.h>
#include <ptext.h>
#include <rows.h>
#include <search.h>
#include <stdafx.h>
#include <utils.h>

void delChar(void) {
  if (curbuf.cy == curbuf.numrows) {
    return;
  }
  if (curbuf.cx == 0 && curbuf.cy == 0) {
    return;
  }
  row *row = &curbuf.rows[curbuf.cy];
  if (curbuf.cx > 0) {
    rowDelChar(row, curbuf.cx - 1);
    curbuf.cx--;
  } else {
    curbuf.cx = (int)curbuf.rows[curbuf.cy - 1].len;
    rowAppendString(&curbuf.rows[curbuf.cy - 1], row->chars, row->len);
    delRow(curbuf.cy);
    curbuf.cy--;
  }
}

void insertAChar(int c) {
  if (curbuf.cy == curbuf.numrows) {
    rowAppend("", 0, 0);
  }
  rowInsertChar(&curbuf.rows[curbuf.cy], curbuf.cx, c);
  curbuf.cx++;
  curbuf.dirty++;
}

void insertNewLine(void) {
  if (curbuf.cx == 0) {
    rowAppend("", 0, curbuf.cy);
  } else {
    row *row = &curbuf.rows[curbuf.cy];
    rowAppend(&row->chars[curbuf.cx], row->len - curbuf.cx, curbuf.cy + 1);
    row = &curbuf.rows[curbuf.cy];
    row->len = curbuf.cx;
    row->chars[row->len] = '\0';
    updateRow(row);
  }
  curbuf.cy++;
  curbuf.cx = 0;
}

#ifdef _WIN32
KEY_EVENT_RECORD readKey(void) {
  HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
  INPUT_RECORD irInBuf;
  DWORD cNumRead;

  while (1) {
    WaitForSingleObject(hStdin, INFINITE);
    if (!ReadConsoleInputA(hStdin, &irInBuf, 1, &cNumRead)) {
      die("read");
    }

    if (irInBuf.EventType == KEY_EVENT && irInBuf.Event.KeyEvent.bKeyDown) {
      return irInBuf.Event.KeyEvent;
    }
  }
}
#else
int readKey(void) {
  int nr;
  int c = '\0';
  while ((nr = (int)read(STDIN_FILENO, &c, 1)) != 1) {
    if (nr == -1 && errno != EAGAIN) {
      die("read");
    }
  }
  if (c == '\x1b') {
    char ecode[3];
    if (read(0, &ecode[0], 1) != 1) {
      return '\x1b';
    }
    if (read(0, &ecode[1], 1) != 1) {
      return '\x1b';
    }
    if (ecode[0] == '[') {
      switch (ecode[1]) {
      case 'A':
        return ARROW_UP;
        break;
      case 'B':
        return ARROW_DOWN;
        break;
      case 'C':
        return ARROW_LEFT;
        break;
      case 'D':
        return ARROW_RIGHT;
        break;
      case '3':
        if (read(0, &ecode[2], 1) != 1) {
          return '\x1b';
        }
        if (ecode[2] == '~') {
          return DEL_KEY;
        }
        break;
      }
    }
  }
  return c;
}
#endif

void procKey(void) {
#ifdef _WIN32
  KEY_EVENT_RECORD key = readKey();
  if (key.uChar.AsciiChar == 0)
    return;

  if (key.dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) {
    /*
    if (handleCustomKeys(key.wVirtualKeyCode)) {
      return;
    }
    */
    switch (key.wVirtualKeyCode) {
    case KEY_Q: {
      if (!curbuf.dirty) {
        system("cls");
        exit(0);
      }

      char *yorn = getPrompt("File has unsaved changes. Save? (y/n) %s", NULL);
      if (yorn == NULL) {
        break;
      }

      if (yorn[0] == 'y') {
        free(yorn);
        system("cls");
        save();
        exit(0);
      }

      free(yorn);
      system("cls");
      exit(0);
    } break;
    case KEY_H:
      delChar();
      break;
    case KEY_S:
      save();
      break;
    case KEY_F:
      search();
      break;
    case KEY_A:
      curbuf.cx = 0;
      break;
    case KEY_E:
      curbuf.cx = (int)curbuf.rows[curbuf.cy].renlen;
      break;
    case KEY_D:
      curbuf.cy = curbuf.numrows - 1;
      break;
    case KEY_U:
      curbuf.cy = 0;
      break;
    case KEY_R:
      replace();
      break;
    }
    return;
  }

  switch (key.wVirtualKeyCode) {
  case ARROW_DOWN:
  case ARROW_UP:
  case ARROW_LEFT:
  case ARROW_RIGHT:
    moveCursor(key.wVirtualKeyCode);
    break;
  case BACKSPACE:
  case DEL_KEY:
    if (key.wVirtualKeyCode == DEL_KEY) {
      moveCursor(ARROW_LEFT);
    }
    delChar();
    break;
  case '\r':
    insertNewLine();
    break;
  default:
    insertAChar(key.uChar.AsciiChar);
    break;
  }
#else

  int c = readKey();
  /*
  if (handleCustomKeys(c)) {
    return;
  }
  */

  switch (c) {
  case CTRL_KEY('q'): {
    if (!curbuf.dirty || !getOption(O_QUITCONF)) {
      write(1, "\x1b[2J", 4);
      write(1, "\x1b[H", 3);
      exit(0);
    }
    char *yorn = getPrompt("File has unsaved changes. Save? (y/n) %s", NULL);
    if (yorn == NULL) {
      break;
    }
    if (yorn[0] == 'y') {
      free(yorn);
      write(1, "\x1b[2J", 4);
      write(1, "\x1b[H", 3);
      save();
      exit(0);
    }
    free(yorn);
    write(1, "\x1b[2J", 4);
    write(1, "\x1b[H", 3);
    exit(0);
  } break;
  case ARROW_DOWN:
  case ARROW_UP:
  case ARROW_LEFT:
  case ARROW_RIGHT:
    moveCursor(c);
    break;
  case BACKSPACE:
  case DEL_KEY:
  case CTRL_KEY('h'):
    if (c == DEL_KEY) {
      moveCursor(ARROW_LEFT);
    }
    delChar();
    break;
  case '\r':
    insertNewLine();
    break;
  case CTRL_KEY('s'):
    save();
    break;
  case CTRL_KEY('l'):
  case '\x1b':
    break;
  case CTRL_KEY('f'):
    search();
    break;
  case CTRL_KEY('a'):
    curbuf.cx = 0;
    break;
  case CTRL_KEY('e'):
    curbuf.cx = (int)curbuf.rows[curbuf.cy].len;
    break;
  case CTRL_KEY('d'):
    curbuf.cy = curbuf.numrows - 1;
    break;
  case CTRL_KEY('u'):
    curbuf.cy = 0;
    break;
  case CTRL_KEY('r'):
    replace();
    break;
  case CTRL_KEY('x'):
    delRow(curbuf.cy);
    break;
  case CTRL_KEY('.'):
    execCommand();
    break;
  case CTRL_KEY('o'):
    if (conf.current_buffer == MAX_BUFFERS - 1) {
      setStatusMessage("You are on the last buffer!");
      break;
    }
    ++conf.current_buffer;
    if (conf.num_buffers < conf.current_buffer) {
      createBuffer(&conf.buffers[conf.current_buffer], 1);
      conf.num_buffers++;
    }
    break;
  case CTRL_KEY('b'):
    if (conf.current_buffer == 0) {
      setStatusMessage("You are already on the first buffer!");
      break;
    }
    --conf.current_buffer;
    break;
  default:
    insertAChar(c);
    break;
  }
#endif
}

/*
int handleCustomKeys(int c) {
  for (int i = 0; i < customKeysLen; i++) {
    if (customKeys[i].key == c) {
      customKeys[i].func();
      return 1;
    }
  }

  return 0;
}
*/
