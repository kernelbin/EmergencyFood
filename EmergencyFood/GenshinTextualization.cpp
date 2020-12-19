/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     Textualize API result.
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#include <Windows.h>
#include <atlstr.h>
#include <cstdlib>
#include "GenshinGetGameRecord.h"

extern "C" WCHAR *GetTextualizedAvatarInfo(GENSHIN_AVATAR_DATA * AvatarData, int AvatarCount)
{
    ATL::CStringW Result;
    
    Result = L"派蒙查到了！";
    for (int i = 0; i < AvatarCount; i++)
    {
        const WCHAR *AvatarName = GetAvatarNameByID(AvatarData[i].AvatarID);
        if (AvatarName)
        {
            ATL::CStringW Buffer;
            Buffer.Format(L"%ls：%d级，好感度%d", AvatarName, AvatarData[i].Level, AvatarData[i].Fetter);
            Result.Append(Buffer);
            if (i != AvatarCount - 1)
            {
                Result.Append(L" | ");
            }
        }
    }

    WCHAR *String = (WCHAR *)malloc(sizeof(WCHAR) * (Result.GetLength() + 1));
    wcscpy_s(String, Result.GetLength() + 1, Result.GetString());

    return String;
}

extern "C" void ReleaseTextualizedAvatarInfo(WCHAR *String)
{
    free(String);
    return;
}

extern "C" WCHAR * GetTextualizedStatsInfo(GENSHIN_STATS_DATA * StatsData)
{
    ATL::CStringW Result;

    Result.Format(L"派蒙查到了！"
    L"活跃天数：%d  "
    L"达成成就：%d  "
    L"获得角色数：%d  "
    L"深境螺旋：%ls  "
    L"华丽/珍贵/精致/普通宝箱：%d/%d/%d/%d",
        StatsData->ActiveDayNumber,
        StatsData->AchievementNumber,
        StatsData->AvatarNumber,
        StatsData->SpiralAbyss,
        StatsData->LuxuriousChestNumber,
        StatsData->PreciousChestNumber,
        StatsData->ExquistieChestNumber,
        StatsData->CommonChestNumber);

    WCHAR *String = (WCHAR *)malloc(sizeof(WCHAR) * (Result.GetLength() + 1));
    wcscpy_s(String, Result.GetLength() + 1, Result.GetString());

    return String;
}

extern "C" void ReleaseTextualizedStatsInfo(WCHAR * String)
{
    free(String);
    return;
}