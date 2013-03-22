#pragma once

#include "AMX\amx\amx.h"
#include "AMX\plugincommon.h"

typedef void (*logprintf_t)(char* format, ...);

logprintf_t logprintf;
extern void *pAMXFunctions;