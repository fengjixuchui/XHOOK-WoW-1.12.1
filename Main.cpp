#include "Main.h"
#include <conio.h>


int baseAddress;

bool tpmenubool = false;
float oldx, oldy, oldz;

using hook_fn = void(*)();
uintptr_t d3d9_device_new = 0;
uintptr_t return_address = 0;
HWND gameWindow;
LPRECT gameWindowRect;



LPD3DXFONT pFont;
ID3DXLine* d3dLine;

#define XHOOKCURRGAME "World of Warcraft"
#define XHOOKCURRGAMEX "X-HOOK for " + XHOOKCURRGAME
#define XHOOKCURRVERSION "V0.1"
#define ENABLE_CONSOLE_ADDR 0xC4EC20

bool Settings::UI::MainUI = false;
ColorVar Settings::UI::accentColor = ImColor(43, 115, 178, 74);  //    |-»
ColorVar Settings::UI::mainColor = ImColor(46, 133, 200, 255);
ColorVar Settings::UI::bodyColor = ImColor(28, 33, 32, 228);
ColorVar Settings::UI::fontColor = ImColor(255, 255, 255, 255);

float Settings::Hacks::Movement::X = 0.001f;
float Settings::Hacks::Movement::Y = 0.001f;
float Settings::Hacks::Movement::Z = 0.001f;
float Settings::Hacks::Movement::CX = 0.001f;
float Settings::Hacks::Movement::CY = 0.001f;
float Settings::Hacks::Movement::CZ = 0.001f;
float Settings::Hacks::Movement::jumpHeightspeed = -20.955547333;
float Settings::Hacks::Movement::jumpHeightwaterspeed = 7.0f;
float Settings::Hacks::Movement::WallClimb = 5.6427876353;
float Settings::Hacks::Movement::Movementspeed = 9.0f;
float Settings::Hacks::Movement::Fallingspeed = 60.0f;
bool Settings::Hacks::Movement::Toggleplayerstate = false;
int Settings::Hacks::Movement::Playerstate = 0;
bool Settings::Hacks::Movement::Teleport;
bool Settings::Hacks::Movement::SuperFly;
bool Settings::Hacks::Movement::AntiJump;
bool Settings::Hacks::Movement::AntiMove;
bool Settings::Hacks::Movement::AntiRoot;
bool Settings::Hacks::Movement::Infintejump;
bool Settings::Hacks::Movement::NoFallDamage;
float Settings::Hacks::Movement::single;
//float Settings::Hacks::Movement::CX, CY, CZ = 0.001f; // Current XYZ
//float Settings::Hacks::Movement::X, Y, Z = 0.001f; // Destination XYZ
int Settings::Hacks::Movement::MAPID = 0;


bool Settings::Background::enable = true;
bool Settings::Hacks::Movement::Gravity = false;
bool Settings::Hacks::Movement::MovementRunningspeed = false;
bool Settings::Hacks::Movement::MovementWalkingspeed = false;
bool Settings::Hacks::Movement::MovementSwimmingspeed = false;
bool Settings::Hacks::Movement::MovementCameraspeed = false;
bool Settings::Hacks::Movement::MovementFacing = false;
bool Settings::Hacks::Movement::test = false;
bool Settings::Hacks::Movement::MovementJumpStartingHeight = false;
bool Settings::Hacks::Movement::MovementNoClip = false;
//
bool Settings::Hacks::Movement::TeleportBack = false;
bool WeTeleportedOnce = false;
//
bool Settings::Hacks::Movement::TeleporttoCorpse = false;
bool Settings::Hacks::Movement::SetCurrentXYZ = false;
bool Settings::Hacks::Movement::MovementTurnspeed = false;
bool Settings::Hacks::LuaProtection::toggle = false;
bool Settings::Hacks::Movement::Lootpatch = false;

float  Settings::Hacks::Movement::Turnspeed = 3.7f;
float  Settings::Hacks::Movement::Cameraspeed = 10.0f;
float  Settings::Hacks::Movement::Jumpstartingheight;
float  Settings::Hacks::Movement::Swimmingspeed = 7.5f;
float  Settings::Hacks::Movement::Walkingspeed = 2.5f;
float  Settings::Hacks::Movement::Runningspeed = 7.5f;
float  Settings::Hacks::Movement::Facing = 1.0f;
int Settings::Hacks::Movement::MovementType = 60;
int Settings::Hacks::Movement::NoClip = 0;

bool Settings::Hacks::Misc::SeeAllLevels = false;
bool Settings::Hacks::Misc::UnderstandAll = false;
bool Settings::Hacks::Misc::NoCollisionTrees = false;
bool Settings::Hacks::Misc::NoCollisionBuildings = false;
bool Settings::Hacks::Misc::NoCollisionMailboxes = false;

using namespace std;

DWORD_PTR
Settings::offsets::PlayerBase = 0x0087BCD4,
Settings::offsets::Playerbaseptr1 = 0x88,
Settings::offsets::Playerbaseptr2 = 0x28,
Settings::offsets::Playerbaseptr3 = 0x690,
Settings::offsets::Playerbaseptr4 = 0x24,
Settings::offsets::Playerbasespeedptr = 0x304, // exactly position
Settings::offsets::InfiniteJump = 0x007C625E, // 1.12.1.5875 (Array) - On: { 0x10 } - Off: { 0x30 }		
Settings::offsets::SuperFly = 0x006341BC, // 1.12.1.5875 (Array) - On: { 0x90, 0x90 } - Off: { 0x74, 0x25 }
Settings::offsets::NoFallDamage = 0x007C63DA, // 1.12.1.5875 (Array) - On: { 0x31, 0xC9, 0x90 } - Off: { 0x8B, 0x4F, 0x78 }

Settings::offsets::AntiJump = 0x007C625F, // 1.12.1.5875 (Array) - On: { 0xEB } - Off: { 0x75 }
Settings::offsets::Swimandfish = 0x00609DA4,  //JMP 00609DE3 [0xEB, 0x3D]
Settings::offsets::AntiMove = 0x00615CF5, // 1.12.1.5875 (Array) - On: { 0xFE } - Off: { 0xF8 }
Settings::offsets::AntiRoot = 0x006163DB, // 1.12.1.5875 (Array) - On: { 0xEB, 0xF9 } - Off: { 0x8A, 0x47 }
Settings::offsets::JumpGravity = 0x007C6272, // 1.12.1.5875 (Float)
Settings::offsets::JumpGravityWater = 0x007C6269, // 1.12.1.5875 (Float)
Settings::offsets::WallClimb = 0x0080DFFC, // 1.12.1.5875 (Float)
Settings::offsets::HeartbeatInterval = 0x00615BA7, // 1.12.1.5875 (UInt)  - Speedhack anti-disconn
Settings::offsets::TerrainCollision = 0x6721F3,
Settings::offsets::WMOCollision = 0x6A467B,
Settings::offsets::M2Collision1 = 0x6ABF13,
Settings::offsets::M2Collision2 = 0x6ABC5A,
Settings::offsets::RemoveAFK = 0x482ED3,
Settings::offsets::FallSpeed = 0x87D894,
Settings::offsets::isingameptr = 0xB4B424,
Settings::offsets::targetguidptr = 0xB4E2D8,
Settings::offsets::SeeAllLevelsptr = 0x5EC720,
Settings::offsets::Understandallptr = 0x518062,
Settings::offsets::NoCollisionTreesptr = 0x6ABC5A,
Settings::offsets::NoCollisionBuildingsptr = 0x006A467B,
Settings::offsets::NoCollisionMailboxesptr = 0x006ABF13,
//FUNCTIONS
Settings::offsets::LastHardwareAction = 0x00CF0BC8,
Settings::offsets::AutoLoot = 0x4C1FA0,
Settings::offsets::ClickToMove = 0x00611130,
Settings::offsets::GetText = 0x703BF0,
Settings::offsets::DoString = 0x00704CD0,
Settings::offsets::GetEndscene = 0x5A17B6,
Settings::offsets::IsLooting = 0x006126B0,
Settings::offsets::GetLootSlots = 0x004C2260,
Settings::offsets::OnRightClickObject = 0x005F8660,
Settings::offsets::OnRightClickUnit = 0x60BEA0,
Settings::offsets::SetFacing = 0x007C6F30,
Settings::offsets::SendMovementPacket = 0x00600A30,
Settings::offsets::PerformDefaultAction = 0x00481F60,
Settings::offsets::CGInputControl__GetActive = 0x005143E0,
Settings::offsets::CGInputControl__SetControlBit = 0x00515090,
Settings::offsets::EnumVisibleObjects = 0x00468380,
Settings::offsets::GetPtrForGuid = 0x00468460,
Settings::offsets::ClntObjMgrGetActivePlayer = 0x00468550,
Settings::offsets::ClntObjMgrGetMapId = 0x00468580,
Settings::offsets::NetClientSend = 0x005379A0,
//Descriptor : ptrdiff_t 
Settings::offsets::GotLoot = 0xB4,
Settings::offsets::SummonedByGuid = 0x30,
Settings::offsets::DynamicFlags = 0x23C,
Settings::offsets::ChannelingSpellId = 0x240,
Settings::offsets::CreatedByGuid = 0x38,
Settings::offsets::GameObjectCreatedByGuid = 0x18,
Settings::offsets::MovementFlags = 0x9E8,
Settings::offsets::Health = 0x58,
Settings::offsets::MaxHealth = 0x70,
Settings::offsets::NpcId = 0xE6C,
Settings::offsets::FactionId = 0x8C,
Settings::offsets::Mana = 0x5C,
Settings::offsets::MaxMana = 0x74,
Settings::offsets::Rage = 0x60,
Settings::offsets::TargetGuid = 0x40,
Settings::offsets::CastingSpellId = 0xC84,
Settings::offsets::UnitNamePtr = 0xB28,
Settings::offsets::CorpseOwnedBy = 0x18,
Settings::offsets::ItemId = 0x34C,
Settings::offsets::ItemStackCount = 0x38,
Settings::offsets::ItemDurability = 0xB8,
Settings::offsets::ItemMaxDurability = 0xBC,
Settings::offsets::ContainerId = 0x6E4,
Settings::offsets::ContainerTotalSlots = 0x6C8,
Settings::offsets::GameObjPos = 0x240,
Settings::offsets::Unit__CMovementData = 0x9A8,
Settings::offsets::UnitPos = 0x9B0,
Settings::offsets::UnitOrientation = 0x9BC,
Settings::offsets::CorpsePos = 0x24,
Settings::offsets::FirstBuff = 0xBC,
Settings::offsets::FirstDebuff = 0x13C,
Settings::offsets::NextBuff = 0x4,
Settings::offsets::GameObjNamePtr = 0x20C,
MovementstatefreezeFIRST = 0x007C620D,
MovementstatefreezeSEC = 0x007C620E;
bool g_bWasInitialized = false;

IDirect3DDevice9* pD3device;
D3DPRESENT_PARAMETERS m_PresentParam;
WNDPROC oWndProc = NULL;
WNDPROC gWndProc = NULL;
extern LRESULT   ImGui_ImplDX9_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

////HOOKING
void HookFunction(PVOID* oFunction, PVOID pDetour)
{

	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(oFunction, pDetour);
	DetourTransactionCommit();
}
void UnhookFunction(PVOID* oFunction, PVOID pDetour)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourDetach(oFunction, pDetour);
	DetourTransactionCommit();
}


typedef int(__fastcall* _ConsoleWrite) (const char*, int);

static int ConsoleWrite(const char* text)
{
	_ConsoleWrite consoleWrite = (_ConsoleWrite)0x0063CB50;
	return consoleWrite(text, 0);
}

DWORD FindDevice(DWORD Len)
{
	DWORD dwObjBase = 0;

	dwObjBase = (DWORD)LoadLibrary("d3d9.dll");
	while (dwObjBase++ < dwObjBase + Len)
	{
		if ((*(WORD*)(dwObjBase + 0x00)) == 0x06C7
			&& (*(WORD*)(dwObjBase + 0x06)) == 0x8689
			&& (*(WORD*)(dwObjBase + 0x0C)) == 0x8689
			) {
			dwObjBase += 2; break;
		}
	}
	return(dwObjBase);
}



