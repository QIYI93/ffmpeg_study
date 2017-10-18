#include "filehelper.h"

#include <windows.h>


using std::string;

const string file_Helper::getFullPath(const char *file)
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

const string file_Helper::getVideoFullPath(const char *file)
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

const string file_Helper::getAudioFullPath(const char *file)
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
