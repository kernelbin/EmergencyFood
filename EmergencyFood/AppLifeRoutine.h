/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     Declaring functions in AppLifeRoutine.c
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#pragma once

#include <Windows.h>


#ifdef __cplusplus
extern "C" {
#endif

int AppInitialize();

int AppFinialize();

int AppEnabled();

int AppDisabled();

int OnRecvMessage(int msgId, MESSAGE_SOURCE MessageSource, LPCWSTR szMsg, int font);

#ifdef __cplusplus
}
#endif
