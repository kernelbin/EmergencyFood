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
#include "GenshinBasic.h"
#include "GenshinAPIBase.h"
#include "md5.h"
#include "yyjson.h"

#pragma comment(lib, "wininet.lib")

WCHAR miHoYoServer[] = L"api-takumi.mihoyo.com";

BOOL AvatarJsonAnalysis(yyjson_val *nodeAvatars, GENSHIN_USER_GAME_RECORD_RESULT *Result)
{
    if (!nodeAvatars)
    {
        return FALSE;
    }

    int iAvatarCount = yyjson_arr_size(nodeAvatars);
    if (iAvatarCount > _countof(Result->AvatarData))
    {
        // something wrong. possibly array too small.
        return FALSE;
    }
    Result->AvatarCount = iAvatarCount;

    yyjson_val *AvatarEnum =  yyjson_arr_get_first(nodeAvatars);

    for (int iAvatarIndex = 0; iAvatarIndex < iAvatarCount; iAvatarIndex++)
    {
        yyjson_val *nodeID = yyjson_obj_get(AvatarEnum, "id");
        yyjson_val *nodeElement = yyjson_obj_get(AvatarEnum, "element");
        yyjson_val *nodeFetter = yyjson_obj_get(AvatarEnum, "fetter");
        yyjson_val *nodeLevel = yyjson_obj_get(AvatarEnum, "level");

        Result->AvatarData[iAvatarIndex].AvatarID = yyjson_get_int(nodeID);
        Result->AvatarData[iAvatarIndex].Element = ElementTextToEnum(yyjson_get_str(nodeElement));
        Result->AvatarData[iAvatarIndex].Fetter = yyjson_get_int(nodeFetter);
        Result->AvatarData[iAvatarIndex].Level = yyjson_get_int(nodeLevel);


        AvatarEnum = unsafe_yyjson_get_next(AvatarEnum);
    }

    return TRUE;
}

BOOL StatsJsonAnalysis(yyjson_val *nodeStats, GENSHIN_USER_GAME_RECORD_RESULT *Result)
{
    if (!nodeStats)
    {
        return FALSE;
    }

    yyjson_val *nodeActiveDayNumber = yyjson_obj_get(nodeStats, "active_day_number");
    yyjson_val *nodeAchievementNumber = yyjson_obj_get(nodeStats, "achievement_number");
    yyjson_val *nodeAnemoculusNumber = yyjson_obj_get(nodeStats, "anemoculus_number");
    yyjson_val *nodeGeoculusNumber = yyjson_obj_get(nodeStats, "geoculus_number");
    yyjson_val *nodeAvatarNumber = yyjson_obj_get(nodeStats, "avatar_number");
    yyjson_val *nodeWayPointNumber = yyjson_obj_get(nodeStats, "way_point_number");
    yyjson_val *nodeDomainNumber = yyjson_obj_get(nodeStats, "domain_number");
    yyjson_val *nodeSpiralAbyss = yyjson_obj_get(nodeStats, "spiral_abyss");
    yyjson_val *nodePreciousChestNumber = yyjson_obj_get(nodeStats, "precious_chest_number");
    yyjson_val *nodeLuxuriousChestNumber = yyjson_obj_get(nodeStats, "luxurious_chest_number");
    yyjson_val *nodeExquistieChestNumber = yyjson_obj_get(nodeStats, "exquisite_chest_number");
    yyjson_val *nodeCommonChestNumber = yyjson_obj_get(nodeStats, "common_chest_number");


    Result->StatsData.ActiveDayNumber = yyjson_get_int(nodeActiveDayNumber);
    Result->StatsData.AchievementNumber = yyjson_get_int(nodeAchievementNumber);
    Result->StatsData.AnemoculusNumber = yyjson_get_int(nodeAnemoculusNumber);
    Result->StatsData.GeoculusNumber = yyjson_get_int(nodeGeoculusNumber);
    Result->StatsData.AvatarNumber = yyjson_get_int(nodeAvatarNumber);
    Result->StatsData.WayPointNumber = yyjson_get_int(nodeWayPointNumber);
    Result->StatsData.DomainNumber = yyjson_get_int(nodeDomainNumber);
    MultiByteToWideChar(CP_UTF8, 0, yyjson_get_str(nodeSpiralAbyss), -1, Result->StatsData.SpiralAbyss, _countof(Result->StatsData.SpiralAbyss));
    Result->StatsData.PreciousChestNumber = yyjson_get_int(nodePreciousChestNumber);
    Result->StatsData.LuxuriousChestNumber = yyjson_get_int(nodeLuxuriousChestNumber);
    Result->StatsData.LuxuriousChestNumber = yyjson_get_int(nodeExquistieChestNumber);
    Result->StatsData.CommonChestNumber = yyjson_get_int(nodeCommonChestNumber);

    return TRUE;
}

BOOL UserGameRecordJsonAnalysis(LPCSTR lpJsonData, int JsonDataLength, GENSHIN_USER_GAME_RECORD_RESULT *Result)
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

    memset(Result, 0, sizeof(GENSHIN_USER_GAME_RECORD_RESULT));

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
        AvatarJsonAnalysis(nodeAvatars, Result);
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

extern "C" BOOL GenshinGetUserGameRecord(const WCHAR UID[], GENSHIN_USER_GAME_RECORD_RESULT * Result)
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

    
    WCHAR cookie[] = L""; // fill your cookies here
    
    HttpAddRequestHeadersW(hRequest, cookie, -1,
        HTTP_ADDREQ_FLAG_ADD | HTTP_ADDREQ_FLAG_REPLACE);
    
    GenshinAddAPIHttpHeader(hRequest);


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