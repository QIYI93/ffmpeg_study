#include "videothread.h"
#include "qffmpeg.h"

VideoThread *VideoThread::s_videoThread = new VideoThread();
bool isExit = false;

VideoThread* VideoThread::get()
{
    return s_videoThread;
}

VideoThread::VideoThread(QObject *parent)
    :QThread(parent)
{

}

void VideoThread::run()
{
    QFFmpeg *qFFmpeg = QFFmpeg::get();
    while (!isExit)
    {
        if (!qFFmpeg->getIsPlay())
        {
            msleep(10);
            continue;
        }
        AVPacket *pPkt = qFFmpeg->readFrame();
        if (pPkt->size <= 0)
        {
            msleep(10);
            continue;
        }
        if (pPkt->stream_index != qFFmpeg->getVideoStream())
        {
            av_packet_unref(pPkt);
            continue;
        }
        AVFrame *avFrameYUV = qFFmpeg->decoder(pPkt);
        av_packet_unref(pPkt);
        if (qFFmpeg->getFps() > 0)
            msleep(1000 / qFFmpeg->getFps());
    }
}

VideoThread::~VideoThread()
{
}