void WriteProtectedMemory(unsigned int Address, const void* Bytes, unsigned int Size)
{
	unsigned long oldProtection, newProtection;

	VirtualProtect((void*)Address, Size, PAGE_READWRITE, &oldProtection);
	memcpy((void*)Address, Bytes, Size);
	VirtualProtect((void*)Address, Size, oldProtection, &newProtection);

	return;
}


void WriteProtectedMemoryinteger(unsigned int Address, int* value)
{

	unsigned long oldProtection, newProtection;

	VirtualProtect((void*)Address, sizeof(DWORD), PAGE_READWRITE, &oldProtection);
	memcpy((void*)Address, value, sizeof(DWORD));
	VirtualProtect((void*)Address, sizeof(DWORD), oldProtection, &newProtection);

	return;
}

void WriteProtectedMemoryFloat(unsigned int Address, float* value)
{

	unsigned long oldProtection, newProtection;

	VirtualProtect((void*)Address, sizeof(DWORD), PAGE_READWRITE, &oldProtection);
	memcpy((void*)Address, value, sizeof(DWORD));
	VirtualProtect((void*)Address, sizeof(DWORD), oldProtection, &newProtection);

	return;
}

//declares
#define NOP 0x90
//declares
void nop_(PVOID address, int bytes) {
	DWORD d, ds;
	VirtualProtect(address, bytes, PAGE_EXECUTE_READWRITE, &d);
	memset(address, NOP, bytes); VirtualProtect(address, bytes, d, &ds);
}



const char* tabs[] = {
	"Misc",
	"Config"
};


namespace ImGui
{
	static auto vector_getter = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return Combo(label, currIndex, vector_getter, reinterpret_cast<void*>(&values), values.size());
	}

	inline bool
		ListBox(const char* label, int* currIndex, std::vector<std::string>& values, int height_in_items = -1) {
		if (values.empty()) { return false; }
		return ListBox(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size(), height_in_items);
	}

	bool InputText(const char* label, std::string& str, size_t maxInputSize,
		ImGuiInputTextFlags flags, ImGuiTextEditCallback callback, void* user_data)
	{
		if (str.size() > maxInputSize) { // too large for editing
			ImGui::Text(str.c_str());
			return false;
		}

		std::string buffer(str);
		buffer.resize(maxInputSize);
		bool changed = ImGui::InputText(label, &buffer[0], maxInputSize, flags, callback, user_data);
		// using string as char array
		if (changed) {
			auto i = buffer.find_first_of('\0');
			str = buffer.substr(0u, i);
		}
		return changed;
	}

	bool InputTextMultiline(const char* label, std::string& str, size_t maxInputSize, const ImVec2& size,
		ImGuiInputTextFlags flags, ImGuiTextEditCallback callback, void* user_data)
	{
		if (str.size() > maxInputSize) { // too large for editing
			ImGui::Text(str.c_str());
			return false;
		}

		std::string buffer(str);
		buffer.resize(maxInputSize);
		bool changed = ImGui::InputTextMultiline(label, &buffer[0], maxInputSize, size, flags, callback, user_data);
		// using string as char array
		if (changed) {
			auto i = buffer.find_first_of('\0');
			str = buffer.substr(0u, i);
		}
		return changed;
	}
}

LRESULT CALLBACK MsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM  lParam) { return DefWindowProc(hwnd, uMsg, wParam, lParam); }






__declspec(naked) void cfg_hook()
{
	__asm {
		push eax
		mov eax, [esp + 4]
		cmp eax, [return_address] //; endscene return addr
		jne not_endscene
		mov[d3d9_device_new], ebx
		sub[d3d9_device_new], 4
		not_endscene:
		pop eax
			ret
	}
}

hook_fn patch(const uintptr_t address, hook_fn function)
{
	DWORD protection = 0;
	const auto original = hook_fn(*reinterpret_cast<uintptr_t*>(address));
	if (VirtualProtect(LPVOID(address), sizeof(uintptr_t), PAGE_READWRITE, &protection) == FALSE)
		return nullptr;
	*reinterpret_cast<uintptr_t*>(address) = uintptr_t(function);
	VirtualProtect(LPVOID(address), sizeof(uintptr_t), protection, &protection);
	return original;
}

inline void redirect_stdout()
{
	freopen_s(reinterpret_cast<FILE * *>(stdout), "CONOUT$", "w", stdout);
}



uintptr_t get_return_address(const HMODULE d3d_module)
{//
	constexpr auto return_addr_pattern = "8B CF FF D6 8B 4D 08 8B 41 3C FF 40 08 83 4D FC FF";

	MODULEINFO moduleinfo = { nullptr };
	if (GetModuleInformation(GetCurrentProcess(), d3d_module, &moduleinfo, sizeof(moduleinfo)) == FALSE)
		return 0;

	return uintptr_t(FindPattern(PBYTE(moduleinfo.lpBaseOfDll), moduleinfo.SizeOfImage, return_addr_pattern));
}

/* EndScene defs */
typedef long(__stdcall* EndScene_t)(IDirect3DDevice9*);
EndScene_t o_EndScene = nullptr;

/* BeginScene defs */
typedef long(__stdcall* BeginScene_t)(IDirect3DDevice9*);
BeginScene_t o_BeginScene = nullptr;

/* Present defs */
typedef long(__stdcall* Present_t)(IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA*);
Present_t o_Present = nullptr;

/* Reset defs */
typedef long(__stdcall* Reset_t)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
Reset_t o_Reset = nullptr;

/* WndProc defs*/
LONG_PTR o_WndProc = NULL;/*
LONG_PTR o_WndProcc = NULL;*/

static bool bOnce = false;
//
//static int x = 50;
//static int y = 50;
bool Settings::UI::is_renderer_active = false;



DWORD CoordX = 0x00C7B544;
DWORD CoordY = 0x00C7B548;
DWORD CoordZ = 0x00C7B54C;
DWORD WoWVersion = 0x00837C04;
string version;
DWORD fallingspeed = 0x0087D894;
DWORD jumpHeight = 0x007C6272;
DWORD JumpHeightWater = 0x007C6269;
DWORD Wallclimb = 0x0080DFFC;
DWORD proc_id = 16528;
static bool b = false;

//Inf jump
byte InfinitejumpPatch[] = { 0x10 };
byte InfinitejumpOriginal[] = { 0x30 };
// SuperFly
byte SuperFlyPatch[] = { 0x90, 0x90 };
byte SuperFlyOriginal[] = { 0x74, 0x25 };
//AntiJump
byte AntiJumpPatch[] = { 0xEB };
byte AntiJumpOriginal[] = { 0x75 };
//AntiMove
byte AntiMovePatch[] = { 0xFE };
byte AntiMoveOriginal[] = { 0xF8 };
//AntiRoot
byte AntiRootPatch[] = { 0xEB, 0xF9 };
byte AntiRootOriginal[] = { 0x8A, 0x47 };
//NoFallDamage
byte NoFallDamagePatch[] = { 0x31, 0xC9, 0x90 };
byte NoFallDamageOriginal[] = { 0x8B, 0x4F, 0x78 }; 
//Lua_Remover
byte LuaRemover[] = { 0xB8, 0x01, 0x00, 0x00, 0x00, 0xc3 };
//Loot patch
byte LootPatch[] = { 0xEB };
//see all levels

byte SeeAllLevelsOriginal[] = { 0xC2, 0x08, 0x00, 0x55, 0x8b, 0xec };
byte SeeAllLevelsPatch[] = { 0xC2, 0x08, 0x00, 0x55, 0x8b, 0xec };

byte UnderstandAllOriginal[] = { 0xEB , 0x7F, 0x00, 0x55 };
byte UnderstandAllPatch[] = { 0xEB , 0x7F, 0x00, 0x55 };

byte NoCollisionTreesOriginal[] = { 0x0F, 0x85, 0x1B, 0x01, 0x00, 0x00 };
byte NoCollisionTreesPatch[] = { 0x0F, 0x85, 0x1B, 0x01, 0x00, 0x00 };

byte NoCollisionBuildingsOriginal[] = { 0x90, 0x90 };
byte NoCollisionBuildingsPatch[] = { 0x90, 0x90 };

byte NoCollisionMailboxesOriginal[] = { 0xEB ,0x69 };
byte NoCollisionMailboxesPatch[] = { 0xEB ,0x69 };

DWORD base = 0xB41414;
DWORD pointer1 = 0xAC;
DWORD pointerlocalguid = 0xC0;
DWORD pointer2 = 0x14;
DWORD pointer3 = 0x30;
DWORD pointer4 = 0x3C;
DWORD xpointer = 0x9B8;
DWORD ypointer = 0x9BC;
DWORD zpointer = 0x9C0;
int isingame, currentobjtype, firstobjint;
unsigned long long baseaddr, firstobj, nextobj, localguid, localguids, localguidss, targetguid, currentguid;

DWORD Settings::offsets::playerAddress = 0x00;
DWORD Settings::offsets::playerAddressDUMMY = 0x00;
DWORD Settings::offsets::GetBaseAddress()
{
	return (DWORD)GetModuleHandle(NULL);
}

float x, y, z;

static bool StopObjManager = false;
bool temp = false;


float CorpseX, CorpseY, CorpseZ;
static bool Anotheronce = false;
static bool once = false;
int oldmapid;
ObjectType obj_type; 


	int Settings::Objectmanager::coordinates::isingame()
	{
		return *(int*)((uintptr_t)Settings::offsets::isingameptr);
	}

	int Settings::Objectmanager::coordinates::GetMovementflags()
	{
		return *(int*)((uintptr_t)Settings::offsets::playerAddress + Settings::offsets::MovementFlags);
	}

	float Settings::Objectmanager::coordinates::GetX()
	{
		return *(float*)((uintptr_t)Settings::offsets::playerAddress + 0x9B8);
	}

	float Settings::Objectmanager::coordinates::GetY()
	{
		return *(float*)((uintptr_t)Settings::offsets::playerAddress + 0x9BC);
	}
	float Settings::Objectmanager::coordinates::GetZ()
	{
		return *(float*)((uintptr_t)Settings::offsets::playerAddress + 0x9C0);
	}
	int Settings::Objectmanager::coordinates::GetMapid()
	{
		return *(int*)((uintptr_t)Settings::offsets::GetBaseAddress() + 0x84C498);
	}

	std::string GetMapidName()
	{
		//We could retrieve the zonename and instance with LUA however i havent looked into it...
		int CurrentMapID = Settings::Objectmanager::coordinates::GetMapid();

		switch (CurrentMapID) {
		case 0: return ("Eastern kingdoms");
			break;      
		case 1: return ("Kalimdor");
			break;
		case 13: return ("Test");
			break;
		case 17: return ("Kalidar");
			break;
		case 25: return ("ScottTest");
			break;
		case 29: return ("CashTest");
			break;
		case 30: return ("PVPZone01");
			break;
		case 33: return ("ShadowFang");
			break;
		case 34: return ("StormwindJail");
			break;
		case 35: return ("StormwindPrison");
			break;
		case 36: return ("StormwindInstance");
			break;
		case 37: return ("PVPZone02");
			break;
		case 42: return ("Collin");
			break;
		case 43: return ("WailingCaverns");
			break;
		case 44: return ("Monastery");
			break;
		case 47: return ("RazorfenKraulInstance");
			break;
		case 48: return ("Blackfathom");
			break;
		case 70: return ("Uldaman");
			break;
		case 90: return ("GnomeragonInstance");
			break;
		case 109: return ("SunkenTemple");
			break;
		case 129: return ("RazorfenDowns");
			break;
		case 169: return ("EmeraldDream");
			break;
		case 189: return ("MonasteryInstances");
			break;
		default: return ("Unknow");
			break;
		}
	}

bool ReloadMapchangeonobjmanagerchange = false;
void ReloadTeleportListonmapchange(bool decision)
{
	if (!decision)
		return;

		const char* GetGhConfigDirectory = "C://Xhook//wow//Configs//TeleportHelper//";

		std::ostringstream path;

		path << GetGhConfigDirectory << Settings::Hacks::Movement::MAPID << "//"; // KALIMDOR/EASTERKINGDOM BUT HOW ABOUT DUNGEONS??

		const char* GetPathInconst = path.str().c_str();

		CreateDirectoryA(GetPathInconst, nullptr);

		path << XorStr("teleconfig.json");

		Settings::LoadTeleporterInfo(path.str());

		//printf("Teleport list reloaded\n");

	ReloadMapchangeonobjmanagerchange = false;
	ReloadTeleportListonmapchange(false);
}


