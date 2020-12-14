/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     API for getting user game record
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#pragma once

#include <Windows.h>
#include "GenshinBasic.h"

typedef struct
{
    int AvatarID;
    GENSHIN_ELEMENT Element;
    int Fetter;
    int Level;
    
}AVATAR_DATA;

typedef struct
{
    int RetCode;

    int AvatarCount;
    AVATAR_DATA AvatarData[32]; // 32 is already more that the number of characters for now

}USER_GAME_RECORD_RESULT;

#ifdef __cplusplus
extern "C" BOOL GenshinGetUserGameRecord(const WCHAR UID[], USER_GAME_RECORD_RESULT * Result);
#else
BOOL GenshinGetUserGameRecord(const WCHAR UID[], USER_GAME_RECORD_RESULT *Result);
#endif

