/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     API for getting user game record
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#pragma once

#include <Windows.h>
#include <atlcoll.h>
#include "GenshinBasic.h"
#include "GenshinAPIBase.h"


typedef struct __GENSHIN_AVATAR_DATA
{
    int AvatarID;
    GENSHIN_ELEMENT Element;
    int Fetter;
    int Level;

    __GENSHIN_AVATAR_DATA()
    {
        AvatarID = 0;
        Element = ELEMENT_NONE;
        Fetter = 0;
        Level = 0;
    }
    
}GENSHIN_AVATAR_DATA;

typedef struct __GENSHIN_STATS_DATA
{
    int ActiveDayNumber;
    int AchievementNumber;
    // in the json returned from API, there's a field called ```win_rate``` here
    // but I have no idea what it's for.
    int AnemoculusNumber;
    int GeoculusNumber;
    int AvatarNumber;
    int WayPointNumber;
    int DomainNumber;
    WCHAR SpiralAbyss[32];
    int PreciousChestNumber;
    int LuxuriousChestNumber;
    int ExquistieChestNumber;
    int CommonChestNumber;

    __GENSHIN_STATS_DATA()
    {
        ActiveDayNumber = 0;
        AchievementNumber = 0;
        AnemoculusNumber = 0;
        GeoculusNumber = 0;
        AvatarNumber = 0;
        WayPointNumber = 0;
        DomainNumber = 0;
        wcscpy_s(SpiralAbyss, _countof(SpiralAbyss), L"");
        PreciousChestNumber = 0;
        LuxuriousChestNumber = 0;
        ExquistieChestNumber = 0;
        CommonChestNumber = 0;
    }

}GENSHIN_STATS_DATA;

typedef struct __GENSHIN_EXPLORATION_DATA
{
    int Level;
    int ExplorationPercentage;
    WCHAR Name[32];

    __GENSHIN_EXPLORATION_DATA()
    {
        Level = 0;
        ExplorationPercentage = 0;
        wcscpy_s(Name, _countof(Name), L"");
    }
}GENSHIN_EXPLORATION_DATA;

typedef struct __GENSHIN_USER_GAME_RECORD_RESULT :
    GENSHIN_API_RESULT
{
    ATL::CAtlArray<GENSHIN_AVATAR_DATA> AvatarData;

    GENSHIN_STATS_DATA StatsData;

    ATL::CAtlArray<__GENSHIN_EXPLORATION_DATA> ExploationData;

}GENSHIN_USER_GAME_RECORD_RESULT;

#ifdef __cplusplus
extern "C" BOOL GenshinGetUserGameRecord(const WCHAR UID[], GENSHIN_USER_GAME_RECORD_RESULT &Result);
#else
BOOL GenshinGetUserGameRecord(const WCHAR UID[], GENSHIN_USER_GAME_RECORD_RESULT *Result);
#endif

