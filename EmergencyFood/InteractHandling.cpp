/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     Functions about handling interaction
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#include <Windows.h>


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

