#ifndef _FILEHELPER_H_
#define _FILEHELPER_H_

#include <windows.h>
#include <string>

namespace file_Helper
{
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

    string getVideoFullPath(string file)
    {
        char path[MAX_PATH];
        GetModuleFileName(NULL, path, MAX_PATH);
        string fullPath(path);
        size_t index = fullPath.find("ffmpeg_study");
        fullPath.erase(fullPath.begin() + index + 13, fullPath.end());
        fullPath.append("resource\\video\\");
        fullPath.append(file);
        return fullPath;
    }

    string getAudioFullPath(string file)
    {
        char path[MAX_PATH];
        GetModuleFileName(NULL, path, MAX_PATH);
        string fullPath(path);
        size_t index = fullPath.find("ffmpeg_study");
        fullPath.erase(fullPath.begin() + index + 13, fullPath.end());
        fullPath.append("resource\\audio\\");
        fullPath.append(file);
        return fullPath;
    }
};
#endif