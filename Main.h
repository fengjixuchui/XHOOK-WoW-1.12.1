#pragma once


#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "detours.lib")
#pragma warning( disable: 4244 )
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#include <Windows.h>
#include <d3d9.h>
#include "detours.h"
#include <d3dx9.h>
#include <iostream>
#include <fstream>
#include "detours.h"
#include "Console.h"
#include <stdio.h>
#include <vector>
#include "d3d9types.h"
#include <time.h>
#include "stdint.h"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_internal.h"
#include "detours.h"
#include "XorStr.hpp"
#include "Util.h"
#include "Settings.h"
#include "Vector.h"
#include "pattern.h"
#include "Teleporter.h"
#include "stdafx.h"
#include "Detours/src/detours.h"
#include "Renderer.h"
#include "FindPattern.h"
#include "configs.h"


#include <iostream>
#include <vector>
#include <TlHelp32.h>
#include <tchar.h>
#include "memmod.h"


//objmgr
//#include "ObjectMgr.h"
//#include "CGObject_C.h"
//#include "CGPlayer_C.h"
//#include "CGUnit_C.h"

#ifndef UINT32
#define UINT32 unsigned int
#endif

inline void SetTooltip(const char* text) {
	if (ImGui::IsItemHovered())
		ImGui::SetTooltip("%s", text);
}

namespace Main
{
	void main();
	void Objmanager();

	


}

enum class ObjectType : uint8_t {
	NONE = 0,
	ITEM = 1,
	CONTAINER = 2,
	UNIT = 3,
	PLAYER = 4,
	GAMEOBJ = 5,
	DYNOBJ = 6,
	CORPSE = 7,
};

