#include "ObjectMgr.h"



uint64(__cdecl* ObjectMgr::fpGetActivePlayerGuid)() = (ClntObjMgrGetActivePlayerGuidPtr)0x004D3790;
CGObject_C* (__cdecl* ObjectMgr::fpGetObjectPtr)(uint64 objectGuid, TypeMask objectTypeMask, const char* file, int line) = (ClntObjMgrObjectPtr)0x004D4DB0;
BOOL(__cdecl* ObjectMgr::fpEnumVisibleObjects)(VisibleObjectsEnumProc proc, void* param) = (ClntObjMgrEnumVisibleObjectsPtr)0x004D4B30;
