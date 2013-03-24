#pragma once
#include "AMX\amx\amx.h"
#include "AMX\plugincommon.h"

#include "xml.h"

typedef void (*logprintf_t)(char* format, ...);
logprintf_t logprintf;

class AMXInfo {
	AMX* vm;
	XMLHandler* handler;

	AMXInfo(AMX* _vm);

	static std::vector<AMXInfo*> thislist;
public:
	static void add(AMX* _vm);
	static void remove(AMX* _vm);
	static XMLHandler* get(AMX* _vm);
};
