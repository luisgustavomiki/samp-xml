#pragma once
#include "plugin.h"

std::vector<AMXInfo*> AMXInfo::thislist;
extern void *pAMXFunctions;



AMXInfo::AMXInfo(AMX* _vm) {
	this->vm = _vm;
	this->handler = new XMLHandler();
}

void AMXInfo::add(AMX* _vm) {
	AMXInfo::thislist.push_back(new AMXInfo(_vm));
	logprintf(" * AMX Machine 0x%x successfully loaded on index %d!", _vm, AMXInfo::thislist.size() - 1);
}

void AMXInfo::remove(AMX* _vm) {
	for (unsigned i=0; i<AMXInfo::thislist.size(); ++i) {
		if(AMXInfo::thislist[i]->vm == _vm) {
			AMXInfo::thislist.erase(AMXInfo::thislist.begin() + i);
			logprintf(" * AMX Machine 0x%x successfully unloaded from index %d!", _vm, i);
		}
	}
}

XMLHandler* AMXInfo::get(AMX* _vm) {
	for(auto i : AMXInfo::thislist) {
		if(i->vm == _vm) {
			return i->handler;
		}
	}
	return NULL;
}

cell AMX_NATIVE_CALL XMLOpen(AMX* amx, cell* params)
{
	cell *addr = NULL; 
	int length;
	amx_GetAddr(amx, params[1], &addr);
	amx_StrLen(addr, &length);

	char* fname = new char[length + 1];
	amx_GetString(fname, addr, 0, length + 1);

	cell ret = AMXInfo::get(amx)->loadFile(std::string(fname));
	if(ret) {
		logprintf("XML:: (%s) XML File loaded [%d]", fname, ret);
	} else {
		logprintf("XML:: (%s) XML Failed to load (load/parse)", fname, ret);
	}
	delete fname;
    return ret;
}

cell AMX_NATIVE_CALL XMLClose(AMX* amx, cell* params)
{
	int index = params[1];
	AMXInfo::get(amx)->unloadFile(index);
	logprintf("XML:: XML File unloaded [%d]", index);
    return 0;
}

cell AMX_NATIVE_CALL XMLCreatePointer(AMX* amx, cell* params)
{
    return AMXInfo::get(amx)->createPointer(params[1]);
}

cell AMX_NATIVE_CALL XMLDestroyPointer(AMX* amx, cell* params)
{
	AMXInfo::get(amx)->destroyPointer(params[1]);
    return 0;
}

cell AMX_NATIVE_CALL XMLPointerGetName(AMX* amx, cell* params)
{
	cell pointer = params[1];
	int len = params[3];
	cell* string;

	amx_GetAddr(amx, params[2], &string);

	std::string input;
	cell ret = AMXInfo::get(amx)->name(pointer, &input);
	amx_SetString(string, input.c_str(), 0, 0, len);
    return ret;
}

cell AMX_NATIVE_CALL XMLPointerGetValue(AMX* amx, cell* params)
{
	cell pointer = params[1];
	int len = params[3];
	cell* string;

	amx_GetAddr(amx, params[2], &string);

	std::string input;
	cell ret = AMXInfo::get(amx)->value(pointer, &input);
	amx_SetString(string, input.c_str(), 0, 0, len);
    return ret;
}

cell AMX_NATIVE_CALL XMLPointerJumpParent(AMX* amx, cell* params)
{
	return AMXInfo::get(amx)->jumpToParentNode(params[1]);
}

cell AMX_NATIVE_CALL XMLPointerJumpRoot(AMX* amx, cell* params)
{
	return AMXInfo::get(amx)->jumpToDocument(params[1]);
}

cell AMX_NATIVE_CALL XMLPointerJumpChild(AMX* amx, cell* params)
{
	cell *addr = NULL; 
	int length;
	amx_GetAddr(amx, params[2], &addr);
	amx_StrLen(addr, &length);

	char* node = new char[length + 1];
	amx_GetString(node, addr, 0, length + 1);

	return AMXInfo::get(amx)->jumpToChildNode(params[1], node);
}

cell AMX_NATIVE_CALL XMLPointerJumpNext(AMX* amx, cell* params)
{
	cell *addr = NULL; 
	int length;
	amx_GetAddr(amx, params[2], &addr);
	amx_StrLen(addr, &length);

	char* node = new char[length + 1];
	amx_GetString(node, addr, 0, length + 1);

	return AMXInfo::get(amx)->jumpToNextNode(params[1], node);
}

cell AMX_NATIVE_CALL XMLPointerJumpAttr(AMX* amx, cell* params)
{
	cell *addr = NULL; 
	int length;
	amx_GetAddr(amx, params[2], &addr);
	amx_StrLen(addr, &length);

	char* node = new char[length + 1];
	amx_GetString(node, addr, 0, length + 1);

	return AMXInfo::get(amx)->jumpToAttr(params[1], node);
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() 
{
    return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData) 
{
    pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
    logprintf = (logprintf_t) ppData[PLUGIN_DATA_LOGPRINTF];

    logprintf(" * SA:MP XML Plugin by Stewie` loaded.");
    return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
    logprintf(" * SA:MP XML Plugin by Stewie` unloaded.");
}

AMX_NATIVE_INFO PluginNatives[] =
{
    {"xml_open", XMLOpen},
	{"xml_close", XMLClose},
	{"xml_pointer", XMLCreatePointer},
	{"xml_killpointer", XMLDestroyPointer},
	{"xml_pointer_getname", XMLPointerGetName},
	{"xml_pointer_getvalue", XMLPointerGetValue},

	{"xml_pointer_root", XMLPointerJumpRoot},
	{"xml_pointer_parentnode", XMLPointerJumpParent},
	{"xml_pointer_childnode", XMLPointerJumpChild},
	{"xml_pointer_nextnode", XMLPointerJumpNext},

	{"xml_pointer_attr", XMLPointerJumpAttr},
    {0, 0}
};

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad( AMX *amx ) 
{
	AMXInfo::add(amx);
    return amx_Register(amx, PluginNatives, -1);
}


PLUGIN_EXPORT int PLUGIN_CALL AmxUnload( AMX *amx ) 
{
	AMXInfo::remove(amx);
    return AMX_ERR_NONE;
}