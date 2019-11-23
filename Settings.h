#pragma once
#include <Windows.h>
#include <unordered_map>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <vector>
#include "ImGui\imgui.h"
#include "Util.h"
#include "json.h"



//enum class CrossHairType : int
//{
//	NoCross,
//	Cross,
//	Cross2,
//	Cross3,
//};
////
//namespace Settings
//{
//	namespace Misc
//	{
//	//	extern bool Enabled;
//		extern bool CrossHair;
//		//extern CrossHairType CrossType;
//	}
//}



class ColorVar
{
public:
	ImColor color;
	bool rainbow;
	float rainbowSpeed;

	ColorVar() {}

	ColorVar(ImColor color)
	{
		this->color = color;
		this->rainbow = false;
		this->rainbowSpeed = 0.5f;
	}

	ImColor Color()
	{
		ImColor result = this->rainbow ? Util::GetRainbowColor(this->rainbowSpeed) : this->color;
		result.Value.w = this->color.Value.w;
		return result;
	}
};



class HealthColorVar : public ColorVar
{
public:
	bool hp;

	HealthColorVar(ImColor color)
	{
		this->color = color;
		this->rainbow = false;
		this->rainbowSpeed = 0.5f;
		this->hp = false;
	}

	ImColor Color()
	{
		ImColor result;// = this->rainbow ? Util::GetRainbowColor(this->rainbowSpeed) : (this->hp ? Color::ToImColor(Util::GetHealthColor(100)) : this->color);
		result.Value.w = this->color.Value.w;
		return result;
	}
};







namespace Settings
{
	

	namespace TeleportHelper
	{
		extern std::vector<TeleporterInfo> TeleporterInfos;
		extern bool enabled;
		extern std::string actMapName;
	}


	namespace offsets
	{
		extern DWORD GetBaseAddress(), playerAddress, playerAddressDUMMY;
		
		extern DWORD_PTR 
			PlayerBase,
			Playerbaseptr1,
			Playerbaseptr2,
			Playerbaseptr3,
			Playerbaseptr4,
			Playerbasespeedptr, 
			InfiniteJump, // 1.12.1.5875 (Array) - On: { 0x10 } - Off: { 0x30 }		
			SuperFly, // 1.12.1.5875 (Array) - On: { 0x90, 0x90 } - Off: { 0x74, 0x25 }
			NoFallDamage, // 1.12.1.5875 (Array) - On: { 0x31, 0xC9, 0x90 } - Off: { 0x8B, 0x4F, 0x78 }

			AntiJump, // 1.12.1.5875 (Array) - On: { 0xEB } - Off: { 0x75 }
			Swimandfish,  //JMP 00609DE3 [0xEB, 0x3D]
			AntiMove, // 1.12.1.5875 (Array) - On: { 0xFE } - Off: { 0xF8 }
			AntiRoot, // 1.12.1.5875 (Array) - On: { 0xEB, 0xF9 } - Off: { 0x8A, 0x47 }
			JumpGravity, // 1.12.1.5875 (Float)
			JumpGravityWater, // 1.12.1.5875 (Float)
			WallClimb, // 1.12.1.5875 (Float)
			HeartbeatInterval, // 1.12.1.5875 (UInt)  - Speedhack anti-disconn
			TerrainCollision,
			WMOCollision,
			M2Collision1,
			M2Collision2,
			RemoveAFK,
			FallSpeed,
			isingameptr,
			targetguidptr,
			SeeAllLevelsptr,
			Understandallptr,
			NoCollisionTreesptr,
			NoCollisionBuildingsptr,
			NoCollisionMailboxesptr;


		//FUNCTIONS
		extern DWORD_PTR
			LastHardwareAction,
			AutoLoot,
			ClickToMove,
			GetText,
			DoString,
			GetEndscene,
			IsLooting,
			GetLootSlots,
			OnRightClickObject,
			OnRightClickUnit,
			SetFacing,
			SendMovementPacket,
			PerformDefaultAction,
			CGInputControl__GetActive,
			CGInputControl__SetControlBit,
			EnumVisibleObjects,
			GetPtrForGuid,
			ClntObjMgrGetActivePlayer,
			ClntObjMgrGetMapId,
			NetClientSend;

		//Descriptor : ptrdiff_t 
		extern DWORD_PTR
			GotLoot,
			SummonedByGuid,
			DynamicFlags,
			ChannelingSpellId ,
			CreatedByGuid ,
			GameObjectCreatedByGuid,
			MovementFlags,
			Health ,
			MaxHealth,
			NpcId,
			FactionId,
			Mana,
			MaxMana,
			Rage,
			TargetGuid,
			CastingSpellId,
			UnitNamePtr,
			CorpseOwnedBy,
			ItemId,
			ItemStackCount,
			ItemDurability,
			ItemMaxDurability,
			ContainerId,
			ContainerTotalSlots,
			GameObjPos,
			Unit__CMovementData,
			UnitPos,
			UnitOrientation,
			CorpsePos,
			FirstBuff,
			FirstDebuff,
			NextBuff,
			GameObjNamePtr;
	}

