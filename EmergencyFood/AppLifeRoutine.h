/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     Declaring functions in AppLifeRoutine.c
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#pragma once

#include <Windows.h>


int AppInitialize();

int AppFinialize();

int AppEnabled();

int AppDisabled();

int HandlePrivateMessage(int subType, int msgId, long long fromQQ, const WCHAR *msg, int font);

int HandleGroupMessage(int subType, int msgId, long long fromGroup, long long fromQQ, const char *fromAnonymous, const WCHAR *msg, int font);
