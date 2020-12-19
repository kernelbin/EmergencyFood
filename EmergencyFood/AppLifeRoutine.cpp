/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     Life routine of this app
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#include <Windows.h>
#include <wchar.h>
#include "CQAPITransfer.h"
#include "AppLifeRoutine.h"
#include "GenshinGetGameRecord.h"
#include "GenshinTextualization.h"

//生命周期

extern "C" int AppInitialize()//初始化时会被执行
{
    return 0;
}

extern "C" int AppFinialize()//结束时会被执行
{
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
    if (lstrcmpW(szMsg, L"应急食品") == 0)
    {
        SendBackMessage(MessageSource, L"我！才！不！是！应！急！食！品！！");
    }
    else
    {
        long long UID;
        if (swscanf_s(szMsg, L"原神查询%lld", &UID) == 1)
        {
            if (UID >= 1000000000 || UID < 100000000)
            {
                SendBackMessage(MessageSource, L"UID长度错误，请检查输入的UID是否正确");
            }
            else
            {
                WCHAR szUID[10];
                swprintf(szUID, _countof(szUID), L"%lld", UID);
                GENSHIN_USER_GAME_RECORD_RESULT GameRecordResult;
                BOOL bSuccess = GenshinGetUserGameRecord(szUID, &GameRecordResult);

                if (bSuccess)
                {
                    switch (GameRecordResult.RetCode)
                    {
                    case 0:
                    {
                        if (wcsstr(szMsg, L"统计"))
                        {
                            WCHAR *String = GetTextualizedStatsInfo(&(GameRecordResult.StatsData));
                            SendBackMessage(MessageSource, String);
                            ReleaseTextualizedStatsInfo(String);
                        }
                        else
                        {
                            WCHAR *String = GetTextualizedAvatarInfo(GameRecordResult.AvatarData, GameRecordResult.AvatarCount);
                            SendBackMessage(MessageSource, String);
                            ReleaseTextualizedAvatarInfo(String);
                        }
                        
                        break;
                    }
                    case 10101:
                    {
                        // TODO: change another cookie and retry
                        SendBackMessage(MessageSource, L"派蒙...派蒙饿了！cookies被吃光了！");
                        break;
                    }
                    case 10102:
                    {
                        SendBackMessage(MessageSource, L"派蒙什么也没有查到！在米游社里打开原神数据公开，派蒙才能查呢！");
                        break;
                    }
                    default:
                    {
                        SendBackMessage(MessageSource, L"好像哪里出了什么错呢！派蒙也不知道出了什么问题！");
                        break;
                    }
                    }
                    
                }
                else
                {
                    SendBackMessage(MessageSource, L"查询失败了！额，派蒙也不知道为什么（思考）");
                }
            }
        }
    }
    
    return 0;
}

