#ifndef _FILEHELPER_H_
#define _FILEHELPER_H_

#include <windows.h>

std::string getFullPath(const char* file)
{
    char path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    (strrchr(path, '\\'))[1] = '\0';
    lstrcat(path, file);
    return std::string(path);
}



#endif