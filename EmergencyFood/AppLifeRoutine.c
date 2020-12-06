/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     Life routine of this app
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#include <Windows.h>
#include "CQAPITransfer.h"
#include "AppLifeRoutine.h"

//生命周期

int AppInitialize()//初始化时会被执行
{
    
    return 0;
}

int AppFinialize()//结束时会被执行
{
    return 0;
}

int AppEnabled()//启用时执行（如果初始化时是启用的，会在AppInitialize后执行一次）
{
    return 0;
}

int AppDisabled()//禁用时执行（如果结束时是启用的，会在AppFinialize前执行一次，这点和cq原生函数不一样）
{
    return 0;
}

int HandlePrivateMessage(int subType, int msgId, long long fromQQ, const WCHAR * msg, int font)
{
    if (lstrcmpW(msg, L"应急食品") == 0)
    {
        SendPrivateMessage(fromQQ, L"你再叫我应急食品，我就给你起很难听很难听的绰号！！");
    }
    return 0;
}


int HandleGroupMessage(int subType, int msgId, long long fromGroup, long long fromQQ, const char * fromAnonymous, const WCHAR * msg, int font)
{
    if (lstrcmpW(msg, L"应急食品") == 0)
    {
        SendGroupMessage(fromGroup, L"我才不是应急食品！！！");
    }
    return 0;
}
