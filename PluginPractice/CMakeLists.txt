cmake_minimum_required(VERSION 3.12)

# Nombre del proyecto y lenguaje.
project(PluginPrueba LANGUAGES CXX)

set(SDK_DIR $ENV{XPLANE_SDK_PATH})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(PluginPrueba SHARED src/main.cpp)


#donde buscar los headers del SDK
target_include_directories(PluginPrueba PRIVATE
	"${SDK_DIR}/CHeaders/XPLM"
	"${SDK_DIR}/CHeaders/Widgets")

# Versiones de la API que queremos usar. Definir una que tu SDK no conozca es inofensivo.
target_compile_definitions(PluginPrueba PRIVATE
XPLM200=1 XPLM210=1 XPLM300=1 XPLM301=1 XPLM400=1)

if(WIN32)
# Plataforma Windows (IBM es el nombre histórico).
target_compile_definitions(PluginPrueba PRIVATE IBM=1 APL=0 LIN=0
WIN32_LEAN_AND_MEAN NOMINMAX) # evita choques con windows.h

# Enlazar contra las librerías de importación del SDK.
target_link_libraries(PluginPrueba PRIVATE
"${SDK_DIR}/Libraries/Win/XPLM_64.lib"
"${SDK_DIR}/Libraries/Win/XPWidgets_64.lib")

# Runtime de C++ estático: el usuario no necesita instalar el Visual C++ Redistributable.
set_property(TARGET PluginPrueba  PROPERTY
MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(XPL_NAME win)
elseif(APPLE)
target_compile_definitions(PluginPrueba  PRIVATE IBM=0 APL=1 LIN=0)

# Ocultar todo salvo lo marcado con PLUGIN_API.
target_compile_options(PluginPrueba PRIVATE -fvisibility=hidden)
# Enlazar contra los frameworks del SDK.
target_link_libraries(PluginPrueba PRIVATE
"${SDK_DIR}/Libraries/Mac/XPLM.framework"
"${SDK_DIR}/Libraries/Mac/XPWidgets.framework")
set(XPL_NAME mac)
else()
target_compile_definitions(PluginPrueba PRIVATE IBM=0 APL=0 LIN=1)
target_compile_options(PluginPrueba PRIVATE -fPIC -fvisibility=hidden)

# En Linux NO se enlaza nada del SDK: X-Plane aporta los símbolos al cargar el plugin.
set(XPL_NAME lin)
endif()
# Forma final del archivo: sin prefijo "lib", nombre win/mac/lin, extensión.xpl,
# y directamente en dist/PluginPrueba/64. El truco $<1:...> evita que Visual Studio

# añada una subcarpeta Release/ o Debug/.
set(OUT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/dist/PluginPrueba/64")
set_target_properties(PluginPrueba PROPERTIES
PREFIX ""
OUTPUT_NAME "${XPL_NAME}"
SUFFIX ".xpl"
RUNTIME_OUTPUT_DIRECTORY "$<1:${OUT_DIR}>" # Windows coloca aquí la .dll
LIBRARY_OUTPUT_DIRECTORY "$<1:${OUT_DIR}>") # Linux y macOS colocan aquí la .so/.dylib