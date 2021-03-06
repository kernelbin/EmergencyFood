﻿/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     Textualize API result.
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#include <Windows.h>
#include <cstdlib>
#include "GenshinAPIGetGameRecord.h"
#include "GenshinTextualization.h"

BOOL GetTextualizedAvatarInfo(ATL::CAtlArray<GENSHIN_AVATAR_DATA> &AvatarData, ATL::CStringW &Result)
{
    Result = L"派蒙查到了！";
    for (unsigned int i = 0; i < AvatarData.GetCount(); i++)
    {
        const WCHAR *AvatarName = GetAvatarNameByID(AvatarData[i].AvatarID);
        if (AvatarName)
        {
            ATL::CStringW Buffer;
            Buffer.Format(L"%ls：%d级，好感度%d", AvatarName, AvatarData[i].Level, AvatarData[i].Fetter);
            Result.Append(Buffer);
            if (i != AvatarData.GetCount() - 1)
            {
                Result.Append(L" | ");
            }
        }
    }

    return TRUE;
}

BOOL GetTextualizedStatsInfo(GENSHIN_STATS_DATA &StatsData, ATL::CStringW &Result)
{
    Result.Format(L"派蒙查到了！"
    L"活跃天数：%d  "
    L"达成成就：%d  "
    L"获得角色数：%d  "
    L"深境螺旋：%ls  "
    L"华丽/珍贵/精致/普通宝箱：%d/%d/%d/%d",
        StatsData.ActiveDayNumber,
        StatsData.AchievementNumber,
        StatsData.AvatarNumber,
        StatsData.SpiralAbyss,
        StatsData.LuxuriousChestNumber,
        StatsData.PreciousChestNumber,
        StatsData.ExquistieChestNumber,
        StatsData.CommonChestNumber);

    return TRUE;
}

BOOL GetTextualizedExplorationInfo(ATL::CAtlArray<GENSHIN_EXPLORATION_DATA> &ExplorationData, ATL::CStringW &Result)
{
    Result = L"派蒙查到了！";
    for (unsigned int i = 0; i < ExplorationData.GetCount(); i++)
    {
        ATL::CStringW Buffer;
        Buffer.Format(L"%ls：%d级，%d.%d%%",
            ExplorationData[i].Name,
            ExplorationData[i].Level,
            ExplorationData[i].ExplorationPercentage / 10, ExplorationData[i].ExplorationPercentage % 10);

        Result.Append(Buffer);
        if (i != ExplorationData.GetCount() - 1)
        {
            Result.Append(L" | ");
        }
    }
    return TRUE;
}

BOOL GetTextualizedAvatarDetailedInfo(ATL::CAtlArray<GENSHIN_AVATAR_DETAILED_DATA> &AvatarData, ATL::CStringW &Result)
{
    Result = L"派蒙查到了！\r\n";
    for (unsigned int i = 0; i < AvatarData.GetCount(); i++)
    {
        const WCHAR *AvatarName = GetAvatarNameByID(AvatarData[i].AvatarID);
        if (AvatarName)
        {
            ATL::CStringW Buffer;
            Buffer.Format(L"\
%ls：%d级，好感度%d\r\n\
武器：%ls，%d级\r\n",
AvatarName, AvatarData[i].Level, AvatarData[i].Fetter,
AvatarData[i].Weapon.Name, AvatarData[i].Weapon.Level);
            Result.Append(Buffer);

            Result.AppendFormat(L"圣遗物：");

            for (int iReliquary = 0; iReliquary < 5; iReliquary++)
            {
                if (AvatarData[i].bReliquaryExists[iReliquary])
                {
                    Result.AppendFormat(L"\r\n%ls，%d级",
                        AvatarData[i].Reliquaries[iReliquary].Name, AvatarData[i].Reliquaries[iReliquary].Level);
                }
            }
            if (i != AvatarData.GetCount() - 1)
            {
                Result.Append(L"\r\n-------\r\n");
            }
        }
    }

    return TRUE;
}