void Main::Objmanager() {

	if (!once)
	{
		isingame = Settings::Objectmanager::coordinates::isingame();
		printf("Not ingame. login into your toon to continue.\n");
		once = true;
	}
	while (isingame != 1)
	{
		isingame = Settings::Objectmanager::coordinates::isingame();
		//std::this_thread::sleep_for(std::chrono::seconds(1));
		
		if (isingame == 1)
		{

			if (StopObjManager)
			{
				printf("Exiting objmanager.\n");
				return;
			}

			oldmapid = Settings::Objectmanager::coordinates::GetMapid();

			DWORD test = MemMod::getValue(NULL + base);
			firstobj = MemMod::getValueInt(test + 0xAC);

			//printf("firstobj: %llu\n", firstobj);
			localguid = MemMod::getValueInt(test + 0xC0);
			//printf("LOCALGUID: %llu\n", localguid);				//FINE
			targetguid = MemMod::getValueInt(Settings::offsets::targetguidptr);
			//printf("TARGETGUID: %llu\n", targetguid);			//FINE

			if (targetguid == localguid)
			{
				printf("Target set: Localplayer!\n");
			}

			nextobj = firstobj;
			while (firstobj != 0 && (firstobj & 1) == 0)
			{
				currentobjtype = MemMod::getValueInt(firstobj + 0x14);
				//printf("currentobjtypenumber: %u\n", currentobjtype);
				currentguid = MemMod::getValueInt(firstobj + 0x30);

					if (currentguid == localguid)
					{
						Settings::offsets::playerAddress = firstobj;
						//printf("Found local player!")
						printf("We found our Playeraddress at: 0x%0X\n", Settings::offsets::playerAddress);
						Console::Write("We found our playeraddres.", ECHO_COLOR);
						StopObjManager = true;
						Anotheronce = true;				
						ReloadMapchangeonobjmanagerchange = true;
						Main::main();
						break;
					}
							
				nextobj = MemMod::getValueInt(firstobj + 0x3C);

				if (nextobj == firstobj)
				{
					printf("BREAK!\n");
					break;
				}
				else
				{
					firstobj = nextobj;
				}
			}

		}
	}
}

bool Teleportb = false;
Vector Teleportlist;

bool test1, test2, test3;


static bool spamonce = false;
static bool Luaremoverbool = false;


float OriginalRunningSpeed;
float OriginalWalkingSpeed;
float OriginalSwimmingSpeed;
float OriginalWallclimbValue;
float OriginalJumpHeight;
float OriginalJumpHeightWater;
float OriginalFallingspeed;

void Teleport(float x, float y, float z)
{
	if (!Teleportb)
		return;

		//Incase we need to go back
		oldx = MemMod::getValueFloat(Settings::offsets::playerAddress + 0x9B8);
		oldy = MemMod::getValueFloat(Settings::offsets::playerAddress + 0x9BC);
		oldz = MemMod::getValueFloat(Settings::offsets::playerAddress + 0x9C0);

		WriteProtectedMemoryFloat(Settings::offsets::playerAddress + 0x9B8, &x);
		WriteProtectedMemoryFloat(Settings::offsets::playerAddress + 0x9BC, &y);
		WriteProtectedMemoryFloat(Settings::offsets::playerAddress + 0x9C0, &z);
		printf("Teleporting to X: %f, Y: %f, Z: %f\n", x, y, z);
		
		WeTeleportedOnce = true;
		Teleportb = false;
}
bool RestoreOriginalB = false;
bool WriteAfterInjection = false;
bool runonce = false;
void RestoreOriginal()
{
	if (!RestoreOriginalB)
		return;

	if (!runonce)
	{
		OriginalRunningSpeed = MemMod::getValueFloat(Settings::offsets::playerAddress + 0xA34);
		OriginalWalkingSpeed = MemMod::getValueFloat(Settings::offsets::playerAddress + 0xA38);
		OriginalSwimmingSpeed = MemMod::getValueFloat(Settings::offsets::playerAddress + 0xA3C);
		OriginalWallclimbValue = MemMod::getValueFloat(Settings::offsets::WallClimb);
		OriginalFallingspeed = MemMod::getValueFloat(fallingspeed);
		OriginalJumpHeight = MemMod::getValueFloat(Settings::offsets::JumpGravity);
		OriginalJumpHeightWater = MemMod::getValueFloat(JumpHeightWater);

		runonce = true;
	}

	Settings::Hacks::Movement::Movementspeed = OriginalRunningSpeed;
	Settings::Hacks::Movement::MovementWalkingspeed = OriginalWalkingSpeed;
	Settings::Hacks::Movement::MovementSwimmingspeed = OriginalSwimmingSpeed;
	Settings::Hacks::Movement::WallClimb = OriginalWallclimbValue;
	Settings::Hacks::Movement::Fallingspeed = OriginalFallingspeed;
	Settings::Hacks::Movement::jumpHeightspeed = OriginalJumpHeight;
	Settings::Hacks::Movement::jumpHeightwaterspeed = OriginalJumpHeightWater;

	Settings::Hacks::Movement::MovementRunningspeed = false;
	Settings::Hacks::Movement::MovementWalkingspeed = false;
	Settings::Hacks::Movement::MovementSwimmingspeed = false;
	Settings::Hacks::Movement::Gravity = false;
	Settings::Hacks::Movement::MovementCameraspeed = false;
	Settings::Hacks::Movement::MovementFacing = false;
	Settings::Hacks::Movement::MovementJumpStartingHeight = false;
	Settings::Hacks::Movement::MovementNoClip = false;
	Settings::Hacks::Movement::TeleportBack = false;


	if (!WriteAfterInjection)
	{
		RestoreOriginalB = false;
		return;
	}

		

	WriteProtectedMemoryFloat(Settings::offsets::playerAddress + 0xA34, &OriginalRunningSpeed);
	WriteProtectedMemoryFloat(Settings::offsets::playerAddress + 0xA38, &OriginalWalkingSpeed);
	WriteProtectedMemoryFloat(Settings::offsets::playerAddress + 0xA3C, &OriginalSwimmingSpeed);
	WriteProtectedMemoryFloat(Settings::offsets::WallClimb, &OriginalWallclimbValue);
	WriteProtectedMemoryFloat(fallingspeed, &OriginalFallingspeed);
	WriteProtectedMemoryFloat(Settings::offsets::JumpGravity, &OriginalJumpHeight);
	WriteProtectedMemoryFloat(JumpHeightWater, &OriginalJumpHeightWater);
	Settings::Hacks::Movement::MovementNoClip = false;
	RestoreOriginalB = false;

	return;
}


