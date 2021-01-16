/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     API for getting character information
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#pragma once

#include <Windows.h>
#include <atlcoll.h>
#include "GenshinAPIBase.h"
#include "GenshinBasic.h"




typedef struct __GENSHIN_CHARACTER_RESULT :
    GENSHIN_API_RESULT
{
    ATL::CAtlArray<GENSHIN_AVATAR_DETAILED_DATA> AvatarData;

}GENSHIN_CHARACTER_RESULT;


#ifdef __cplusplus
extern "C" BOOL GenshinAPICharacter(const WCHAR UID[], ATL::CAtlArray<int> & AvatarID, GENSHIN_CHARACTER_RESULT & Result);
#else
BOOL GenshinAPICharacter(const WCHAR UID[], ATL::CAtlArray<int> &AvatarID, GENSHIN_CHARACTER_RESULT &Result);
#endif
