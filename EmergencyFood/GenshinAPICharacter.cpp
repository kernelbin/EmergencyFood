/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     API for getting character information
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#include <Windows.h>
#include <atlcoll.h>
#include "GenshinAPIBase.h"
#include "GenshinAPICharacter.h"
#include "EncodeConvert.h"

BOOL ReliquaryJsonAnalysis(yyjson_val *nodeReliquary, GENSHIN_RELIQUARY_DATA &ReliquaryData)
{
    if (!nodeReliquary)
    {
        return FALSE;
    }

    yyjson_val *nodeName = yyjson_obj_get(nodeReliquary, "name");
    yyjson_val *nodePos = yyjson_obj_get(nodeReliquary, "pos");
    yyjson_val *nodeRarity = yyjson_obj_get(nodeReliquary, "rarity");
    yyjson_val *nodeLevel = yyjson_obj_get(nodeReliquary, "level");

    int cchNameLen = MultiByteToWideChar(CP_UTF8, 0, yyjson_get_str(nodeName), -1, 0, 0);
    MultiByteToWideChar(CP_UTF8, 0, yyjson_get_str(nodeName), -1, ReliquaryData.Name.GetBuffer(cchNameLen), cchNameLen);
    ReliquaryData.Name.ReleaseBuffer();

    ReliquaryData.Pos = yyjson_get_int(nodePos) - 1;
    ReliquaryData.Rarity = yyjson_get_int(nodeRarity);
    ReliquaryData.Level = yyjson_get_int(nodeLevel);

    return TRUE;
}

BOOL WeaponJsonAnalysis(yyjson_val *nodeWeapon, GENSHIN_WEAPON_DATA &WeaponData)
{
    if (!nodeWeapon)
    {
        return FALSE;
    }

    yyjson_val *nodeName = yyjson_obj_get(nodeWeapon, "name");
    yyjson_val *nodeLevel = yyjson_obj_get(nodeWeapon, "level");

    int cchNameLen = MultiByteToWideChar(CP_UTF8, 0, yyjson_get_str(nodeName), -1, 0, 0);
    MultiByteToWideChar(CP_UTF8, 0, yyjson_get_str(nodeName), -1, WeaponData.Name.GetBuffer(cchNameLen), cchNameLen);
    WeaponData.Name.ReleaseBuffer();

    WeaponData.Level = yyjson_get_int(nodeLevel);

    return TRUE;
}
BOOL AvatarsDetaildJsonAnalysis(yyjson_val *nodeAvatars, ATL::CAtlArray<GENSHIN_AVATAR_DETAILED_DATA> &AvatarData)
{
    if (!nodeAvatars)
    {
        return FALSE;
    }

    int iAvatarCount = yyjson_arr_size(nodeAvatars);

    yyjson_val *AvatarEnum = yyjson_arr_get_first(nodeAvatars);

    AvatarData.SetCount(iAvatarCount);

    for (int iAvatarIndex = 0; iAvatarIndex < iAvatarCount; iAvatarIndex++)
    {
        yyjson_val *nodeID = yyjson_obj_get(AvatarEnum, "id");
        yyjson_val *nodeElement = yyjson_obj_get(AvatarEnum, "element");
        yyjson_val *nodeFetter = yyjson_obj_get(AvatarEnum, "fetter");
        yyjson_val *nodeLevel = yyjson_obj_get(AvatarEnum, "level");
        yyjson_val *nodeWeapon = yyjson_obj_get(AvatarEnum, "weapon");
        yyjson_val *nodeReliquaries = yyjson_obj_get(AvatarEnum, "reliquaries");

        AvatarData[iAvatarIndex].AvatarID = yyjson_get_int(nodeID);
        AvatarData[iAvatarIndex].Element = ElementTextToEnum(yyjson_get_str(nodeElement));
        AvatarData[iAvatarIndex].Fetter = yyjson_get_int(nodeFetter);
        AvatarData[iAvatarIndex].Level = yyjson_get_int(nodeLevel);
        WeaponJsonAnalysis(nodeWeapon, AvatarData[iAvatarIndex].Weapon);
        {
            int iReliquariesCount = yyjson_arr_size(nodeReliquaries);
            yyjson_val *ReliquaryEnum = yyjson_arr_get_first(nodeReliquaries);
            for (int iReliquariesIndex = 0; iReliquariesIndex < iReliquariesCount; iReliquariesIndex++)
            {
                GENSHIN_RELIQUARY_DATA ReliquaryData;
                ReliquaryJsonAnalysis(ReliquaryEnum, ReliquaryData);
                AvatarData[iAvatarIndex].Reliquaries[ReliquaryData.Pos] = ReliquaryData;
                AvatarData[iAvatarIndex].bReliquaryExists[ReliquaryData.Pos] = TRUE;
                ReliquaryEnum = unsafe_yyjson_get_next(ReliquaryEnum);
            }
        }
        
        AvatarEnum = unsafe_yyjson_get_next(AvatarEnum);
    }

    return TRUE;
}
BOOL CharacterJsonAnalysis(GENSHIN_CHARACTER_RESULT &Result)
{
    yyjson_val *nodeData = Result.nodeData;
    if (!nodeData)
    {
        return FALSE;
    }

    yyjson_val *nodeAvatars = yyjson_obj_get(nodeData, "avatars");
    
    if (nodeAvatars)
    {
        AvatarsDetaildJsonAnalysis(nodeAvatars, Result.AvatarData);
    }
    
    return TRUE;
}

