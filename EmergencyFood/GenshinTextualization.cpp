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