	namespace Objectmanager
	{
		namespace coordinates
		{
			extern int isingame(), GetMovementflags(), GetMapid();
			extern float GetX(), GetY(), GetZ();
		}
	/*	int isingame()
		{
			return *(int*)((uintptr_t)Settings::offsets::isingameptr);
		}

		int GetMovementflags()
		{
			return *(int*)((uintptr_t)Settings::offsets::playerAddress + Settings::offsets::MovementFlags);
		}
		namespace coordinates
		{
			float GetX()
			{
				return *(float*)((uintptr_t)Settings::offsets::playerAddress + 0x9B8);
			}
			float GetY()
			{
				return *(float*)((uintptr_t)Settings::offsets::playerAddress + 0x9BC);
			}
			float GetZ()
			{
				return *(float*)((uintptr_t)Settings::offsets::playerAddress + 0x9C0);
			}
			int MAPID()
			{
				return *(int*)((uintptr_t)Settings::offsets::GetBaseAddress() + 0x84C498);
			}
		}*/
	}

	namespace Hacks
	{
		namespace LuaProtection {
			extern bool toggle;
		}
		namespace Misc {
			extern bool NoCollisionTrees, NoCollisionBuildings, NoCollisionMailboxes;;
			extern bool SeeAllLevels, UnderstandAll;
		}
		namespace Movement
		{
			extern int MovementType, NoClip; //BYTE
			extern float Cameraspeed, Jumpstartingheight, Swimmingspeed, Walkingspeed, Runningspeed, Turnspeed, Facing;
			extern float jumpHeightspeed;
			extern float jumpHeightwaterspeed;
			extern float Movementspeed;
			extern float Fallingspeed;
			extern bool Toggleplayerstate;
			extern float WallClimb;
			extern int Playerstate;
			extern bool Teleport;
			extern bool test, SuperFly, Infintejump, NoFallDamage, AntiJump, AntiMove, AntiRoot, Gravity, MovementFacing, TeleportBack, TeleporttoCorpse, SetCurrentXYZ, MovementNoClip, MovementJumpStartingHeight, MovementCameraspeed, MovementSwimmingspeed, MovementWalkingspeed, MovementRunningspeed, MovementTurnspeed, Lootpatch;
			extern float single;
			extern float CX, CY, CZ; // Current XYZ
			extern float X, Y, Z; // Destination XYZ
			extern float CorpseX, CorpseY, CorpseZ; // Corpse xyz
			extern int MAPID;
		}

	}

	namespace Background
	{
		extern bool enable;
	}
	namespace UI
	{
		extern bool is_renderer_active;
		extern ColorVar mainColor;
		extern ColorVar bodyColor;
		extern ColorVar fontColor;
		extern ColorVar accentColor;

		extern bool MainUI;

		namespace Windows {
			namespace Config {
				extern int posX;
				extern int posY;
				extern int sizeX;
				extern int sizeY;
				extern bool open;
				extern bool reload; // True on config load, used to change Window Position.
			}
		}

	}
	void LoadDefaultsOrSave(std::string path);
	void LoadConfig(std::string path);
	void LoadSettings();
	//namespace Generalptr
	//{
	//	DWORD_PTR
	//		IsInGame = 0x00B4B424; // 1.12.1.5875 (Byte)
	//}

	//namespace Cameraptr
	//{
	//	DWORD_PTR
	//		CameraPtr = 0x0074B2BC, // 1.12.1.5875
	//		CameraPtrOffset = 0x000065B8, // 1.12.1.5875
	//		CameraPosition = 0x00000008, // 1.12.1.5875 (CVec3)
	//		CameraFollowingGUID = 0x00000088; // 1.12.1.5875 (GUID)
	//}

	//namespace ObjectManagerptr
	//{
	//	DWORD_PTR
	//		ObjectDescriptors = 0x00000008, // 1.12.1.5875
	//		ObjectMovementData = 0x00000118, // 1.12.1.5875 (UInt)
	//		ObjectMovementDataPosition = 0x00000010, // 1.12.1.5875 (CVec3)
	//		ObjectMovementDataRotation = 0x0000001C, // 1.12.1.5875 (Float)
	//		ObjectMovementDataState = 0x000000A0, // 1.12.1.5875 (UInt)
	//		ObjectMovementDataFlags = 0x00000040, // 1.12.1.5875 (UInt)
	//		ObjectMovementDataRunSpeed = 0x0000008C, // 1.12.1.5875 (Float)
	//		PlayerObjectTracking = 0x00002EB0; // 1.12.1.5875 (Byte)
	//}

	//namespace Hacksptr
	//{
	//	DWORD_PTR
	//     InfiniteJump = 0x007C625E, // 1.12.1.5875 (Array) - On: { 0x10 } - Off: { 0x30 }		
	//		SuperFly = 0x006341BC, // 1.12.1.5875 (Array) - On: { 0x90, 0x90 } - Off: { 0x74, 0x25 }
	//		NoFallDamage = 0x007C63DA, // 1.12.1.5875 (Array) - On: { 0x31, 0xC9, 0x90 } - Off: { 0x8B, 0x4F, 0x78 }
	//		AntiJump = 0x007C625F, // 1.12.1.5875 (Array) - On: { 0xEB } - Off: { 0x75 }
	//		AntiMove = 0x00615CF5, // 1.12.1.5875 (Array) - On: { 0xFE } - Off: { 0xF8 }
	//		AntiRoot = 0x006163DB, // 1.12.1.5875 (Array) - On: { 0xEB, 0xF9 } - Off: { 0x8A, 0x47 }
	//		JumpGravity = 0x007C6272, // 1.12.1.5875 (Float)
	//		JumpGravityWater = 0x007C6269, // 1.12.1.5875 (Float)
	//		WallClimb = 0x0080DFFC, // 1.12.1.5875 (Float)
	//		HeartbeatInterval = 0x00615BA7; // 1.12.1.5875 (UInt)  - Speedhack anti-disconnect
	//}
	void SaveTeleporterInfo(std::string path);
	void LoadTeleporterInfo(std::string path);
}

