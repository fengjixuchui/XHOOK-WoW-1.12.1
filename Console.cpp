#include "Main.h"


#define ConsoleWriteAddr 0x63CD00
#define ConsoleRegisterCommandAddr 0x63F9E0
#define ConsoleUnregisterCommandAddr 0x63FB40
#define ENABLE_CONSOLE_ADDR = 0xC4EC20,
#define ConsoleInvalidfunctionPtrStart = 0x884800, //1
#define ConsoleInvalidfunctionPtrEnd = 0x884C00,  //7FFFFFFF


void (*Console::Write)(const char* fmt, COLOR_T color, ...) = reinterpret_cast<ConsoleWriteA>(ConsoleWriteAddr);
BOOL(*Console::RegisterCommand)(char const* command, CommadHandler handler, CommandCategory category, char const* help) = reinterpret_cast<ConsoleCommandRegisterPtr>(ConsoleRegisterCommandAddr);
void (*Console::UnregisterCommand)(char const* command) = reinterpret_cast<ConsoleCommandUnregisterPtr>(ConsoleUnregisterCommandAddr);
