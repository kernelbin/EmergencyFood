/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     Basic definition and function about GenshinImpact
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#include <Windows.h>
#include "GenshinBasic.h"

const char *GenshinElementList[] = { "None", "Anemo", "Pyro", "Geo", "Electro", "Cryo", "Hydro", "Dendro" };

const GENSHIN_AVATAR_INFO GenshinAvatarInfoList[] =
{
    {10000003, L"琴", ELEMENT_ANEMO},
    {10000005, L"旅行者（空）", ELEMENT_NONE},
    {10000006, L"丽莎", ELEMENT_ELECTRO},
    {10000007, L"旅行者（荧）", ELEMENT_NONE},
    {10000014, L"芭芭拉", ELEMENT_HYDRO},
    {10000015, L"凯亚", ELEMENT_CRYO},
    {10000016, L"迪卢克", ELEMENT_PYRO},
    {10000020, L"雷泽", ELEMENT_ELECTRO},
    {10000021, L"安柏", ELEMENT_PYRO},
    {10000022, L"温迪", ELEMENT_ANEMO},
    {10000023, L"香菱", ELEMENT_PYRO},
    {10000024, L"北斗", ELEMENT_ELECTRO},
    {10000025, L"行秋", ELEMENT_HYDRO},
    {10000027, L"凝光", ELEMENT_GEO},
    {10000029, L"可莉", ELEMENT_PYRO},
    {10000030, L"钟离", ELEMENT_GEO},
    {10000031, L"菲谢尔", ELEMENT_ELECTRO},
    {10000032, L"班尼特", ELEMENT_PYRO},
    {10000033, L"达达利亚", ELEMENT_HYDRO},
    {10000034, L"诺艾尔", ELEMENT_GEO},
    {10000036, L"重云", ELEMENT_CRYO},
    {10000039, L"迪奥娜", ELEMENT_CRYO},
    {10000041, L"莫娜", ELEMENT_HYDRO},
    {10000042, L"刻晴", ELEMENT_ELECTRO},
    {10000043, L"砂糖", ELEMENT_ANEMO},
    {10000044, L"辛焱", ELEMENT_PYRO}
};


GENSHIN_ELEMENT ElementTextToEnum(const char *ElementText)
{
    for (int i = 0; i < _countof(GenshinElementList); i++)
    {
        if (strcmp(GenshinElementList[i], ElementText) == 0)
        {
            return i;
        }
    }
    return ELEMENT_ERROR;
}

const WCHAR *GetAvatarNameByID(int ID)
{
    // TODO: Internationalization

    for (int i = 0; i < _countof(GenshinAvatarInfoList); i++)
    {
        if (GenshinAvatarInfoList[i].AvatarID == ID)
        {
            return GenshinAvatarInfoList[i].AvatarName_zhCN;
        }
    }
    return NULL;
}