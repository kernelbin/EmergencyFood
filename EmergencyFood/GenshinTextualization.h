/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     Textualize API result.
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#pragma once
#include <Windows.h>
#include <atlstr.h>
#include "GenshinGetGameRecord.h"


BOOL GetTextualizedAvatarInfo(ATL::CAtlArray<GENSHIN_AVATAR_DATA> &AvatarData, ATL::CStringW &Result);
BOOL GetTextualizedStatsInfo(GENSHIN_STATS_DATA &StatsData, ATL::CStringW &Result);

