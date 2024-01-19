#include <ptext.h>
#include <stdafx.h>

void enableRawMode(void) {
#ifdef _WIN32

  if (!GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE),
                      &conf.originalConsoleMode))
    die("GetConsoleMode");

  DWORD newConsoleMode =
      conf.originalConsoleMode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT);
  if (!SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), newConsoleMode))
    die("SetConsoleMode");

#elif defined(__linux__)
  if (tcgetattr(STDIN_FILENO, &conf.orig_termios) == -1)
    die("tcgetattr");
  struct termios raw = conf.orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcsetattr(0, TCSAFLUSH, &raw) == -1)
    die("tcsetattr");
#endif
}

void disableRawMode(void) {
#ifdef _WIN32
  if (!SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), conf.originalConsoleMode))
    die("SetConsoleMode");
#else
  if (tcsetattr(0, TCSAFLUSH, &conf.orig_termios) == -1)
    die("tcsetattr");
#endif
}
