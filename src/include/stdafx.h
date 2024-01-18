#pragma once

#ifndef H_STDAFX
#define H_STDAFX

#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>

#include <time.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include <string.h>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#include <winuser.h>
#include <crtdbg.h>
#include <crtdefs.h>
#else
#include <sys/ioctl.h>
#include <termio.h>
#include <unistd.h>
#include <termios.h>
#endif

#endif // !H_STDAFX
