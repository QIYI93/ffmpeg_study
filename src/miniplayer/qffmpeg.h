#ifndef QFFMPEG_H
#define QFFMPEG_H

#include <QString>
#include <QMutex>
#include <QMutexLocker>

extern "C"
{
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

class QFFmpeg
{

public:
    static QFFmpeg* get();
    virtual ~QFFmpeg();

    bool openFile(const char *filePath);
    AVPacket* readFrame(); //Outside release
    AVFrame* decoder(const AVPacket *pAVPkt);
    bool YUVtoRGBA(char *outData, int outWidth, int outHeight);
    void closeFile();
    QString getError();
    int getVideoStream() { return m_videoIndex; }
    int getTotalMS() { return m_totalMS; }
    int getVideoelapseTime() { return m_videoElapseTime; } //return elapse time(ms)
    double getFps() { return m_fps; }
    bool seek(int ms);
    bool getIsPlay() { return m_isplay; }
    void setIsplay(bool isPlay) { m_isplay = isPlay; }

protected:
    AVFormatContext *m_pFormatCtx = nullptr;
    QMutex m_mutex;
    AVFrame *m_pAVFrameYUV = nullptr;
    int m_videoIndex = -1;
    AVCodec *m_videoCodec = nullptr;
    AVCodecContext *m_videoCodecCtx = nullptr;
    SwsContext *m_pSwsCtx = nullptr;

private:
    QFFmpeg();

    static QFFmpeg *s_qFFmpeg;
    char m_errMsgBuffer[2048];
    int m_totalMS = 0;
    double m_fps = 0;
    int m_videoElapseTime = 0;
    bool m_isplay = false;
};




#endif
