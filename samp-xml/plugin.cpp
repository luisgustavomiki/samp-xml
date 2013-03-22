#include "plugin.h"
/* Thanks to - http://forum.sa-mp.com/showthread.php?t=295798 - */

cell AMX_NATIVE_CALL HelloWorld(AMX* amx, cell* params)
{
    logprintf("This was printed from the Test plugin! Yay!");
    return 1;
}

PLUGIN_EXPORT unsigned int PLUGIN_CALL Supports() 
{
    return SUPPORTS_VERSION | SUPPORTS_AMX_NATIVES;
}

PLUGIN_EXPORT bool PLUGIN_CALL Load(void **ppData) 
{
    pAMXFunctions = ppData[PLUGIN_DATA_AMX_EXPORTS];
    logprintf = (logprintf_t) ppData[PLUGIN_DATA_LOGPRINTF];

    logprintf(" * Test plugin was loaded.");
    return true;
}

PLUGIN_EXPORT void PLUGIN_CALL Unload()
{
    logprintf(" * Test plugin was unloaded.");
}

AMX_NATIVE_INFO PluginNatives[] =
{
    {"HelloWorld", HelloWorld},
    {0, 0}
};

PLUGIN_EXPORT int PLUGIN_CALL AmxLoad( AMX *amx ) 
{
    return amx_Register(amx, PluginNatives, -1);
}


PLUGIN_EXPORT int PLUGIN_CALL AmxUnload( AMX *amx ) 
{
    return AMX_ERR_NONE;
}