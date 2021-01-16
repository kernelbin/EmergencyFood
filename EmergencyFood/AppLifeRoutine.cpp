/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     Life routine of this app
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#include <Windows.h>
#include <wchar.h>
#include <atlcoll.h>
#include "CQAPITransfer.h"
#include "AppLifeRoutine.h"
#include "GenshinAPIBase.h"
#include "InteractHandling.h"

//生命周期

extern "C" int AppInitialize()//初始化时会被执行
{
    InitializeGenshinAPI(TRUE);
    return 0;
}

extern "C" int AppFinialize()//结束时会被执行
{
    InitializeGenshinAPI(FALSE);
    return 0;
}

extern "C" int AppEnabled()//启用时执行（如果初始化时是启用的，会在AppInitialize后执行一次）
{
    return 0;
}

extern "C" int AppDisabled()//禁用时执行（如果结束时是启用的，会在AppFinialize前执行一次，这点和cq原生函数不一样）
{
    return 0;
}

extern "C" int OnRecvMessage(int msgId, MESSAGE_SOURCE MessageSource, LPCWSTR szMsg, int font)
{
    int CallingPrefixLen;
    if (IsCallingPrefix(szMsg, CallingPrefixLen))
    {
        HandlingCommand(MessageSource, szMsg + CallingPrefixLen);
    }
    return 0;
}

