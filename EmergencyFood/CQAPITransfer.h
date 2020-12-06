/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     Translate CoolQ API call
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "CoolQDef.h"
#include "CoolQ_State.h"

int SendPrivateMessage(long long QQID, const WCHAR * msg);

int SendGroupMessage(long long GroupID, const WCHAR * msg);

const char* GetGroupMemberInfo(long long GroupID, long long QQID, BOOL NoCache);

const char* GetStrangerInfo(long long QQID, BOOL NoCache);

const char* GetAppDirectory();