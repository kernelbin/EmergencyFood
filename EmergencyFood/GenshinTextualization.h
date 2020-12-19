/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     Textualize API result.
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#pragma once
#include <Windows.h>
#include "GenshinGetGameRecord.h"


#ifdef __cplusplus
extern "C" {
#endif

WCHAR *GetTextualizedAvatarInfo(GENSHIN_AVATAR_DATA *AvatarData, int AvatarCount);
void ReleaseTextualizedAvatarInfo(WCHAR *String);
WCHAR *GetTextualizedStatsInfo(GENSHIN_STATS_DATA *StatsData);
void ReleaseTextualizedStatsInfo(WCHAR *String);

#ifdef __cplusplus
}
#endif
