/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     Basic definition and function about GenshinImpact
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#pragma once

#include <Windows.h>
#include <atlstr.h>


// Elements definition

typedef enum __GENSHIN_ELEMENT
{
    ELEMENT_ERROR = -1,
    ELEMENT_NONE = 0,
    ELEMENT_ANEMO,
    ELEMENT_PYRO,
    ELEMENT_GEO,
    ELEMENT_ELECTRO,
    ELEMENT_CRYO,
    ELEMENT_HYDRO,
    ELEMENT_DENDRO
}GENSHIN_ELEMENT;

extern const char *GenshinElementList[];


// avatar definition
typedef struct __GENSHIN_AVATAR_INFO
{
    int AvatarID;
    const WCHAR *AvatarName_zhCN;
    GENSHIN_ELEMENT AvatarDefaultElement;
}GENSHIN_AVATAR_INFO;

extern const GENSHIN_AVATAR_INFO GenshinAvatarInfoList[];


// avatar data
typedef struct __GENSHIN_AVATAR_DATA
{
    int AvatarID;
    GENSHIN_ELEMENT Element;
    int Fetter;
    int Level;

    __GENSHIN_AVATAR_DATA()
    {
        AvatarID = 0;
        Element = ELEMENT_NONE;
        Fetter = 0;
        Level = 0;
    }

}GENSHIN_AVATAR_DATA;

typedef struct __GENSHIN_WEAPON_DATA
{
    ATL::CStringW Name;
    int Level;

}GENSHIN_WEAPON_DATA;

typedef struct __GENSHIN_RELIQUARY_DATA
{
    ATL::CStringW Name;
    int Pos;
    int Rarity;
    int Level;

}GENSHIN_RELIQUARY_DATA;

typedef struct __GENSHIN_AVATAR_DETAILED_DATA :
    public GENSHIN_AVATAR_DATA
{
    GENSHIN_WEAPON_DATA Weapon;
    GENSHIN_RELIQUARY_DATA Reliquaries[5];
    BOOL bReliquaryExists[5];

    __GENSHIN_AVATAR_DETAILED_DATA()
    {
        for (int i = 0; i < _countof(bReliquaryExists); i++)
        {
            bReliquaryExists[i] = FALSE;
        }
    }
}GENSHIN_AVATAR_DETAILED_DATA;

// functions
#ifdef __cplusplus
extern "C" GENSHIN_ELEMENT ElementTextToEnum(const char *ElementText);
extern "C" const WCHAR *GetAvatarNameByID(int ID);
extern "C" int GetAvatarIDByName(const WCHAR Name[]);
#else
GENSHIN_ELEMENT ElementTextToEnum(const char *ElementText);
const WCHAR *GetAvatarNameByID(int ID);
int GetAvatarIDByName(const WCHAR Name[]);
#endif
