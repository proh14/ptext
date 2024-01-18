#include <config.h>
#include <input.h>
#include <utils.h>

void sayHello(void) { setStatusMessage("Hello from config"); }

// TODO: lua scripting support ?

// To costumize the key bindings, add a new entry to the customKeys array
// like this:

struct keyPair customKeys[] = {
	{
#ifdef _WIN32
		0x49, // VK_KEY_I
#else
		CTRL_KEY('i'),
#endif // _WIN32
		&sayHello}};

// The first argument is the key combination, the second is the function that
// will get executed
//
// For example here when i press CTRL+i the status message will be update to
// "Hello from config"
//
// The CTRL_KEY macro is defined in input.h and will add the CTRL key of
// something. Some more keys are defined in input.h aswell

int customKeysLen = sizeof(customKeys) / sizeof(customKeys[0]);
