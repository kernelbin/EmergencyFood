/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     Life routine of this app
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#include <Windows.h>
#include <wchar.h>
#include "CQAPITransfer.h"
#include "AppLifeRoutine.h"
#include "GenshinAPIGetGameRecord.h"
#include "GenshinTextualization.h"
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
        if (CallingPrefixLen == wcslen(szMsg))
        {
            SendBackMessage(MessageSource, L"有什么事吗？");
        }
        else
        {
            LPCWSTR szCommand = szMsg + CallingPrefixLen;

            if (wcsncmp(szCommand, L"查询", wcslen(L"查询")) == 0)
            {
                szCommand += wcslen(L"查询");
            }

            szCommand += GetSpacingCharLength(szCommand);

            if (wcscmp(szCommand, L"帮助") == 0)
            {
                SendBackMessage(MessageSource, L"\
输入「派蒙查询UID」就可以查到对应UID的信息哦，比如「派蒙查询130741966」\
\r\n输入「派蒙查询UID统计」或者「派蒙查询UID探索」，也可以查询到对应 UID 的统计或者探索信息呢\r\n\
其他功能尚在开发，「前面的区域，以后再来探索吧」");
            }
            else
            {
                long long UID;
                if (swscanf_s(szCommand, L"%lld", &UID) == 1)
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
                        BOOL bSuccess = GenshinAPIGetUserGameRecord(szUID, GameRecordResult);

                        if (bSuccess)
                        {
                            switch (GameRecordResult.RetCode)
                            {
                            case 0:
                            {
                                if (wcsstr(szCommand, L"统计"))
                                {
                                    ATL::CStringW String;
                                    GetTextualizedStatsInfo(GameRecordResult.StatsData, String);
                                    SendBackMessage(MessageSource, String);
                                }
                                else if (wcsstr(szCommand, L"探索"))
                                {
                                    ATL::CStringW String;
                                    GetTextualizedExplorationInfo(GameRecordResult.ExploationData, String);
                                    SendBackMessage(MessageSource, String);
                                }
                                else
                                {
                                    ATL::CStringW String;
                                    GetTextualizedAvatarInfo(GameRecordResult.AvatarData, String);
                                    SendBackMessage(MessageSource, String);
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
                else
                {
                    SendBackMessage(MessageSource, L"额，派蒙听不懂你在说什么....");
                }
            }
        }
    }

    
    
    return 0;
}

