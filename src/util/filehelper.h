#ifndef _FILEHELPER_H_
#define _FILEHELPER_H_

#include <windows.h>

std::string getFullPath(std::string file)
{
    char path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    std::string fullPath(path);
    size_t index = fullPath.find("ffmpeg_study");
    fullPath.erase(fullPath.begin() + index + 13, fullPath.end());
    fullPath.append("resource\\");
    fullPath.append(file);
    return std::string(fullPath);
}



#endif