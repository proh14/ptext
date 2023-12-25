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
  char *query = getPrompt("Search: %s", &searchCallBack);
  free(query);
}
