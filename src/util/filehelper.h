#ifndef _FILEHELPER_H_
#define _FILEHELPER_H_

#include <string>

struct file_Helper
{
    file_Helper() = delete;
    static const std::string getFullPath(const char *file);
    static const std::string getAudioFullPath(const char *file);
    static const std::string getVideoFullPath(const char *file);
};

#endif