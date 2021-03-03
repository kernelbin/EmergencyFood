/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     Functions about handling interaction
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#include <Windows.h>
#include <wchar.h>
#include "GenshinAPIGetGameRecord.h"
#include "GenshinAPICharacter.h"
#include "CQAPITransfer.h"
#include "GenshinTextualization.h"

int GetSpacingCharLength(LPCWSTR szMsg)
{
    const WCHAR *SpacingCharList[] = { L" ", L"，", L"。", L"\n", L"\r" };

    LPCWSTR szCurrentPlace = szMsg;
    for (int i = 0; i < _countof(SpacingCharList); i++)
    {
        if (wcsncmp(szCurrentPlace, SpacingCharList[i], wcslen(SpacingCharList[i])) == 0)
        {
            szCurrentPlace += wcslen(SpacingCharList[i]);
            i = 0;
            continue;
        }
    }
    return szCurrentPlace - szMsg;
}

BOOL IsCallingPrefix(LPCWSTR szMsg, int &PrefixLenght)
{
    const WCHAR *CallingName[] = { L"应急食品", L"派蒙", L"Paimon" };

    for (int i = 0; i < _countof(CallingName); i++)
    {
        if (_wcsnicmp(szMsg, CallingName[i], wcslen(CallingName[i])) == 0)
        {
            PrefixLenght = wcslen(CallingName[i]) +
                GetSpacingCharLength(szMsg + wcslen(CallingName[i]));
            return TRUE;
        }
    }

    return FALSE;
}

int HandleGenshinAPIErrorCode(MESSAGE_SOURCE MessageSource, int RetCode, LPCWSTR Message)
{
    switch (RetCode)
    {
    case 10101:
    {
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
        ATL::CStringW FmtMessage;
        FmtMessage.Format(L"好像哪里出了什么错呢！派蒙也不知道出了什么问题... 我找到了这个，上面写着「retcode:%d  %ls」旅行者，你见多识广，看得懂吗？", RetCode, Message);
        SendBackMessage(MessageSource, FmtMessage);
        break;
    }
    }
    return TRUE;
}

int HandlingCommand(MESSAGE_SOURCE MessageSource, LPCWSTR szCommand)
{
    if (szCommand[0] == L'\0')
    {
        SendBackMessage(MessageSource, L"有什么事吗？");
    }

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
                // UID length is 9
                szCommand += 9;
                szCommand += GetSpacingCharLength(szCommand);

                if (szCommand[0] == L'\0')
                {
                    WCHAR szUID[10];
                    swprintf(szUID, _countof(szUID), L"%lld", UID);
                    GENSHIN_USER_GAME_RECORD_RESULT GameRecordResult;
                    BOOL bSuccess = GenshinAPIGetUserGameRecord(szUID, GameRecordResult);

                    if (bSuccess)
                    {
                        if (GameRecordResult.RetCode != 0)
                        {
                            HandleGenshinAPIErrorCode(MessageSource, GameRecordResult.RetCode, GameRecordResult.Message);
                        }
                        else
                        {
                            // ok
                            ATL::CStringW String;
                            GetTextualizedAvatarInfo(GameRecordResult.AvatarData, String);
                            SendBackMessage(MessageSource, String);
                        }
                    }
                    else
                    {
                        SendBackMessage(MessageSource, L"查询失败了！额，派蒙也不知道为什么（思考）");
                    }
                }
                else if (wcscmp(szCommand, L"统计") == 0)
                {
                    WCHAR szUID[10];
                    swprintf(szUID, _countof(szUID), L"%lld", UID);
                    GENSHIN_USER_GAME_RECORD_RESULT GameRecordResult;
                    BOOL bSuccess = GenshinAPIGetUserGameRecord(szUID, GameRecordResult);

                    if (bSuccess)
                    {
                        if (GameRecordResult.RetCode != 0)
                        {
                            HandleGenshinAPIErrorCode(MessageSource, GameRecordResult.RetCode, GameRecordResult.Message);
                        }
                        else
                        {
                            // ok
                            ATL::CStringW String;
                            GetTextualizedStatsInfo(GameRecordResult.StatsData, String);
                            SendBackMessage(MessageSource, String);
                        }
                    }
                    else
                    {
                        SendBackMessage(MessageSource, L"查询失败了！额，派蒙也不知道为什么（思考）");
                    }
                }
                else if (wcscmp(szCommand, L"探索") == 0)
                {
                    WCHAR szUID[10];
                    swprintf(szUID, _countof(szUID), L"%lld", UID);
                    GENSHIN_USER_GAME_RECORD_RESULT GameRecordResult;
                    BOOL bSuccess = GenshinAPIGetUserGameRecord(szUID, GameRecordResult);

                    if (bSuccess)
                    {
                        if (GameRecordResult.RetCode != 0)
                        {
                            HandleGenshinAPIErrorCode(MessageSource, GameRecordResult.RetCode, GameRecordResult.Message);
                        }
                        else
                        {
                            // ok
                            ATL::CStringW String;
                            GetTextualizedExplorationInfo(GameRecordResult.ExploationData, String);
                            SendBackMessage(MessageSource, String);
                        }
                    }
                    else
                    {
                        SendBackMessage(MessageSource, L"查询失败了！额，派蒙也不知道为什么（思考）");
                    }
                }
                else
                {
                    int AvatarID;
                    if ((AvatarID = GetAvatarIDByName(szCommand)) != 0)
                    {
                        WCHAR szUID[10];
                        swprintf(szUID, _countof(szUID), L"%lld", UID);
                        GENSHIN_CHARACTER_RESULT CharacterResult;
                        ATL::CAtlArray<int> QueryCharacterID;
                        QueryCharacterID.Add(AvatarID);
                        BOOL bSuccess = GenshinAPICharacter(szUID, QueryCharacterID, CharacterResult);

                        if (bSuccess)
                        {
                            if (CharacterResult.RetCode != 0)
                            {
                                HandleGenshinAPIErrorCode(MessageSource, CharacterResult.RetCode, CharacterResult.Message);
                            }
                            else
                            {
                                // ok
                                ATL::CStringW String;
                                GetTextualizedAvatarDetailedInfo(CharacterResult.AvatarData, String);
                                SendBackMessage(MessageSource, String);
                            }
                        }
                        else
                        {
                            SendBackMessage(MessageSource, L"查询失败了！额，派蒙也不知道为什么（思考）");
                        }
                    }
                    else
                    {
                        SendBackMessage(MessageSource, L"额，派蒙听不懂你在说什么....");
                    }
                }
            }
        }
        else
        {
            SendBackMessage(MessageSource, L"额，派蒙听不懂你在说什么....");
        }
    }
    return 0;
}