#include <config.h>
#include <input.h>
#include <utils.h>

void sayHello(void) { setStatusMessage("Hello from config"); }

struct keyPair customKeys[] = {{CTRL_KEY('i'), &sayHello}};
int customKeysLen = sizeof(customKeys) / sizeof(customKeys[0]);