void Main::main()
{
	if (!spamonce)
	{
		printf("Starting our Objectmanager.\n");
		spamonce = true;
	}

	Main::Objmanager(); // Execute our objmanager once!

	while (true)
	{

		if (Settings::offsets::playerAddress == Settings::offsets::playerAddressDUMMY && !Anotheronce)
		{
			printf("PlayerAddress is invalid\n");
			Console::Write("PlayerAddress is invalid.", ERROR_COLOR);
			StopObjManager = false;
			isingame = -1;
			oldmapid = NULL;
			Main::Objmanager();
			return;
		}

		if (!StopObjManager)
			return;

		int currentisingame = MemMod::getValueInt(Settings::offsets::isingameptr);
		if (isingame != currentisingame)
		{
			printf("You're currently not ingame.\n");
			StopObjManager = false;
			isingame = -1;
			oldmapid = NULL;
			return;
		}

		if (Settings::Hacks::Movement::MAPID == -1)
		{
			printf("Map id is invalid.\n");
			StopObjManager = false;
			isingame = -1;
			oldmapid = NULL;
			return;
		}
		Settings::Hacks::Movement::MAPID = MemMod::getValueInt(Settings::offsets::GetBaseAddress() + 0x84C498);
		if (oldmapid != Settings::Hacks::Movement::MAPID)
		{
			printf("Invalid map id. && player_address.\n");
			StopObjManager = false;
			isingame = -1;
			oldmapid = NULL;
			return;
		}
		x = Settings::Objectmanager::coordinates::GetX();
		y = Settings::Objectmanager::coordinates::GetY();
		z = Settings::Objectmanager::coordinates::GetZ();
		Settings::Hacks::Movement::CX = Settings::Objectmanager::coordinates::GetX();
		Settings::Hacks::Movement::CY = Settings::Objectmanager::coordinates::GetY();
		Settings::Hacks::Movement::CZ = Settings::Objectmanager::coordinates::GetZ();

		if (!Luaremoverbool)
		{
			Luaremoverbool = true;

			Settings::LoadSettings();

			Settings::Hacks::Movement::SetCurrentXYZ = true;

			RestoreOriginalB = true;
			RestoreOriginal();
		}	
				

		
		if (ReloadMapchangeonobjmanagerchange)
		{
			ReloadTeleportListonmapchange(true);
		}

		if (Settings::Hacks::Movement::TeleportBack)
		{	
			if (!WeTeleportedOnce) //If you havent teleported yet it will teleport you to XYZ: 0, 0 ,0
				return;

			WriteProtectedMemoryFloat(Settings::offsets::playerAddress + 0x9B8, &oldx);
			WriteProtectedMemoryFloat(Settings::offsets::playerAddress + 0x9BC, &oldy);
			WriteProtectedMemoryFloat(Settings::offsets::playerAddress + 0x9C0, &oldz);
			printf("Teleporting back to: X: %f, Y: %f, Z: %f\n", oldx, oldy, oldz);

			Settings::Hacks::Movement::TeleportBack = false;
		}

		if (Settings::Hacks::Movement::SetCurrentXYZ)
		{
			Settings::Hacks::Movement::X = Settings::Objectmanager::coordinates::GetX();
			Settings::Hacks::Movement::Y = Settings::Objectmanager::coordinates::GetY();
			Settings::Hacks::Movement::Z = Settings::Objectmanager::coordinates::GetZ();
			//printf("Textboxes are now set on current: X: %f, Y: %f, Z: %f coordinates.\n", Settings::Hacks::Movement::X, Settings::Hacks::Movement::Y, Settings::Hacks::Movement::Z);
			Settings::Hacks::Movement::SetCurrentXYZ = false;
		}

		if (Settings::Hacks::Movement::MovementRunningspeed)
		{
			float speed = MemMod::getValueFloat(Settings::offsets::playerAddress + 0xA34);
			WriteProtectedMemoryFloat(Settings::offsets::playerAddress + 0xA34, &Settings::Hacks::Movement::Runningspeed);
			printf("Running speed is: %f and is now set to %f\n", speed, Settings::Hacks::Movement::Runningspeed);
			Settings::Hacks::Movement::MovementRunningspeed = false;
		}

		if (Settings::Hacks::Movement::MovementWalkingspeed)
		{
			float speed = MemMod::getValueFloat(Settings::offsets::playerAddress + 0xA38);
			WriteProtectedMemoryFloat(Settings::offsets::playerAddress + 0xA38, &Settings::Hacks::Movement::Walkingspeed);
			printf("Walking speed is: %f and is now set to %f\n", speed, Settings::Hacks::Movement::Walkingspeed);
			Settings::Hacks::Movement::Walkingspeed = false;
		}

		if (Settings::Hacks::Movement::MovementSwimmingspeed)
		{
			float speed = MemMod::getValueFloat(Settings::offsets::playerAddress + 0xA3C);
			WriteProtectedMemoryFloat(Settings::offsets::playerAddress + 0xA3C, &Settings::Hacks::Movement::Swimmingspeed);
			printf("Swimming speed: %f and is now set to %f\n", speed, Settings::Hacks::Movement::Swimmingspeed);
			Settings::Hacks::Movement::MovementSwimmingspeed = false;
		}

		if (Settings::Hacks::Movement::MovementFacing)
		{
			//Maybe nice for teleportation?
			float speed = MemMod::getValueFloat(Settings::offsets::playerAddress + 0x9C4);
			WriteProtectedMemoryFloat(Settings::offsets::playerAddress + 0x9C4, &Settings::Hacks::Movement::Facing);
			printf("You were facing: %f and now at: %f\n", speed, Settings::Hacks::Movement::Facing);
			Settings::Hacks::Movement::MovementFacing = false;
		}

		int NoClipoff = 1053557240;
		int NoClipOnn = 0;
		if (Settings::Hacks::Movement::MovementNoClip)
		{
			int CheckValue = MemMod::getValueInt(Settings::offsets::playerAddress + 0xA58);
			if (CheckValue == NoClipOnn)
			{

			}
			else
			{
				WriteProtectedMemoryinteger(Settings::offsets::playerAddress + 0xA58, &NoClipOnn);
			//	printf("NoClip enabled.\n");
			}

		}
		else
		{
			int CheckValue = MemMod::getValueInt(Settings::offsets::playerAddress + 0xA58);
			if (CheckValue == NoClipoff)
			{

			}
			else
			{

				WriteProtectedMemoryinteger(Settings::offsets::playerAddress + 0xA58, &NoClipoff);

			//	printf("NoClip disabled.\n");
			}
		}

		if (Settings::Hacks::Misc::NoCollisionBuildings)
		{

			WriteProtectedMemory(Settings::offsets::NoCollisionBuildingsptr, NoCollisionBuildingsPatch, 2);
			Settings::Hacks::Misc::NoCollisionBuildings = false;
		}
		else
		{
			//WriteProtectedMemory(Settings::offsets::NoCollisionBuildingsptr, NoCollisionBuildingsOriginal, 2);

		}
		if (Settings::Hacks::Misc::NoCollisionTrees)
		{
			WriteProtectedMemory(Settings::offsets::NoCollisionTreesptr, NoCollisionTreesPatch, 6);
			Settings::Hacks::Misc::NoCollisionTrees = false;
		}
		else
		{
			//WriteProtectedMemory(Settings::offsets::NoCollisionTreesptr, NoCollisionTreesOriginal, 6);
		}
		if (Settings::Hacks::Misc::NoCollisionMailboxes)
		{
			WriteProtectedMemory(Settings::offsets::NoCollisionMailboxesptr, NoCollisionMailboxesPatch, 2);
			Settings::Hacks::Misc::NoCollisionMailboxes = false;
		}
		else
		{
			//WriteProtectedMemory(Settings::offsets::NoCollisionMailboxesptr, NoCollisionMailboxesOriginal, 2);
		}

		if (Settings::Hacks::Movement::Infintejump)
		{
			WriteProtectedMemory(Settings::offsets::InfiniteJump, InfinitejumpPatch, 1);
		}
		else
		{
			//WriteProtectedMemory(Settings::offsets::InfiniteJump, InfinitejumpOriginal, 1);
		}

		if (Settings::Hacks::Movement::SuperFly)
		{
			WriteProtectedMemory(Settings::offsets::SuperFly, SuperFlyPatch, 2);
		}
		else
		{
			WriteProtectedMemory(Settings::offsets::SuperFly, SuperFlyOriginal, 2);
		}

		if (Settings::Hacks::Movement::AntiJump)
		{
			WriteProtectedMemory(Settings::offsets::AntiJump, AntiJumpPatch, 1);
		}
		else
		{
			WriteProtectedMemory(Settings::offsets::AntiJump, AntiJumpOriginal, 1);
		}

		if (Settings::Hacks::Movement::AntiMove)
		{
			WriteProtectedMemory(Settings::offsets::AntiMove, AntiMovePatch, 1);
		}
		else
		{
			WriteProtectedMemory(Settings::offsets::AntiMove, AntiMoveOriginal, 1);
		}

		if (Settings::Hacks::Movement::AntiRoot)
		{
			WriteProtectedMemory(Settings::offsets::AntiRoot, AntiRootPatch, 2);
		}
		else
		{
			WriteProtectedMemory(Settings::offsets::AntiRoot, AntiRootOriginal, 2);
		}

		if (Settings::Hacks::Movement::NoFallDamage)
		{
			WriteProtectedMemory(Settings::offsets::NoFallDamage, NoFallDamagePatch, 3);
		}
		else
		{
			WriteProtectedMemory(Settings::offsets::NoFallDamage, NoFallDamageOriginal, 3);
		}

		if (Settings::Hacks::Movement::Lootpatch)
		{
			WriteProtectedMemory(0x004C21C0, LootPatch, 1);
			Settings::Hacks::Movement::Lootpatch = false;
		}

		//if (Settings::Hacks::Misc::SeeAllLevels)
		//{
		//	WriteProtectedMemory(Settings::offsets::SeeAllLevelsptr, SeeAllLevelsPatch, 6);
		//	//Settings::Hacks::Misc::SeeAllLevels = false;

		//}
		//else
		//{
		//	//WriteProtectedMemory(Settings::offsets::SeeAllLevelsptr, SeeAllLevelsOriginal, 6);
		//}
		//if (Settings::Hacks::Misc::UnderstandAll)
		//{
		//	WriteProtectedMemory(Settings::offsets::Understandallptr, UnderstandAllPatch, 2);
		//	//Settings::Hacks::Misc::UnderstandAll = false;
		//}
		//else
		//{
		//	//WriteProtectedMemory(Settings::offsets::Understandallptr, UnderstandAllOriginal, 2);
		//}

		if (Settings::Hacks::Movement::Gravity)
		{

			WriteProtectedMemoryFloat(fallingspeed, &Settings::Hacks::Movement::Fallingspeed); // Freeze value because it resets with movement.
		}
		else
		{

		}

		DWORD base = 0x0087BCD4;

		DWORD Offset1 = 0x88;
		DWORD Offset2 = 0x28;
		DWORD Offset3 = 0x690;
		DWORD Offset4 = 0x24;
		DWORD MaxSpeed = 0x380;
		//DWORD SetPOSX = 0x304;
		//DWORD SetPOSY = 0x308;
		//DWORD SetPOSZ = 0x30C;

		DWORD pointer1Value = MemMod::getValue(Settings::offsets::GetBaseAddress() + base);
		DWORD pointer2Value = MemMod::getValue(pointer1Value + Offset1);
		DWORD pointer3Value = MemMod::getValue(pointer2Value + Offset2);
		//DWORD pointer4Value = MemMod::getValue(pointer3Value + Offset3);
		//DWORD pointer5Value = MemMod::getValue(pointer4Value + Offset4);

		//Movementdata
		//WoW.exe + 0x468608

		DWORD Movementpointer0Value = MemMod::getValue(Settings::offsets::GetBaseAddress() + base);
		DWORD Movementpointer1Value = MemMod::getValue(Movementpointer0Value + Offset1);
		DWORD Movementpointer2Value = MemMod::getValue(Movementpointer1Value + Offset2);
		DWORD Movementpointer3Value = MemMod::getValue(Movementpointer2Value + 0x118);

		if (Settings::Hacks::Movement::MovementJumpStartingHeight)
		{
			WriteProtectedMemoryFloat(Movementpointer3Value + 0x7C, &Settings::Hacks::Movement::Jumpstartingheight);
		}
		else
		{

		}

		if (Settings::Hacks::Movement::TeleporttoCorpse)
		{
			CorpseX = MemMod::getValueFloat(0x00B4E284);
			CorpseY = MemMod::getValueFloat(0x00B4E288);
			CorpseZ = MemMod::getValueFloat(0x00B4E28C);

			if (CorpseX != 0 && CorpseY != 0 && CorpseZ != 0)
			{
				WriteProtectedMemoryFloat(Settings::offsets::playerAddress + 0x9B8, &CorpseX);
				WriteProtectedMemoryFloat(Settings::offsets::playerAddress + 0x9BC, &CorpseY);
				WriteProtectedMemoryFloat(Settings::offsets::playerAddress + 0x9C0, &CorpseZ);
				printf("Corpse found at X: %f Y: %f Z: %f!\n", CorpseX, CorpseY, CorpseZ);

				Settings::Hacks::Movement::TeleporttoCorpse = false;
			}
			else
			{
				printf("Corpse not found! X: %f Y: %f Z: %f! \n", CorpseX, CorpseY, CorpseZ);
				Settings::Hacks::Movement::TeleporttoCorpse = false;
			}
		}

		if (Settings::Hacks::Movement::MovementTurnspeed)
		{
			WriteProtectedMemoryFloat(Movementpointer3Value + 0x9C, &Settings::Hacks::Movement::Turnspeed);
		}

	}
}


