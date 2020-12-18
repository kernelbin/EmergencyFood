/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     Textualize API result.
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#pragma once
#include <Windows.h>
#include "GenshinGetGameRecord.h"

WCHAR *GetTextualizedAvatarInfo(GENSHIN_AVATAR_DATA *AvatarData, int AvatarCount);
void ReleaseTextualizedAvatarInfo(WCHAR *String);

