/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     Basic function and tools for calling Genshin API
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#pragma once

#include <Windows.h>
#include <WinINet.h>
#include <atlcoll.h>
#include <atlstr.h>
#include "yyjson.h"


class GENSHIN_API_RESULT
{
private:
    yyjson_doc *JsonDoc;

public:
    GENSHIN_API_RESULT()
    {
        JsonDoc = NULL;
        nodeData = NULL;
        RetCode = -1;
    }

    ~GENSHIN_API_RESULT()
    {
        if (JsonDoc)
        {
            yyjson_doc_free(JsonDoc);
        }
    }

    // information retrieved from json
    int RetCode;
    ATL::CStringW Message;
    yyjson_val *nodeData;
    
    friend BOOL GenshinAPISendRequest(HINTERNET hRequest, GENSHIN_API_RESULT &GenshinAPIResult, LPVOID lpOptional, DWORD dwOptionalLength);
};

BOOL InitializeGenshinAPI(BOOL bInitialize);

HINTERNET GetmiHoYoServerConnect();

const WCHAR *GetServerNameByUID(const WCHAR UID[]);

BOOL GenshinAPISendRequest(HINTERNET hRequest, GENSHIN_API_RESULT &GenshinAPIResult, LPVOID lpOptional, DWORD dwOptionalLength);