char ConfigNames[128] = "";
bool ColorPicker(float* col, bool alphabar)
{
	const int EDGE_SIZE = 200; 
	const ImVec2 SV_PICKER_SIZE = ImVec2(EDGE_SIZE, EDGE_SIZE);
	const float SPACING = ImGui::GetStyle().ItemInnerSpacing.x;
	const float HUE_PICKER_WIDTH = 20.f;
	const float CROSSHAIR_SIZE = 7.0f;

	ImColor color(col[0], col[1], col[2]);
	bool value_changed = false;

	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	ImVec2 picker_pos = ImGui::GetCursorScreenPos();

	float hue, saturation, value;
	ImGui::ColorConvertRGBtoHSV(color.Value.x, color.Value.y, color.Value.z, hue, saturation, value);

	ImColor colors[] = {
		ImColor(255, 0, 0),
		ImColor(255, 255, 0),
		ImColor(0, 255, 0),
		ImColor(0, 255, 255),
		ImColor(0, 0, 255),
		ImColor(255, 0, 255),
		ImColor(255, 0, 0)
	};

	for (int i = 0; i < 6; i++)
	{
		draw_list->AddRectFilledMultiColor(
			ImVec2(picker_pos.x + SV_PICKER_SIZE.x + SPACING, picker_pos.y + i * (SV_PICKER_SIZE.y / 6)),
			ImVec2(picker_pos.x + SV_PICKER_SIZE.x + SPACING + HUE_PICKER_WIDTH,
				picker_pos.y + (i + 1) * (SV_PICKER_SIZE.y / 6)),
			colors[i],
			colors[i],
			colors[i + 1],
			colors[i + 1]
		);
	}

	draw_list->AddLine(
		ImVec2(picker_pos.x + SV_PICKER_SIZE.x + SPACING - 2, picker_pos.y + hue * SV_PICKER_SIZE.y),
		ImVec2(picker_pos.x + SV_PICKER_SIZE.x + SPACING + 2 + HUE_PICKER_WIDTH, picker_pos.y + hue * SV_PICKER_SIZE.y),
		ImColor(255, 255, 255)
	);

	if (alphabar)
	{
		float alpha = col[3];

		draw_list->AddRectFilledMultiColor(
			ImVec2(picker_pos.x + SV_PICKER_SIZE.x + 2 * SPACING + HUE_PICKER_WIDTH, picker_pos.y),
			ImVec2(picker_pos.x + SV_PICKER_SIZE.x + 2 * SPACING + 2 * HUE_PICKER_WIDTH, picker_pos.y + SV_PICKER_SIZE.y),
			ImColor(0, 0, 0), ImColor(0, 0, 0), ImColor(255, 255, 255), ImColor(255, 255, 255)
		);

		draw_list->AddLine(
			ImVec2(picker_pos.x + SV_PICKER_SIZE.x + 2 * (SPACING - 2) + HUE_PICKER_WIDTH, picker_pos.y + alpha * SV_PICKER_SIZE.y),
			ImVec2(picker_pos.x + SV_PICKER_SIZE.x + 2 * (SPACING + 2) + 2 * HUE_PICKER_WIDTH, picker_pos.y + alpha * SV_PICKER_SIZE.y),
			ImColor(255.f - alpha, 255.f, 255.f)
		);
	}

	const ImU32 c_oColorBlack = ImGui::ColorConvertFloat4ToU32(ImVec4(0.f, 0.f, 0.f, 1.f));
	const ImU32 c_oColorBlackTransparent = ImGui::ColorConvertFloat4ToU32(ImVec4(0.f, 0.f, 0.f, 0.f));
	const ImU32 c_oColorWhite = ImGui::ColorConvertFloat4ToU32(ImVec4(1.f, 1.f, 1.f, 1.f));

	ImVec4 cHueValue(1, 1, 1, 1);
	ImGui::ColorConvertHSVtoRGB(hue, 1, 1, cHueValue.x, cHueValue.y, cHueValue.z);
	ImU32 oHueColor = ImGui::ColorConvertFloat4ToU32(cHueValue);

	draw_list->AddRectFilledMultiColor(
		ImVec2(picker_pos.x, picker_pos.y),
		ImVec2(picker_pos.x + SV_PICKER_SIZE.x, picker_pos.y + SV_PICKER_SIZE.y),
		c_oColorWhite,
		oHueColor,
		oHueColor,
		c_oColorWhite
	);

	draw_list->AddRectFilledMultiColor(
		ImVec2(picker_pos.x, picker_pos.y),
		ImVec2(picker_pos.x + SV_PICKER_SIZE.x, picker_pos.y + SV_PICKER_SIZE.y),
		c_oColorBlackTransparent,
		c_oColorBlackTransparent,
		c_oColorBlack,
		c_oColorBlack
	);

	float x = saturation * SV_PICKER_SIZE.x;
	float y = (1 - value) * SV_PICKER_SIZE.y;
	ImVec2 p(picker_pos.x + x, picker_pos.y + y);
	draw_list->AddLine(ImVec2(p.x - CROSSHAIR_SIZE, p.y), ImVec2(p.x - 2, p.y), ImColor(255, 255, 255));
	draw_list->AddLine(ImVec2(p.x + CROSSHAIR_SIZE, p.y), ImVec2(p.x + 2, p.y), ImColor(255, 255, 255));
	draw_list->AddLine(ImVec2(p.x, p.y + CROSSHAIR_SIZE), ImVec2(p.x, p.y + 2), ImColor(255, 255, 255));
	draw_list->AddLine(ImVec2(p.x, p.y - CROSSHAIR_SIZE), ImVec2(p.x, p.y - 2), ImColor(255, 255, 255));

	ImGui::InvisibleButton("saturation_value_selector", SV_PICKER_SIZE);

	if (ImGui::IsItemActive() && ImGui::GetIO().MouseDown[0])
	{
		ImVec2 mouse_pos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - picker_pos.x, ImGui::GetIO().MousePos.y - picker_pos.y);

		if (mouse_pos_in_canvas.x < 0)
			mouse_pos_in_canvas.x = 0;
		else if (mouse_pos_in_canvas.x >= SV_PICKER_SIZE.x - 1)
			mouse_pos_in_canvas.x = SV_PICKER_SIZE.x - 1;

		if (mouse_pos_in_canvas.y < 0)
			mouse_pos_in_canvas.y = 0;
		else if (mouse_pos_in_canvas.y >= SV_PICKER_SIZE.y - 1)
			mouse_pos_in_canvas.y = SV_PICKER_SIZE.y - 1;

		value = 1 - (mouse_pos_in_canvas.y / (SV_PICKER_SIZE.y - 1));
		saturation = mouse_pos_in_canvas.x / (SV_PICKER_SIZE.x - 1);
		value_changed = true;
	}

	// hue bar logic
	ImGui::SetCursorScreenPos(ImVec2(picker_pos.x + SPACING + SV_PICKER_SIZE.x, picker_pos.y));
	ImGui::InvisibleButton("hue_selector", ImVec2(HUE_PICKER_WIDTH, SV_PICKER_SIZE.y));

	if (ImGui::GetIO().MouseDown[0] && (ImGui::IsItemHovered() || ImGui::IsItemActive()))
	{
		ImVec2 mouse_pos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - picker_pos.x, ImGui::GetIO().MousePos.y - picker_pos.y);

		if (mouse_pos_in_canvas.y < 0)
			mouse_pos_in_canvas.y = 0;
		else if (mouse_pos_in_canvas.y >= SV_PICKER_SIZE.y - 1)
			mouse_pos_in_canvas.y = SV_PICKER_SIZE.y - 1;

		hue = mouse_pos_in_canvas.y / (SV_PICKER_SIZE.y - 1);
		value_changed = true;
	}

	if (alphabar)
	{
		ImGui::SetCursorScreenPos(ImVec2(picker_pos.x + SPACING * 2 + HUE_PICKER_WIDTH + SV_PICKER_SIZE.x, picker_pos.y));
		ImGui::InvisibleButton("alpha_selector", ImVec2(HUE_PICKER_WIDTH, SV_PICKER_SIZE.y));

		if (ImGui::GetIO().MouseDown[0] && (ImGui::IsItemHovered() || ImGui::IsItemActive()))
		{
			ImVec2 mouse_pos_in_canvas = ImVec2(ImGui::GetIO().MousePos.x - picker_pos.x, ImGui::GetIO().MousePos.y - picker_pos.y);

			if (mouse_pos_in_canvas.y < 0)
				mouse_pos_in_canvas.y = 0;
			else if (mouse_pos_in_canvas.y >= SV_PICKER_SIZE.y - 1)
				mouse_pos_in_canvas.y = SV_PICKER_SIZE.y - 1;

			float alpha = mouse_pos_in_canvas.y / (SV_PICKER_SIZE.y - 1);
			col[3] = alpha;
			value_changed = true;
		}
	}

	color = ImColor::HSV(hue >= 1 ? hue - 10 * 1e-6 : hue, saturation > 0 ? saturation : 10 * 1e-6, value > 0 ? value : 1e-6);
	col[0] = color.Value.x;
	col[1] = color.Value.y;
	col[2] = color.Value.z;

	bool widget_used;
	ImGui::PushItemWidth((alphabar ? SPACING + HUE_PICKER_WIDTH : 0) + SV_PICKER_SIZE.x + SPACING + HUE_PICKER_WIDTH - 2 * ImGui::GetStyle().FramePadding.x);
	widget_used = alphabar ? ImGui::ColorEdit4("", col) : ImGui::ColorEdit3("", col);
	ImGui::PopItemWidth();

	float new_hue, new_sat, new_val;
	ImGui::ColorConvertRGBtoHSV(col[0], col[1], col[2], new_hue, new_sat, new_val);

	if (new_hue <= 0 && hue > 0)
	{
		if (new_val <= 0 && value != new_val)
		{
			color = ImColor::HSV(hue, saturation, new_val <= 0 ? value * 0.5f : new_val);
			col[0] = color.Value.x;
			col[1] = color.Value.y;
			col[2] = color.Value.z;
		}
		else if (new_sat <= 0)
		{
			color = ImColor::HSV(hue, new_sat <= 0 ? saturation * 0.5f : new_sat, new_val);
			col[0] = color.Value.x;
			col[1] = color.Value.y;
			col[2] = color.Value.z;
		}
	}

	return value_changed | widget_used;
}
bool ColorPicker4(float col[4])
{
	return ColorPicker(col, true);
}

struct ColorListVar
{
	const char* name;
	union
	{
		ColorVar* colorVarPtr;
		HealthColorVar* healthColorVarPtr;
	};
	enum
	{
		COLORVAR_TYPE,
		HEALTHCOLORVAR_TYPE
	} type;

	ColorListVar(const char* name, ColorVar* ptr)
	{
		this->name = name;
		this->colorVarPtr = ptr;
		this->type = COLORVAR_TYPE;
	}

	ColorListVar(const char* name, HealthColorVar* ptr)
	{
		this->name = name;
		this->healthColorVarPtr = ptr;
		this->type = HEALTHCOLORVAR_TYPE;
	}
};


ColorListVar colors[] = {
	{ "UI Main", &Settings::UI::mainColor },
	{ "UI Body", &Settings::UI::bodyColor },
	{ "UI Font", &Settings::UI::fontColor },
	{ "UI accent", &Settings::UI::accentColor },

};
const char* colorNames[IM_ARRAYSIZE(colors)];
bool Once = true;
LPDIRECT3DTEXTURE9 BackgroundTexture = NULL;


void EnableConsole()
{
	*reinterpret_cast<DWORD*>(ENABLE_CONSOLE_ADDR) = 1;
}

