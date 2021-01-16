/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     Basic function and tools for calling Genshin API
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#include <Windows.h>
#include <WinINet.h>
#include <atlstr.h>
#include "md5.h"
#include "GenshinAPIBase.h"

#pragma comment(lib, "wininet.lib")


WCHAR miHoYoServer[] = L"api-takumi.mihoyo.com";

HINTERNET hInternet = NULL, hConnect = NULL;

LPCWSTR ServerNameList[10] = { NULL, L"cn_gf01", NULL, NULL, NULL, L"cn_qd01", NULL, NULL, NULL, NULL };

BOOL InitializeGenshinAPI(BOOL bInitialize)
{
    if (bInitialize)
    {
        // Initiate WinINet
        hInternet = InternetOpenW(L"EmergencyFood", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, NULL);

        if (!hInternet)
        {
            return FALSE;
        }

        hConnect = InternetConnectW(hInternet,
            miHoYoServer, INTERNET_DEFAULT_HTTPS_PORT, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);

        if (!hConnect)
        {
            InternetCloseHandle(hInternet);
            hInternet = NULL;
            return FALSE;
        }

        return TRUE;
    }
    else
    {
        InternetCloseHandle(hConnect);
        hConnect = 0;
        InternetCloseHandle(hInternet);
        hInternet = 0;

        return TRUE;
    }
}

HINTERNET GetmiHoYoServerConnect()
{
    return hConnect;
}

const WCHAR *GetServerNameByUID(const WCHAR UID[])
{
    if (UID[0] >= L'0' && UID[0] <= L'9')
    {
        return ServerNameList[UID[0] - L'0'];
    }
}

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

BOOL GenshinAPISendRequest(HINTERNET hRequest, GENSHIN_API_RESULT &GenshinAPIResult, LPVOID lpOptional, DWORD dwOptionalLength)
{
    WCHAR cookie[] = L""; // fill your cookies here

    HttpAddRequestHeadersW(hRequest, cookie, -1,
        HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);

    GenshinAddAPIHttpHeader(hRequest);

    BOOL bSuccess = HttpSendRequestW(hRequest, NULL, NULL, lpOptional, dwOptionalLength);

    if (!bSuccess)
    {
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


    yyjson_doc *JsonDoc = yyjson_read((LPCSTR)Buffer.GetData(), AccumBytesRead, 0);

    if (!JsonDoc)
    {
        return FALSE;
    }

    yyjson_val *nodeRoot = yyjson_doc_get_root(JsonDoc);
    if (!nodeRoot)
    {
        // Failed to get root
        yyjson_doc_free(JsonDoc);
        return FALSE;
    }

    yyjson_val *nodeRetCode = yyjson_obj_get(nodeRoot, "retcode");
    if (!nodeRetCode)
    {
        // RetCode not found
        yyjson_doc_free(JsonDoc);
        return FALSE;
    }

    yyjson_val *nodeData = yyjson_obj_get(nodeRoot, "data");
    if (!nodeData)
    {
        // Data not found
        yyjson_doc_free(JsonDoc);
        return FALSE;
    }

    yyjson_val *nodeMessage = yyjson_obj_get(nodeRoot, "message");
    // ok if message not exist

    GenshinAPIResult.JsonDoc = JsonDoc; // will be released when GenshinAPIResult destructs
    GenshinAPIResult.RetCode = yyjson_get_int(nodeRetCode);
    GenshinAPIResult.nodeData = nodeData;

    if (nodeMessage)
    {
        int CChMessageLen = MultiByteToWideChar(CP_UTF8, 0, yyjson_get_str(nodeMessage), -1, 0, 0);
        MultiByteToWideChar(CP_UTF8, 0,
            yyjson_get_str(nodeMessage), -1,
            GenshinAPIResult.Message.GetBuffer(CChMessageLen), CChMessageLen);
        GenshinAPIResult.Message.ReleaseBuffer();
    }

    return TRUE;
}
