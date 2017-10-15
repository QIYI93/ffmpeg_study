#ifndef XFFMPEG_H
#define XFFMPEG_H

#include <QString>
#include <QMutex>
#include <QMutexLocker>

extern "C"
{
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

class XFFmpeg
{

public:
    static XFFmpeg* get();
    ~XFFmpeg();
    bool openFile(const char*);
    void closeFile();
    QString getError();
    int getTotalMS() { return m_totalMS; }

protected:
    AVFormatContext *m_pFormatCtx = nullptr;
    QMutex mutex;

private:
    XFFmpeg();

    static XFFmpeg *s_xFFMpeg;
    char m_errMsgBuffer[2048];
    int m_totalMS = 0;
};




#endif