extern "C" BOOL GenshinAPICharacter(const WCHAR UID[], ATL::CAtlArray<int> &AvatarID, GENSHIN_CHARACTER_RESULT &Result)
{
    if (!UID)
    {
        return FALSE;
    }

    const WCHAR *rgpszAcceptTypes[] = { L"*/*", NULL };

    const WCHAR *RequestURL = L"/game_record/genshin/api/character";

    HINTERNET hRequest = HttpOpenRequestW(GetmiHoYoServerConnect(), L"POST", RequestURL,
        NULL, NULL, rgpszAcceptTypes, INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_COOKIES | INTERNET_FLAG_SECURE, 0);

    if (!hRequest)
    {
        return FALSE;
    }

    // generate json string
    yyjson_mut_doc *JsonStringDoc = yyjson_mut_doc_new(NULL);
    yyjson_mut_val *DocRoot = yyjson_mut_obj(JsonStringDoc);
    yyjson_mut_doc_set_root(JsonStringDoc, DocRoot);

    yyjson_mut_val *CharacterIDs = yyjson_mut_arr_with_sint32(JsonStringDoc, AvatarID.GetData(), AvatarID.GetCount());
    yyjson_mut_obj_add_val(JsonStringDoc, DocRoot, "character_ids", CharacterIDs);


    LPSTR StrUID = StrConvWC2MB(CP_UTF8, UID, -1, NULL);
    LPSTR StrServerName = StrConvWC2MB(CP_UTF8, GetServerNameByUID(UID), -1, NULL);
    yyjson_mut_obj_add_str(JsonStringDoc, DocRoot, "role_id", StrUID);
    yyjson_mut_obj_add_str(JsonStringDoc, DocRoot, "server", StrServerName);

    const char *JsonStr = yyjson_mut_write(JsonStringDoc, 0, NULL);

    free(StrUID);
    free(StrServerName);

    if (!GenshinAPISendRequest(hRequest, Result, (LPVOID)JsonStr, strlen(JsonStr)))
    {
        InternetCloseHandle(hRequest);
        return FALSE;
    }

    free((void *)JsonStr);

    BOOL bAnalysisResult = CharacterJsonAnalysis(Result);

    InternetCloseHandle(hRequest);

    return bAnalysisResult;

}