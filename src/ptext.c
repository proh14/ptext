#include <files.h>
#include <input.h>
#include <output.h>
#include <ptext.h>
#include <stdafx.h>
#include <termio.h>
#include <utils.h>

struct config conf;

void init(void);
void freeall(void);
void freeall(void);
void done(void);
void printVersion(void);

/*
TODO: output is not working properly

INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_  HINSTANCE hPrevInstance,
_In_ char* pCmdLine, _In_ int nCmdShow)
{
    if (!AllocConsole())
    {
        return EXIT_FAILURE;
    }

    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONIN$", "r", stdin);

    main(__argc, __argv);
}
*/

int main(int argc, char *argv[]) {
#ifdef _INC_CRTDBG
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
  _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
#endif // _INC_CRTDBG

  if (argc >= 2 && strcmp(argv[1], "-v") == 0) {
    printVersion();
    return 0;
  }

  enableRawMode();
  init();
  if (argc >= 2) {
    openFile(argv[1]);
  }
  while (1) {
    refresh();
    procKey();
  }

  return EXIT_SUCCESS;
}

void printVersion(void) {
  // clang-format off
  printf("@@@@@@@@@@@@@@@\n");
  printf("@@@   @@@   @@@    @@@@@@@@@@@@@@@\n");
  printf("@@@   @@@   @@@    @@@@@@@@@@@@@@@\n");
  printf("      @@@                      @@@\n");
  printf("      @@@                      @@@\n");
  printf("      @@@                      @@@\n");
  printf("   @@@@@@@@@                   @@@\n");
  printf("   @@@@@@@@@                   @@@\n");
  printf("                               @@@               Name:           %s\n",NAME);
  printf("                               @@@               Version:        %s\n",VERSION);
  printf("@@@                            @@@               Maintainer:     %s\n",MAINTAINER);
  printf("@@@                            @@@               Licensed Under: %s\n",LICENSE);
  printf("@@@                            @@@\n");
  printf("@@@                            @@@\n");
  printf("@@@                            @@@\n");
  printf("@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n");
  // clang-format on
}

void die(const char *s) {
  perror(s);
  exit(1);
}

void init(void) {
  conf.numrows = 0;
  conf.rows = NULL;
  conf.cx = 0;
  conf.cy = 0;
  conf.dirty = 0;
  conf.rowoff = 0;
#ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);

  ShowScrollBar(GetConsoleWindow(), SB_VERT, 0);

  CONSOLE_SCREEN_BUFFER_INFO csbi;
  GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
  COORD newSize = {csbi.srWindow.Right + 1, csbi.srWindow.Bottom + 1};

  conf.width = newSize.X;
  conf.height = newSize.Y;

  SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), newSize);
#else
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  conf.width = w.ws_col;
  conf.height = w.ws_row;
#endif // _WIN32

  conf.filename = NULL;
  setStatusMessage("HELP: Ctrl-S = save | Ctrl-Q = quit | Ctrl-F = find");
  atexit(done);
}

void freeall(void) {
  if (conf.rows == NULL && conf.numrows == 0) {
    return;
  }
  for (int i = 0; i < conf.numrows; i++) {
    free(conf.rows[i].chars);
    free(conf.rows[i].renchar);
    free(conf.rows[i].hl);
  }
  free(conf.rows);
  conf.rows = NULL;

  free(conf.filename);
}

void done(void) {
#ifdef _INC_CRTDBG
  _CrtDumpMemoryLeaks();
#endif // _INC_CRTDBG

#ifdef _WIN32
  FreeConsole();
#endif // _WIN32
  freeall();
  disableRawMode();
}