//DEFINES
LPD3DXFONT g_pFont = NULL;
LPD3DXFONT g_pFont2 = NULL;
ID3DXLine* g_pLine;
#define GREY	D3DCOLOR_ARGB(255, 128, 128, 128)
#define WHITE	D3DCOLOR_ARGB(255, 255, 255, 255)
#define RED		D3DCOLOR_ARGB(255, 255, 000, 000)
#define GREEN	D3DCOLOR_ARGB(255, 000, 255, 000)
#define YELLOW	D3DCOLOR_ARGB(255, 255, 255, 000)
#define CYAN	D3DCOLOR_ARGB(255, 000, 162, 232)
//LPDIRECT3DTEXTURE9 White, Red, Green, Yellow;
//----------------------------------DrawString------------------------------------------------------------------------------------------------
void DrawString(int x, int y, DWORD color, const char* fmt, ...)
{
	RECT FontPos = { x, y, x + 30, y + 20 };
	char buf[1024] = { '\0' };
	va_list va_alist;

	va_start(va_alist, fmt);
	vsprintf(buf, fmt, va_alist);
	va_end(va_alist);

	g_pFont2->DrawText(NULL, buf, -1, &FontPos, DT_NOCLIP, color);
}
//------------------------------------Show timeinfo----------------------------------------------------------------------------------------------
void Time(int x, int y, D3DCOLOR color)
{
	static float TimeElapsed = 0;
	static char FinalString[MAX_PATH];
	static time_t TimeValue;
	static tm* timeinfo;

	RECT FontPos = { x, y, x + 200, y + 16 };

	time(&TimeValue);
	timeinfo = localtime(&TimeValue);
	if (timeinfo->tm_hour > 12)
		sprintf(FinalString, " [Time : %d:%02d:%02d PM] ", timeinfo->tm_hour - 12, timeinfo->tm_min, timeinfo->tm_sec);
	else
		sprintf(FinalString, " [Time : %d:%02d:%02d AM] ", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
	g_pFont->DrawTextA(NULL, FinalString, -1, &FontPos, DT_NOCLIP, color);
}
//-------------------------------------current time---------------------------------------------------------------------------------------------
void currenttime(int x, int y, D3DCOLOR color)
{
	static char cdate[20] = "";
	struct tm* current_tm;

	RECT FontPos = { x, y, x + 200, y + 16 };

	time_t current_time;
	time(&current_time);
	current_tm = localtime(&current_time);
	sprintf(cdate, " [Date : %d-%d-%02d] ", current_tm->tm_mon + 1, current_tm->tm_mday, current_tm->tm_year - 100 + 2000);
	g_pFont->DrawTextA(NULL, cdate, -1, &FontPos, DT_NOCLIP, color);
}

void convertfloatchar(char* ch, float fl)
{
	sprintf(ch, "%f", fl);
}

void convertintchar(char* ch, int integer)
{
	sprintf(ch, "%i", integer);
}

void convertstringchar(char* ch, string word)
{
	sprintf(ch, "%s", word);
}

int throwMessage = 0;
bool testos = false;
bool DrawOnSCreen = false;
//HEIGHT WIDTH//
int width;
int height;
HRESULT __stdcall EndScene_h(IDirect3DDevice9* device)
{
	
	Settings::UI::is_renderer_active = renderer.IsActive();

	DWORD dwOld_D3DRS_COLORWRITEENABLE;
	device->GetRenderState(D3DRS_COLORWRITEENABLE, &dwOld_D3DRS_COLORWRITEENABLE);
	device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xffffffff);
	if (!testos)
	{
		if (g_pFont == NULL) D3DXCreateFont(device, 15, 0, FW_BOLD, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &g_pFont); //Create fonts
		if (g_pFont2 == NULL) D3DXCreateFont(device, 15, 0, FW_BOLD, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &g_pFont2); //Create fonts
		if (g_pLine == NULL) D3DXCreateLine(device, &g_pLine);
	}

	if (DrawOnSCreen)
	{

		DrawString(0, 1, CYAN, "XHOOK for World of Warcraft");
		currenttime(0, 15, CYAN); // x,y = width and height
		Time(0, 29, CYAN);      // x,y = width and height

		char myxarray[64], myyarray[64], myzarray[64], mymaparray[64], mymapname[64];
		convertfloatchar(myxarray, Settings::Hacks::Movement::CX),
		convertfloatchar(myyarray, Settings::Hacks::Movement::CY),
		convertfloatchar(myzarray, Settings::Hacks::Movement::CZ);
		convertintchar(mymaparray, Settings::Hacks::Movement::MAPID);
	
		char* instancenamearray = new char[GetMapidName().length() + 1];
		strcpy(instancenamearray, GetMapidName().c_str());


		DrawString(250, 15, CYAN, myxarray);
		DrawString(250, 29, CYAN, myyarray);
		DrawString(250, 43, CYAN, myzarray);
		DrawString(250, 57, CYAN, mymaparray);
		DrawString(250, 71, CYAN, instancenamearray);
	}
	if (!g_bWasInitialized)
	{
		printf("EndScene_h hooked  \n");
		ImGuiIO& io = ImGui::GetIO();
		io.DeltaTime = 1.0f / 60.0f;
		D3DDEVICE_CREATION_PARAMETERS d3dcp{ 0 };
		device->GetCreationParameters(&d3dcp);
		io.Fonts->AddFontDefault();
		ImGui_ImplDX9_Init(d3dcp.hFocusWindow, device);
		g_bWasInitialized = true;
	}

	ImGui::GetIO().MouseDrawCursor = Settings::UI::is_renderer_active;
	Settings::UI::MainUI = Settings::UI::is_renderer_active;
	ImGui_ImplDX9_NewFrame();
	static ImGuiStyle* style = &ImGui::GetStyle();
	static int throwMessageCurrent = -1;
	static char inputName[40];

	static bool Executeonce;
	if (!Executeonce)
	{
		ImGuiStyle& style = ImGui::GetStyle();

		ImVec4 mainColorHovered = ImVec4(Settings::UI::mainColor.Color().Value.x + 0.1f, Settings::UI::mainColor.Color().Value.y + 0.1f, Settings::UI::mainColor.Color().Value.z + 0.1f, Settings::UI::mainColor.Color().Value.w);
		ImVec4 mainColorActive = ImVec4(Settings::UI::mainColor.Color().Value.x + 0.2f, Settings::UI::mainColor.Color().Value.y + 0.2f, Settings::UI::mainColor.Color().Value.z + 0.2f, Settings::UI::mainColor.Color().Value.w);
		ImVec4 menubarColor = ImVec4(Settings::UI::bodyColor.Color().Value.x, Settings::UI::bodyColor.Color().Value.y, Settings::UI::bodyColor.Color().Value.z, Settings::UI::bodyColor.Color().Value.w - 0.8f);
		ImVec4 frameBgColor = ImVec4(Settings::UI::bodyColor.Color().Value.x, Settings::UI::bodyColor.Color().Value.y, Settings::UI::bodyColor.Color().Value.z, Settings::UI::bodyColor.Color().Value.w + .1f);
		ImVec4 tooltipBgColor = ImVec4(Settings::UI::bodyColor.Color().Value.x, Settings::UI::bodyColor.Color().Value.y, Settings::UI::bodyColor.Color().Value.z, Settings::UI::bodyColor.Color().Value.w + .05f);

		style.Alpha = 1.0f;
		style.WindowPadding = ImVec2(8, 8);
		style.WindowMinSize = ImVec2(32, 32);
		style.WindowRounding = 3.0f;
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
		style.ChildWindowRounding = 3.0f;
		style.FramePadding = ImVec2(4, 3);
		style.FrameRounding = 2.0f;
		style.ItemSpacing = ImVec2(8, 4);
		style.ItemInnerSpacing = ImVec2(4, 4);
		style.TouchExtraPadding = ImVec2(0, 0);
		style.IndentSpacing = 21.0f;
		style.ColumnsMinSpacing = 3.0f;
		style.ScrollbarSize = 12.0f;
		style.ScrollbarRounding = 0.0f;
		style.GrabMinSize = 5.0f;
		style.GrabRounding = 2.0f;
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style.DisplayWindowPadding = ImVec2(22, 22);
		style.DisplaySafeAreaPadding = ImVec2(4, 4);
		style.AntiAliasedLines = true;
		style.AntiAliasedShapes = true;
		style.CurveTessellationTol = 1.25f;

		style.Colors[ImGuiCol_Text] = Settings::UI::fontColor.Color();
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style.Colors[ImGuiCol_WindowBg] = Settings::UI::bodyColor.Color();
		style.Colors[ImGuiCol_ChildWindowBg] = ImVec4(.0f, .0f, .0f, .0f);
		style.Colors[ImGuiCol_PopupBg] = tooltipBgColor;
		style.Colors[ImGuiCol_Border] = Settings::UI::mainColor.Color();
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
		style.Colors[ImGuiCol_FrameBg] = frameBgColor;
		style.Colors[ImGuiCol_FrameBgHovered] = mainColorHovered;
		style.Colors[ImGuiCol_FrameBgActive] = mainColorActive;
		style.Colors[ImGuiCol_TitleBg] = Settings::UI::mainColor.Color();
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(1.00f, 0.98f, 0.95f, 0.75f);
		style.Colors[ImGuiCol_TitleBgActive] = Settings::UI::mainColor.Color();
		style.Colors[ImGuiCol_MenuBarBg] = menubarColor;
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(frameBgColor.x + .05f, frameBgColor.y + .05f, frameBgColor.z + .05f, frameBgColor.w);
		style.Colors[ImGuiCol_ScrollbarGrab] = Settings::UI::mainColor.Color();
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = mainColorHovered;
		style.Colors[ImGuiCol_ScrollbarGrabActive] = mainColorActive;
		style.Colors[ImGuiCol_Separator] = mainColorActive;
		style.Colors[ImGuiCol_ComboBg] = frameBgColor;
		style.Colors[ImGuiCol_CheckMark] = Settings::UI::mainColor.Color();
		style.Colors[ImGuiCol_SliderGrab] = mainColorHovered;
		style.Colors[ImGuiCol_SliderGrabActive] = mainColorActive;
		style.Colors[ImGuiCol_Button] = Settings::UI::mainColor.Color();
		style.Colors[ImGuiCol_ButtonHovered] = mainColorHovered;
		style.Colors[ImGuiCol_ButtonActive] = mainColorActive;
		style.Colors[ImGuiCol_Header] = Settings::UI::mainColor.Color();
		style.Colors[ImGuiCol_HeaderHovered] = mainColorHovered;
		style.Colors[ImGuiCol_HeaderActive] = mainColorActive;
		style.Colors[ImGuiCol_Column] = Settings::UI::mainColor.Color();
		style.Colors[ImGuiCol_ColumnHovered] = mainColorHovered;
		style.Colors[ImGuiCol_ColumnActive] = mainColorActive;
		style.Colors[ImGuiCol_ResizeGrip] = Settings::UI::mainColor.Color();
		style.Colors[ImGuiCol_ResizeGripHovered] = mainColorHovered;
		style.Colors[ImGuiCol_ResizeGripActive] = mainColorActive;
		style.Colors[ImGuiCol_CloseButton] = Settings::UI::mainColor.Color();
		style.Colors[ImGuiCol_CloseButtonHovered] = mainColorHovered;
		style.Colors[ImGuiCol_CloseButtonActive] = mainColorActive;
		style.Colors[ImGuiCol_PlotLines] = Settings::UI::mainColor.Color();
		style.Colors[ImGuiCol_PlotLinesHovered] = mainColorHovered;
		style.Colors[ImGuiCol_PlotHistogram] = Settings::UI::mainColor.Color();
		style.Colors[ImGuiCol_PlotHistogramHovered] = mainColorHovered;
		style.Colors[ImGuiCol_TextSelectedBg] = mainColorHovered;
		style.Colors[ImGuiCol_ModalWindowDarkening] = ImVec4(0.0f, 0.0f, 0.0f, 0.75f);
	}

	if (Settings::UI::is_renderer_active)
	{


		static int page = 0;
		ImGui::SetNextWindowSize(ImVec2(1080, 720), ImGuiSetCond_FirstUseEver);
		static bool shit;

		ImGui::Begin("XHOOK for World of Warcraft 1.12.1 5875", &shit, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders);
		{
			for (int i = 0; i < IM_ARRAYSIZE(tabs); i++)
			{
				int distance = i == page ? 0 : i > page ? i - page : page - i;

				if (ImGui::Button(tabs[i], ImVec2(ImGui::GetWindowSize().x / IM_ARRAYSIZE(tabs) - 9, 0)))
					page = i;

				if (i < IM_ARRAYSIZE(tabs) - 1)
					ImGui::SameLine();
			}

			ImGui::Separator();

			if (!Settings::Background::enable)
			{
				//Background Image
				ImVec2 pos = ImGui::GetCursorScreenPos();
				ImVec2 maxPos = ImVec2(pos.x + ImGui::GetWindowSize().x, pos.y + ImGui::GetWindowSize().y);


				ImGui::GetWindowDrawList()->AddImage(
					(void*)BackgroundTexture,
					ImVec2(pos.x, pos.y),
					ImVec2(maxPos),
					ImVec2(1, 0), ImVec2(0, 1)
				);

			}

			Configs::RenderWindow();

			int currentwidth = ImGui::GetWindowSize().x;
			int nextwidth = (currentwidth - (currentwidth * 2));

			switch (page)
			{
			case 0:

				ImGui::Columns(1, NULL, true);
				{
					ImGui::BeginChild("COL1", ImVec2(0, 0), true);
					{
						ImGui::Text("MISC");
						ImGui::BeginChild("MISC", ImVec2(0, 0), true);
						ImGui::Text("Cheats");
						ImGui::Separator();
						ImGui::Columns(2, NULL, true);
						{
							ImGui::PushItemWidth(100);
							ImGui::Text("X:");
							ImGui::SameLine();
							ImGui::InputFloat(XorStr("##X"), &Settings::Hacks::Movement::X, 10.01f, 0.0f, 2.0f);
							ImGui::SameLine();
							ImGui::Text("Y:");
							ImGui::SameLine();
							ImGui::InputFloat(XorStr("##Y"), &Settings::Hacks::Movement::Y, 10.01f, 3.0f, 2.0f); 
							ImGui::SameLine();
							ImGui::Text("Z:");
							ImGui::SameLine();
							ImGui::InputFloat(XorStr("##Z"), &Settings::Hacks::Movement::Z, 10.01f, 1.0f, 2.0f);
							ImGui::SameLine();
						
							if (ImGui::Button("Get xyz"))
							{
								Settings::Hacks::Movement::SetCurrentXYZ = true;
							}
							ImGui::SameLine();
							//ImGui::PushItemWidth(1600);
							if (ImGui::Button("Teleport!"))
							{
								Teleportb = true;
								Teleport(Settings::Hacks::Movement::X, Settings::Hacks::Movement::Y, Settings::Hacks::Movement::Z);
							}
							ImGui::PopItemWidth();
							ImGui::PushItemWidth(478);

							ImGui::SliderFloat("##RUNNING_SPEED", &Settings::Hacks::Movement::Runningspeed, 0, 350, "Running speed %0.3f");
							ImGui::SameLine();
							if (ImGui::Button("SET!###ID1"))
							{
								Settings::Hacks::Movement::MovementRunningspeed = true;
							}
							ImGui::SliderFloat("##WALKING_SPEED", &Settings::Hacks::Movement::Walkingspeed, 0, 350, "Walking speed %0.3f");
							ImGui::SameLine();
							if (ImGui::Button("SET!###ID154"))
							{
								Settings::Hacks::Movement::MovementWalkingspeed = true;
							}

							ImGui::SliderFloat("##SWIMMING_SPEED", &Settings::Hacks::Movement::Swimmingspeed, 0, 350, "Swimming speed %0.3f");
							ImGui::SameLine();
							if (ImGui::Button("SET!###ID12"))
							{
								Settings::Hacks::Movement::MovementSwimmingspeed = true;
							}


							ImGui::SliderFloat("##FALLING_SPEED", &Settings::Hacks::Movement::Fallingspeed, 0, 350, "Falling speed %0.3f");
							ImGui::SameLine();
							if (ImGui::Button("SET!###ID2"))
							{
								WriteProtectedMemoryFloat(Settings::offsets::FallSpeed, &Settings::Hacks::Movement::Fallingspeed);
							}


							ImGui::SliderFloat("##JUMPHEIGHTSTARTING_HEIGHT", &Settings::Hacks::Movement::Jumpstartingheight, -350, 350, "Jump starting height %0.3f");
							ImGui::SameLine();
							if (!Settings::Hacks::Movement::MovementJumpStartingHeight)
							{
								if (ImGui::Button("OFF!###ID2222"))
								{
									Settings::Hacks::Movement::MovementJumpStartingHeight = true;
								}
							}
							else
							{
								if (ImGui::Button("ON!!###ID22222"))
								{
									Settings::Hacks::Movement::MovementJumpStartingHeight = false;
								}
							}
							

							ImGui::SliderFloat("##JUMPHEIGHT_SPEED", &Settings::Hacks::Movement::jumpHeightspeed, -350, 350, "Jump height %0.3f");
							ImGui::SameLine();
							if (ImGui::Button("SET!###ID3"))
							{
								WriteProtectedMemoryFloat(Settings::offsets::JumpGravity, &Settings::Hacks::Movement::jumpHeightspeed);
							}

							ImGui::SliderFloat("##JUMPHEIGHTWATER_SPEED", &Settings::Hacks::Movement::jumpHeightwaterspeed, -350, 350, "Jump height water %0.3f");
							ImGui::SameLine();
							if (ImGui::Button("SET!###ID4"))
							{
								WriteProtectedMemoryFloat(JumpHeightWater, &Settings::Hacks::Movement::jumpHeightwaterspeed);
							}

							ImGui::SliderFloat("##WALLCLIMB", &Settings::Hacks::Movement::WallClimb, 0, 350, "Wallclimb angle %0.3f");
							ImGui::SameLine();
							if (ImGui::Button("SET!###ID5"))
							{
								WriteProtectedMemoryFloat(Settings::offsets::WallClimb, &Settings::Hacks::Movement::WallClimb);
							}



							ImGui::SliderFloat("##TURNSPEED", &Settings::Hacks::Movement::Turnspeed, 0, 350, "Turn speed %0.3f");
							ImGui::SameLine();
							if (ImGui::Button("SET!###ID555"))
							{
								Settings::Hacks::Movement::MovementTurnspeed = true;
							}
							/*			
							ImGui::SliderFloat("##FACINGROTATION", &Settings::Hacks::Movement::Facing, -10, 350, "Facing at: %0.3f");
							ImGui::SameLine();
							if (ImGui::Button("SET!###ID5555"))
							{
								Settings::Hacks::Movement::MovementFacing = true;
							}
							*/

							ImGui::NextColumn();
							{
								char* tab2 = new char[GetMapidName().length() + 1];
								strcpy(tab2, GetMapidName().c_str());
								//	ImGui::Text("%s\n", version);
								ImGui::Text("Current Coords:");
								ImGui::Text("X: %f Y: %f Z: %f MAPID: %i ZONE: %s", Settings::Hacks::Movement::CX, Settings::Hacks::Movement::CY, Settings::Hacks::Movement::CZ, Settings::Hacks::Movement::MAPID, tab2);
								ImGui::PushItemWidth(250);
								if (ImGui::Button("Teleport back to the previous location.", ImVec2(-1, 0)))
								{
									Settings::Hacks::Movement::TeleportBack = true;
								}
								//ImGui::SameLine();
								if (ImGui::Button("Teleport to corpse.", ImVec2(-1, 0)))
								{
									Settings::Hacks::Movement::TeleporttoCorpse = true;
								}
								ImGui::PopItemWidth();
								
								if (!tpmenubool)
								{
									if (ImGui::Button("Teleport Menu - not visible", ImVec2(-1, 0)))
									{
										tpmenubool = true;
									}
								}
								else if (tpmenubool)
								{
									if (ImGui::Button("Teleport Menu - visible", ImVec2(-1, 0)))
									{
										tpmenubool = false;
									}
								}
								if (tpmenubool)
								{
									if (ImGui::Begin(XorStr("Teleport Menu"), &Configs::showWindow,
										ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_ShowBorders /*
										ImGuiWindowFlags_NoResize*/)) {
					

										ImGui::Columns(1);
										ImGui::PushItemWidth(500);
										ImGui::InputText("", inputName, sizeof(inputName));
										ImGui::PopItemWidth();
										ImGui::SameLine();
										if (ImGui::Button("Add"))
										{

											if (strlen(inputName) > 0)
											{
												Vector currpos;
												currpos.x = Settings::Hacks::Movement::CX;
												currpos.y = Settings::Hacks::Movement::CY;
												currpos.z = Settings::Hacks::Movement::CZ;


												TeleporterInfo SET = TeleporterInfo(currpos, inputName, throwMessage); 
												Settings::TeleportHelper::TeleporterInfos.push_back(SET);
												throwMessage++;


												const char* GetGhConfigDirectory = "C://Xhook//wow//Configs//TeleportHelper//";

												std::ostringstream path;
												
												path << GetGhConfigDirectory << Settings::Hacks::Movement::MAPID << "//"; 

												const char* GetPathInconst = path.str().c_str();


												CreateDirectoryA(GetPathInconst, nullptr);

												path << XorStr("teleconfig.json");
												Settings::SaveTeleporterInfo(path.str());

											}
											strcpy(inputName, "");
										}

										ImGui::PushItemWidth(550);
								
										auto lambda = [](void* data, int idx, const char** out_text)
										{							
											*out_text = Settings::TeleportHelper::TeleporterInfos.at(idx).name.c_str();	

											return *out_text != NULL;
										};
									
										ImGui::ListBox("", &throwMessageCurrent, lambda, NULL, Settings::TeleportHelper::TeleporterInfos.size(), 7);
										ImGui::PopItemWidth();

										ImGui::Columns(1);

										if (ImGui::Button("Teleport", ImVec2(ImGui::GetWindowWidth(), 30)))
										{

											if (Settings::TeleportHelper::TeleporterInfos.empty())
											{
												printf("Error cant find XYZ nor location string.\n");
											}

											for (auto TeleportInfo = Settings::TeleportHelper::TeleporterInfos.begin(); TeleportInfo != Settings::TeleportHelper::TeleporterInfos.end(); TeleportInfo++)
											{					
											//	printf("Current message is :%i  Saved message is: %i", throwMessageCurrent, grenadeInfo->currmsg);
												if (throwMessageCurrent == TeleportInfo->currmsg)
												{

													Teleportb = true;
													Teleport(TeleportInfo->pos.x, TeleportInfo->pos.y, TeleportInfo->pos.z);											
												}
											
											}
											

										}
										if (ImGui::Button("Replace", ImVec2(ImGui::GetWindowWidth(), 20)))
											if (throwMessageCurrent > -1 && (int)Settings::TeleportHelper::TeleporterInfos.size() > throwMessageCurrent)
											{
												
												int SavedthrowMessageCurrent = throwMessageCurrent;
												Settings::TeleportHelper::TeleporterInfos.erase(Settings::TeleportHelper::TeleporterInfos.begin() + throwMessageCurrent);

												Vector currpos;
												currpos.x = Settings::Hacks::Movement::CX;
												currpos.y = Settings::Hacks::Movement::CY;
												currpos.z = Settings::Hacks::Movement::CZ;
												
												std::string NoInput = "NO_INPUT_WAS_GIVEN"; // <== We should get the zone name with LUA
												if (strlen(inputName) <= 0)
												{
													TeleporterInfo SET = TeleporterInfo(currpos, NoInput, SavedthrowMessageCurrent);
													Settings::TeleportHelper::TeleporterInfos.push_back(SET);
												}
												else
												{
													TeleporterInfo SET = TeleporterInfo(currpos, inputName, throwMessage);
													Settings::TeleportHelper::TeleporterInfos.push_back(SET);
												}
																	
												const char* GetGhConfigDirectory = "C://Xhook//wow//Configs//TeleportHelper//";

												std::ostringstream path;

												path << GetGhConfigDirectory << Settings::Hacks::Movement::MAPID << "//"; // KALIMDOR/EASTERKINGDOM BUT HOW ABOUT DUNGEONS??

												const char* GetPathInconst = path.str().c_str();


												CreateDirectoryA(GetPathInconst, nullptr);

												path << XorStr("teleconfig.json");
												Settings::SaveTeleporterInfo(path.str());
												

												
											}

										//if (ImGui::Button("Remove", ImVec2(ImGui::GetWindowWidth(), 20)))
										//	if (throwMessageCurrent > -1 && (int)Settings::TeleportHelper::TeleporterInfos.size() > throwMessageCurrent)
										//	{
										//		//Should fix this Remove just fucks up the entire Vector.
										//		Settings::TeleportHelper::TeleporterInfos.erase(Settings::TeleportHelper::TeleporterInfos.begin() + throwMessageCurrent);

										//	}
										if (ImGui::Button("Manual Reload the maplist", ImVec2(ImGui::GetWindowWidth(), 20)))
										{
											ReloadTeleportListonmapchange(true);
										}

										
										ImGui::End();
									}
								}
										
							}

							ImGui::Columns(1);
							ImGui::Separator();
							ImGui::Text("More features");
							ImGui::Separator();
							ImGui::Columns(2, NULL, true);
							{				
								ImGui::Separator();
								ImGui::Checkbox("AntiJump", &Settings::Hacks::Movement::AntiJump);
								ImGui::SameLine();
								ImGui::Checkbox("NoFallDamage", &Settings::Hacks::Movement::NoFallDamage);
								ImGui::Checkbox("AntiRoot", &Settings::Hacks::Movement::AntiRoot);
								ImGui::SameLine();
								ImGui::Checkbox("Infinite Jumps", &Settings::Hacks::Movement::Infintejump);
								ImGui::Checkbox("AntiMove", &Settings::Hacks::Movement::AntiMove);					
								ImGui::SameLine();
								ImGui::Checkbox("Fly", &Settings::Hacks::Movement::SuperFly);							
								ImGui::SameLine();
								ImGui::Checkbox("NoClip", &Settings::Hacks::Movement::MovementNoClip);
								//SetTooltip("Be aware Activating this without a flyhack will probably kill you.");

								ImGui::Checkbox("Nocollision1", &Settings::Hacks::Misc::NoCollisionBuildings);
								SetTooltip("Buildings");
								ImGui::SameLine();
								ImGui::Checkbox("NoCollision2", &Settings::Hacks::Misc::NoCollisionTrees);
								SetTooltip("Objects like trees.");
								ImGui::SameLine();
								ImGui::Checkbox("NoCollision3", &Settings::Hacks::Misc::NoCollisionMailboxes);
								SetTooltip("Other objects.");

								ImGui::NextColumn();
								{			
									if (!DrawOnSCreen)
									{
										if (ImGui::Button("DRAW XYZ - OFF"))
										{									
											DrawOnSCreen = true;
										}
									}
									else
									{
										if (ImGui::Button("DRAW XYZ - ON"))
										{
											DrawOnSCreen = false;
										}
									}
							
									static bool check = false;

									if (!check)
									{
										if (ImGui::Button("Remove LUA Protection"))
										{
											WriteProtectedMemory(0x494a57, LuaRemover, 6);
											printf("Lua protection removed.\n");
											Console::Write("Lua protection removed.", ECHO_COLOR);
											check = true;
										}
									}
									if (!Settings::Hacks::Movement::test)
									{
										if (ImGui::Button("Movement freeze - on"))
										{
											Settings::Hacks::Movement::test = true;
											byte step1[]{ 0x20 };
											byte step2[]{ 0x00 };
											WriteProtectedMemory(MovementstatefreezeFIRST, step1, 1);
											WriteProtectedMemory(MovementstatefreezeSEC, step2, 1);
										}
									}
									else
									{
										if (ImGui::Button("Movement freeze - off"))
										{
											Settings::Hacks::Movement::test = false;
											byte step1[]{ 0x00 };
											byte step2[]{ 0x20 };

											WriteProtectedMemory(MovementstatefreezeFIRST, step1, 1);
											WriteProtectedMemory(MovementstatefreezeSEC, step2, 1);
										}
									}
									SetTooltip("Freezes your CURRENT movetype.\n Turn off then on to let this work.");


									if (ImGui::Button("Loot while mounted"))
									{
										nop_((PVOID)0x0060BFA0, 1);
									}
									SetTooltip("Method 1. (NOPS) didn't test it.");
									ImGui::SameLine();
									if (ImGui::Button("Loot while mounted2"))
									{
										Settings::Hacks::Movement::Lootpatch = true;
									}
									SetTooltip("Metod 2. (BYTES) did't test it.");


									if (ImGui::Button("PANIC BUTTON!"))
									{
										RestoreOriginalB = true;
										WriteAfterInjection = true;
										RestoreOriginal();
									}
									SetTooltip("Restore MOST values to original");
								}				
							}

							ImGui::Columns(1);
							ImGui::EndChild();
							ImGui::EndChild();
						}
					}


				}
				break;

			case 1:
				static int colorSelected = 0;
				for (int i = 0; i < IM_ARRAYSIZE(colors); i++)
					colorNames[i] = colors[i].name;

				ImGui::SetNextWindowSize(ImVec2(540, 325), ImGuiSetCond_Always);
				ImGui::Columns(2, NULL, true);
				{
					ImGui::PushItemWidth(-1);
					ImGui::ListBox("##COLORSELECTION", &colorSelected, colorNames, IM_ARRAYSIZE(colorNames), 12);
					ImGui::PopItemWidth();
					ImGui::Checkbox("Background", &Settings::Background::enable);
					if (ImGui::Button("Reload Background"))
					{
						if (D3DXCreateTextureFromFile(device, "C://xhook//wow//bg.jpg", &BackgroundTexture) != D3D_OK) {
							ImGui::OpenPopup("Success###Picture Reloaded!");
						}
					}
					SetTooltip("Save your BG at:\n C://xhook//wow//bg.jpg");
					ImGui::Checkbox("Config window", &Configs::showWindow);
				}
				ImGui::NextColumn();
				{
					if (colors[colorSelected].type == ColorListVar::HEALTHCOLORVAR_TYPE)
					{
						ColorPicker4((float*)colors[colorSelected].healthColorVarPtr);
						ImGui::Checkbox("Rainbow", &colors[colorSelected].healthColorVarPtr->rainbow);
						SetTooltip("Makes the color an animated rainbow.");
						ImGui::SameLine();
						ImGui::Checkbox("Health-Based", &colors[colorSelected].healthColorVarPtr->hp);
						SetTooltip("Takes color from entity health, i.e. 100 - green, 50 - yellow.");
						ImGui::PushItemWidth(-1);
						ImGui::SliderFloat("##RAINBOWSPEED", &colors[colorSelected].healthColorVarPtr->rainbowSpeed, 0.f, 1.f, "Rainbow Speed: %0.3f");
						ImGui::PopItemWidth();
					}
					else
					{
						ColorPicker4((float*)colors[colorSelected].colorVarPtr);
						ImGui::Checkbox("Rainbow", &colors[colorSelected].colorVarPtr->rainbow);
						SetTooltip("Makes the color an animated rainbow.");
						ImGui::PushItemWidth(-1);
						ImGui::SliderFloat("##RAINBOWSPEED", &colors[colorSelected].colorVarPtr->rainbowSpeed, 0.f, 1.f, "Rainbow Speed: %0.3f");

					}					
				}

				break;
			}
			ImGui::End();
		}
		ImGui::Render();
	}

	device->SetRenderState(D3DRS_COLORWRITEENABLE, dwOld_D3DRS_COLORWRITEENABLE);

	return o_EndScene(device);
}

