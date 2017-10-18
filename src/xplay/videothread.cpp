#include "videothread.h"
#include "xffmpeg.h"

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
    XFFmpeg *xFFmpeg = XFFmpeg::get();
    while (!isExit)
    {
        AVPacket *pPkt = xFFmpeg->readFrame();
        if (pPkt->size <= 0)
        {
            msleep(10);
            continue;
        }
        if (pPkt->stream_index != xFFmpeg->getVideoStream())
        {
            av_packet_unref(pPkt);
            continue;
        }
        AVFrame *avFrameYUV = xFFmpeg->decoder(pPkt);
        av_packet_unref(pPkt);
        if (xFFmpeg->getFps() > 0)
            msleep(1000 / xFFmpeg->getFps());
    }
}

VideoThread::~VideoThread()
{
}