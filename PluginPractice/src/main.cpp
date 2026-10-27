#include <cstdio>
#include <cstdint>

//Includes de X-Plane 
#include "XPLMDefs.h"				// PLUGIN API y tipos basicos del SDK
#include "XPLMPlugin.h"				// XPLMPluginID, mensajes XPLM_MSG_*, XPLMEnableFeature
#include "XPLMUtilities.h"			// XPLMDebugString (Log.txt) y XPLMSpeakString

static void Log(const char* msg)
{
	char buf[512];
	std::snprintf(buf, sizeof(buf), "[Plugin de Prueba] %s\n", msg);
	XPLMDebugString(buf);		//añadir el mensaje al log
}

// 1) Start: X-Plane acaba de cargar el .xpl
PLUGIN_API int XPluginStart(char* outName, char* outSig, char* outDesc)
{
	// Cada buffer de salida tiene 256 bytes. snprintf nunca se pasa de ese tamaño.
	std::snprintf(outName, 256, "%s", "MiPrimerPlugin");
	std::snprintf(outSig, 256, "%s", "com.tunombre.miprimerplugin");
	std::snprintf(outDesc, 256, "%s", "Hola Mundo XPlane 12 y verga para la IA");

	XPLMEnableFeature("XPLM_USE_NATIVE_PATHS", 1);		//pedir rutas nativas del sistema
	Log("Plugin Habilitado");

	return 1;		//Mantener el Plugin Cargado
}

// 2) Enable: ya están cargados todos los plugins. Empezamos a trabajar.
PLUGIN_API int XPluginEnable(void)
{
	Log("XPluginEnable:Activado");
	XPLMSpeakString("Comenzando mi primer plugin 1 2 3"); // texto en pantalla (y voz, si está activada)
	return 1;
}

// 3) ReceiveMessage: X-Plane (u otro plugin) nos avisa de algo.
PLUGIN_API void XPluginReceiveMessage(XPLMPluginID inFrom, int inMsg, void* inParam)
{
	(void)inFrom;
	const int planeIndex = static_cast<int>(reinterpret_cast<intptr_t>(inParam));
	switch (inMsg) 
	{
		case XPLM_MSG_PLANE_LOADED:			// inParam es el ÍNDICE del avión: 0 = el del usuario, 1+ = aviones de IA.
			
			char buf[128];
			std::snprintf(buf, sizeof(buf), "Avion cargado en el indice %d", planeIndex);
			Log(buf);
			break;

		case XPLM_MSG_AIRPORT_LOADED:
			Log("Aeropuerto Cargado");
			break;

		case XPLM_MSG_PLANE_CRASHED:
			Log("El avion se ha chocado");
			break;

		default:
			break;
	}
}

// 4) Disable: dejamos de trabajar (deshacemos lo de Enable).
PLUGIN_API void XPluginDisable(void)
{
	Log("XPluginDisable : Plugin Desactivado");
}

// 5) Stop: nos vamos (deshacemos lo de Start).
PLUGIN_API void XPluginStop(void)
{
	Log("XPluginStop: descargado");
}