HRESULT __stdcall Reset_h(IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	////Reset
	//D3D_RELEASE(pFont);
	//D3D_RELEASE(pLine);

	//if (pFont)
	//	pFont->OnLostDevice();

	///*ImGui_ImplDX9_CreateDeviceObjects();
	//SetWindowLongA(GAME_HWND, GWL_WNDPROC, (LONG)g_gameWndProc);*/

	//HRESULT hRet = pDevice->Reset(pPresentationParameters);
	//if (pFont)
	//	pFont->OnResetDevice();

	//if (SUCCEEDED(hRet)) {
	//	m_PresentParam = *pPresentationParameters;
	//}
	//return hRet;

	return o_Reset(pDevice, pPresentationParameters);
}

HRESULT __stdcall Present_h(IDirect3DDevice9* pDevice, RECT* pSourceRect, RECT* pDestRect, HWND hDestWindowOverride, RGNDATA* pDirtyRegion)
{
	//printf("Present_h hooked! \n");

	return o_Present(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

bool PressedKeys[256] = {};
LRESULT __stdcall WndProc_h(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//Helpers::Log("Wndproc called");

	switch (uMsg) // record window message
	{
	case WM_LBUTTONDOWN:
		PressedKeys[VK_LBUTTON] = true;
		break;
	case WM_LBUTTONUP:
		PressedKeys[VK_LBUTTON] = false;
		break;
	case WM_RBUTTONDOWN:
		PressedKeys[VK_RBUTTON] = true;
		break;
	case WM_RBUTTONUP:
		PressedKeys[VK_RBUTTON] = false;
		break;
	case WM_MBUTTONDOWN:
		PressedKeys[VK_MBUTTON] = true;
		break;
	case WM_MBUTTONUP:
		PressedKeys[VK_MBUTTON] = false;
		break;
	case WM_XBUTTONDOWN:
	{
		UINT button = GET_XBUTTON_WPARAM(wParam);
		if (button == XBUTTON1)
		{
			PressedKeys[VK_XBUTTON1] = true;
		}
		else if (button == XBUTTON2)
		{
			PressedKeys[VK_XBUTTON2] = true;
		}
		break;
	}
	case WM_XBUTTONUP:
	{
		UINT button = GET_XBUTTON_WPARAM(wParam);
		if (button == XBUTTON1)
		{
			PressedKeys[VK_XBUTTON1] = false;
		}
		else if (button == XBUTTON2)
		{
			PressedKeys[VK_XBUTTON2] = false;
		}
		break;
	}
	case WM_KEYDOWN:
		PressedKeys[wParam] = true;
		break;
	case WM_KEYUP:
		PressedKeys[wParam] = false;
		break;
	default:
		break;
	}

	return CallWindowProcW((WNDPROC)o_WndProc, hWnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK hWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplDX9_WndProcHandler(hwnd, uMsg, wParam, lParam) && GetKeyState(VK_DELETE) == 1) {
		return 1l;
	}
	return CallWindowProc((WNDPROC)WndProc_h, hwnd, uMsg, wParam, lParam);
}

HRESULT __stdcall BeginScene_h(IDirect3DDevice9* pDevice)
{
	//	//printf("BeginScene_h honk \n");
//	D3DXCreateFontA(pDevice, 25, 0, FW_HEAVY, 1, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial", &pFont);
//	D3DXCreateLine(pDevice, &d3dLine);

	//if (!bOnce)
	//{
	//	D3DDEVICE_CREATION_PARAMETERS pPara;
	//	pDevice->GetCreationParameters(&pPara);
	//	gameWindow = pPara.hFocusWindow;

	//	//hook wndproc and window size
	//	o_WndProc = SetWindowLongPtr(gameWindow, GWLP_WNDPROC, (LONG_PTR)WndProc_h);
	//	GetWindowRect(gameWindow, gameWindowRect);

	//	//char gameName[256];
	//	//GetClassName(gameWindow, gameName, sizeof(gameName - 2));
	//	//printf("%s\n", gameName);
	//	//bOnce = true;
	//}
	return o_BeginScene(pDevice);
}



uintptr_t guard_function;
hook_fn original_fn;

bool find_device(const HMODULE d3d_module)
{
	// get cfg return address
	return_address = get_return_address(d3d_module);

	if (return_address == 0) {
		printf("could not find return address\n");
		return false;
	}

	printf("return address: 0x%08X\n", return_address);

	if (ImageNtHeader(d3d_module) == nullptr)
	{
		printf("d3d9 module is not a valid pe image!\n");
		return false;
	}

	// get load config directory entry
	ULONG directory_size = 0;
	const auto load_config = PIMAGE_LOAD_CONFIG_DIRECTORY(ImageDirectoryEntryToData(d3d_module, TRUE, IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG, &directory_size));

	if (load_config == nullptr)
	{
		printf("could not find IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG\n");
		return false;
	}

	printf("load config: 0x%p\n", load_config);

	guard_function = load_config->GuardCFCheckFunctionPointer;

	printf("guard function: 0x%08X\n", guard_function);

	// replace original function with ours
	original_fn = patch(guard_function, cfg_hook);

	if (original_fn == nullptr)
	{
		printf("could not patch GuardCFCheckFunctionPointer\n");
		return false;
	}

	printf("original function: 0x%p\n", original_fn);

	while (true)
	{
		if (HIBYTE(GetAsyncKeyState(VK_DELETE)))
		{
			break;
		}

		// wait until d3d device is found
		if (d3d9_device_new != 0)
		{
			//.data:00837C04 00000007 C 1.12.1
			string xhook_version = ("1.12.1");
			char* CurrentSupportedversion = MemMod::getValueChar(0x00837C04);
		//	printf("%s\n", Currentversion);

			if (xhook_version != CurrentSupportedversion)
			{
				printf("XHOOK for World of Warcraft only supports patch 1.12.1\n Aborting...");
				break;
			}
			printf("d3d9 device: 0x%I32X\n", d3d9_device_new);

			DWORD_PTR* pDeviceVT = reinterpret_cast<DWORD_PTR*>(d3d9_device_new);
			pDeviceVT = reinterpret_cast<DWORD_PTR*>(pDeviceVT[0]);

			IDirect3DDevice9* ref = nullptr;

			o_EndScene = reinterpret_cast<EndScene_t>(pDeviceVT[42]);
		/*	o_BeginScene = reinterpret_cast<BeginScene_t>(pDeviceVT[41]);
			o_Reset = reinterpret_cast<Reset_t>(pDeviceVT[16]);
			o_Present = reinterpret_cast<Present_t>(pDeviceVT[17]);*/

			pD3device = **reinterpret_cast<IDirect3DDevice9 * **>(pDeviceVT);

			renderer.Initialize(FindWindowA(XorStr("GxWindowClassD3d"), NULL), pD3device);

			//HookFunction(reinterpret_cast<PVOID*>(&o_Reset), Reset_h);
		    //HookFunction(reinterpret_cast<PVOID*>(&o_Present), Present_h);
			//HookFunction(reinterpret_cast<PVOID*>(&o_BeginScene), BeginScene_h);
			HookFunction(reinterpret_cast<PVOID*>(&o_EndScene), EndScene_h);

			Main::main();


			Beep(650, 50);
			break;
		}

		using namespace std::chrono_literals;
		std::this_thread::sleep_for(50ms);
	}

	// restore original function
	patch(guard_function, original_fn);

	return true;
}


DWORD WINAPI initialize(void* mod)
{


	const auto current_module = static_cast<HMODULE>(mod);
	const auto d3d9_module = GetModuleHandle(TEXT("d3d9.dll"));
	// don't allocate a console if there's already one
	const auto allocate_console = GetConsoleWindow() == nullptr;

	if (allocate_console) {
		AllocConsole();
		redirect_stdout();
	}

	if (d3d9_module != nullptr) {
		if (find_device(d3d9_module))
			printf("success\n");
		else
			printf("fail!\n");
	}
	else
	{
		printf("could not find d3d9 module\n");
	}

	while (!GetAsyncKeyState(VK_DELETE))
	{
		Sleep(5);
	}

	using namespace std::chrono_literals;
	std::this_thread::sleep_for(500ms);

	if (allocate_console) {
		redirect_stdout();
		FreeConsole();
	}

	UnhookFunction(reinterpret_cast<PVOID*>(&o_EndScene), EndScene_h);
	//UnhookFunction(reinterpret_cast<PVOID*>(&o_Reset), Reset_h);
	//UnhookFunction(reinterpret_cast<PVOID*>(&o_Present), Present_h);
	//UnhookFunction(reinterpret_cast<PVOID*>(&o_BeginScene), BeginScene_h);
	if (o_WndProc)
		SetWindowLongPtr(gameWindow, GWL_WNDPROC, o_WndProc);

	patch(guard_function, original_fn);

	// unload
	FreeLibraryAndExitThread(current_module, 0);
	//return 0;
}

bool SetTextColor(DWORD dwFore, DWORD dwBack)
{
	auto hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	return SetConsoleTextAttribute(hConsole, dwFore | dwBack << 4);
}


BOOL WINAPI DllMain(const HMODULE mod, const ULONG reason, PVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		EnableConsole();
		Console::Write("XHOOK for World of Warcraft v1.0 Loaded", WARNING_COLOR); // RED
		const auto thread = CreateThread(nullptr, 0, initialize, mod, 0, nullptr);
		CloseHandle(thread);
	}
	return TRUE;
}


