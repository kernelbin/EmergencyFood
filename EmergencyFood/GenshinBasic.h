/*
 * PROJECT:     EmergencyFood Project
 * PURPOSE:     Basic definition and function about GenshinImpact
 * COPYRIGHT:   Copyright 2020 kernel.bin (1160386205@qq.com)
 */


#pragma once

#include <Windows.h>

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
    WCHAR *AvatarName_zhCN;
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


// functions
#ifdef __cplusplus
extern "C" GENSHIN_ELEMENT ElementTextToEnum(const char *ElementText);
extern "C" const WCHAR *GetAvatarNameByID(int ID);
#else
GENSHIN_ELEMENT ElementTextToEnum(const char *ElementText);
const WCHAR *GetAvatarNameByID(int ID);
#endif
