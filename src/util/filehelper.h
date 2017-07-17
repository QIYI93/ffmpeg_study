#ifndef _FILEHELPER_H_
#define _FILEHELPER_H_

#include <windows.h>

using std::string;
string getFullPath(string file)
{
    char path[MAX_PATH];
    GetModuleFileName(NULL, path, MAX_PATH);
    string fullPath(path);
    size_t index = fullPath.find("ffmpeg_study");
    fullPath.erase(fullPath.begin() + index + 13, fullPath.end());
    fullPath.append("resource\\");
    fullPath.append(file);
    return fullPath;
}

#endif