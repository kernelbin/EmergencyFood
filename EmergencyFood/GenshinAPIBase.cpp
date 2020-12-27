/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     Basic function and tools for calling Genshin API
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#include <Windows.h>
#include <WinINet.h>
#include <atlstr.h>
#include "md5.h"

long long GetPresentUnixTime()
{
    SYSTEMTIME st;
    FILETIME ft;
    LARGE_INTEGER li;

    GetSystemTime(&st);
    SystemTimeToFileTime(&st, &ft);
    li.LowPart = ft.dwLowDateTime;
    li.HighPart = ft.dwHighDateTime;
    li.QuadPart /= 10000000;
    li.QuadPart -= 11644473600LL;
    return li.QuadPart;
}

BOOL DSGet(ATL::CStringW &DS)
{
    /*
     * this algorithm comes from:
     * https://github.com/Womsxd/YuanShen_User_Info
     */

     // TODO: support choosing different version of miHoYo client
     // and randomly generate r (instead of using aaaaaa)

    ATL::CStringA Temp;
    long long i = GetPresentUnixTime();
    Temp.Format("salt=%ls&t=%lld&r=%ls", L"cx2y9z9a29tfqvr1qsq6c7yz99b5jsqt", i, L"aaaaaa");

    uint8_t MD5Result[16];
    md5((const uint8_t *)Temp.GetString(), Temp.GetLength(), MD5Result);

    DS.Format(L"DS: %lld,%ls,", i, L"aaaaaa");

    for (int i = 0; i < 16; i++)
    {
        DS.Format(L"%s%x", DS.GetString(), MD5Result[i] >> 4);
        DS.Format(L"%s%x", DS.GetString(), MD5Result[i] & 0xf);
    }

    DS.Format(L"%s\r\n", DS.GetString());
    return TRUE;
}

BOOL GenshinAddAPIHttpHeader(HINTERNET hRequest)
{
    ATL::CStringW DS;
    DSGet(DS);

    HttpAddRequestHeadersW(hRequest, DS.GetString(), -1,
        HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
    HttpAddRequestHeadersW(hRequest, L"Origin: https://webstatic.mihoyo.com", -1,
        HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
    HttpAddRequestHeadersW(hRequest, L"x-rpc-app_version: 2.2.1\r\n", -1,
        HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
    HttpAddRequestHeadersW(hRequest, L"x-rpc-client_type: 4\r\n", -1,
        HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
    HttpAddRequestHeadersW(hRequest, L"Referer: https://webstatic.mihoyo.com/app/community-game-records/index.html?v=6\r\n", -1,
        HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
    HttpAddRequestHeadersW(hRequest, L"X-Requested-With: com.mihoyo.hyperion\r\n", -1,
        HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);

    // TODO: check for error
    return TRUE;
}

