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
    virtual ~XFFmpeg();

    bool openFile(const char*);
    AVPacket* readFrame(); //Outside release
    AVFrame* decoder(const AVPacket*);
    void closeFile();
    QString getError();
    int getTotalMS() { return m_totalMS; }

protected:
    AVFormatContext *m_pFormatCtx = nullptr;
    QMutex m_mutex;
    AVFrame *m_AVFrameYUV = nullptr;
    int m_videoIndex = -1;
    AVCodec *m_videoCodec = nullptr;
    AVCodecContext *m_videoCodecCtx = nullptr;

private:
    XFFmpeg();

    static XFFmpeg *s_xFFMpeg;
    char m_errMsgBuffer[2048];
    int m_totalMS = 0;
};




#endif
