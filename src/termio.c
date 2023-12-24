#include <termios.h>

void enableRawMode(void) {
  if (tcgetattr(STDIN_FILENO, &conf.orig_termios) == -1)
    die("tcgetattr");

  struct termios raw = conf.orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    die("tcsetattr");
}

void disableRawMode(void) {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &conf.orig_termios) == -1)
    die("tcsetattr");
}
