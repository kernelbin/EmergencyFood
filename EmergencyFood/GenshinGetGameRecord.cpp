/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     API for getting user game record
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#include <Windows.h>
#include <WinInet.h>
#include <atlstr.h>
#include <atlcoll.h>
#include <cstdlib>
#include "GenshinGetGameRecord.h"
#include "md5.h"
#include "yyjson.h"

#pragma comment(lib, "wininet.lib")

WCHAR miHoYoServer[] = L"api-takumi.mihoyo.com";

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

BOOL UserGameRecordJsonAnalysis(LPCSTR lpJsonData, int JsonDataLength, USER_GAME_RECORD_RESULT * Result)
{
    yyjson_doc *nodeJsonDoc = yyjson_read(lpJsonData, JsonDataLength, 0);
    if (!nodeJsonDoc)
    {
        return FALSE;
    }

    yyjson_val *nodeRoot = yyjson_doc_get_root(nodeJsonDoc);
    if (!nodeRoot)
    {
        // Failed to get root
        yyjson_doc_free(nodeJsonDoc);
        return FALSE;
    }

    yyjson_val *nodeRetCode = yyjson_obj_get(nodeRoot, "retcode");
    if (!nodeRetCode)
    {
        // RetCode not found
        yyjson_doc_free(nodeJsonDoc);
        return FALSE;
    }

    yyjson_val *nodeData = yyjson_obj_get(nodeRoot, "data");
    if (!nodeData)
    {
        // Data not found
        yyjson_doc_free(nodeJsonDoc);
        return FALSE;
    }

    memset(Result, 0, sizeof(USER_GAME_RECORD_RESULT));

    Result->RetCode = yyjson_get_int(nodeRetCode);
    if (Result->RetCode != 0)
    {
        yyjson_doc_free(nodeJsonDoc);
        return TRUE;
    }

    yyjson_val *nodeAvatars = yyjson_obj_get(nodeData, "avatars");
    yyjson_val *nodeStats = yyjson_obj_get(nodeData, "stats");
    yyjson_val *nodeCityExplorations = yyjson_obj_get(nodeData, "city_explorations");

    if (nodeAvatars)
    {
        // TODO: analysis
    }

    if (nodeStats)
    {
        // TODO: analysis
    }

    if (nodeCityExplorations)
    {
        // TODO: analysis
    }

    yyjson_doc_free(nodeJsonDoc);
    return TRUE;
}

extern "C" BOOL GenshinGetUserGameRecord(const WCHAR UID[], USER_GAME_RECORD_RESULT * Result)
{
    if (!UID)
    {
        return FALSE;
    }

    // construct request URL first
    ATL::CStringW RequestURL;
    if (lstrlenW(UID) != 9)
    {
        // Wrong UID
        return FALSE;
    }

    switch (UID[0])
    {
    case L'1':
        // China, official server
        RequestURL.Format(L"/game_record/genshin/api/index?server=%ls&role_id=%ls", L"cn_gf01", UID);
        break;
    case L'5':
        // China, bilibili server
        RequestURL.Format(L"/game_record/genshin/api/index?server=%ls&role_id=%ls", L"cn_qd01", UID);
        break;
    default:
        // server not supported, or wrong UID
        return FALSE;
    }

    // Initiate a request
    // TODO: reuse hInternet and hConnect instead of opening one each time.
    HINTERNET hInternet = InternetOpenW(L"EmergencyFood", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);

    if (!hInternet)
    {
        return FALSE;
    }
    
    HINTERNET hConnect = InternetConnectW(hInternet,
        miHoYoServer, INTERNET_DEFAULT_HTTP_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);

    if (!hConnect)
    {
        InternetCloseHandle(hInternet);
        return FALSE;
    }
    
    const WCHAR *rgpszAcceptTypes[] = { L"*/*", NULL };

    HINTERNET hRequest = HttpOpenRequestW(hConnect, L"GET", RequestURL,
        NULL, NULL, rgpszAcceptTypes, INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_COOKIES, NULL);

    if (!hRequest)
    {
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return FALSE;
    }

    ATL::CStringW DS;
    DSGet(DS);
    WCHAR cookie[] = L""; // fill your cookies here

    HttpAddRequestHeadersW(hRequest, DS.GetString(), -1,
        HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
    HttpAddRequestHeadersW(hRequest, cookie, -1,
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
    
    BOOL bSuccess = HttpSendRequestW(hRequest, NULL, NULL, NULL, NULL);

    if (!bSuccess)
    {
        InternetCloseHandle(hRequest);
        InternetCloseHandle(hConnect);
        InternetCloseHandle(hInternet);
        return FALSE;
    }

    ATL::CAtlArray<BYTE> Buffer;
    DWORD AccumBytesRead = 0;
    for (;;)
    {
        DWORD BytesRead = 0;
        Buffer.SetCount(AccumBytesRead + 1024);
        BOOL bSuccess = InternetReadFile(hRequest, Buffer.GetData() + AccumBytesRead, 1024, &BytesRead);

        if (!bSuccess)
        {
            InternetCloseHandle(hRequest);
            InternetCloseHandle(hConnect);
            InternetCloseHandle(hInternet);
            return FALSE;
        }

        if (BytesRead == 0)
        {
            break;
        }
        else
        {
            AccumBytesRead += BytesRead;
        }
    }
    
    // ensure zero terminated.
    Buffer.SetCount(AccumBytesRead + 1);
    Buffer[AccumBytesRead] = 0;

    // analysis json text
    BOOL bAnalysisResult = UserGameRecordJsonAnalysis((LPCSTR)Buffer.GetData(), AccumBytesRead, Result);

    InternetCloseHandle(hRequest);
    InternetCloseHandle(hConnect);
    InternetCloseHandle(hInternet);

    return bAnalysisResult;
}