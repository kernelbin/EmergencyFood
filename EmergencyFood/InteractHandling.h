/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     Functions about handling interaction
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#pragma once

#include <Windows.h>

int GetSpacingCharLength(LPCWSTR szMsg);

BOOL IsCallingPrefix(LPCWSTR szMsg, int &PrefixLenght);

