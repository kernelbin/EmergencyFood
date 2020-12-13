/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     API for getting user game record
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#pragma once

#include<Windows.h>

typedef struct
{
    int RetCode;

}USER_GAME_RECORD_RESULT;

#ifdef __cplusplus
extern "C" BOOL GenshinGetUserGameRecord(const WCHAR UID[], USER_GAME_RECORD_RESULT * Result);
#else
BOOL GenshinGetUserGameRecord(const WCHAR UID[], USER_GAME_RECORD_RESULT *Result);
#endif

