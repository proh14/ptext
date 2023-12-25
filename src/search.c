#include <cursor.h>
#include <ptext.h>
#include <search.h>
#include <stdlib.h>
#include <string.h>
#include <utils.h>

void searchCallBack(char *query, int c) {
  if (c == '\x1b' || c == '\r') {
    return;
  }
  int i;
  for (i = 0; i < conf.numrows; i++) {
    char *match = strstr(conf.rows[i].renchar, query);
    if (match) {
      conf.cy = i;
      conf.cx = match - conf.rows[i].renchar;
      conf.rowoff = conf.numrows;
      break;
    }
  }
}

void search(void) {
  int saved_cx = conf.cx;
  int saved_cy = conf.cy;
  int saved_rowoff = conf.rowoff;
  char *query = getPrompt("Search: %s", &searchCallBack);
  if (query) {
    free(query);
  } else {
    conf.cx = saved_cx;
    conf.cy = saved_cy;
    conf.rowoff = saved_rowoff;
  }
}
