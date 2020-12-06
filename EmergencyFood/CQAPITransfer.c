/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     Translate CoolQ API call
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "CoolQDef.h"
#include "CoolQ_State.h"
#include "CQAPITransfer.h"
#include "EncodeConvert.h"



int SendPrivateMessage(long long QQID, const WCHAR * msg)
{
    int iRet;

    char* lpMultiByte = StrConvWC2MB(CP_GB18030, msg, -1, NULL);
    iRet = CQ_sendPrivateMsg(CQAuthCode, QQID, lpMultiByte);
    free(lpMultiByte);

    return iRet;
}

int SendGroupMessage(long long GroupID, const WCHAR * msg)
{
    int iRet;

    char *lpMultiByte = StrConvWC2MB(CP_GB18030, msg, -1, NULL);
    return CQ_sendGroupMsg(CQAuthCode, GroupID, lpMultiByte);
    free(lpMultiByte);

    return iRet;
}

const char * GetGroupMemberInfo(long long GroupID, long long QQID, BOOL NoCache)
{
    return CQ_getGroupMemberInfoV2(CQAuthCode, GroupID, QQID, NoCache);
}

const char* GetStrangerInfo(long long QQID, BOOL NoCache)
{
    return CQ_getStrangerInfo(CQAuthCode, QQID, NoCache);
}

const char* GetAppDirectory()
{
    return CQ_getAppDirectory(CQAuthCode);
}