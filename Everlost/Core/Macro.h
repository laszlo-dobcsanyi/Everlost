#ifndef CORE_MACRO_H
#define CORE_MACRO_H

#define _WIN32_WINNT 0x0502
#define _SCL_SECURE_NO_WARNINGS

#define LOGGING

#ifdef LOGGING
#include "Core\Logger.h"
#endif

#endif