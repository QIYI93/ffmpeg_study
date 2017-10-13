#include <windows.h>
#include <QApplication>
#include <QDebug>

extern "C"
{
#include <libavformat/avformat.h>
}

#include <filehelper.h>
#include "xplay.h"

static inline double r2d(AVRational r)
{
    return r.num == 0 || r.den == 0 ? 0 : (double)r.num / (double)r.den;
}

int main(int argc, char *argv[])
{
    char bufferErrMsg[4096] = { 0 };
    av_register_all();
    AVFormatContext *pFormatCtx = nullptr;
    pFormatCtx = avformat_alloc_context();
    std::string filePath = file_Helper::getVideoFullPath("Titanic.mkv");
    int ret = avformat_open_input(&pFormatCtx, filePath.c_str(), nullptr, nullptr);
    if(ret != 0)
    {
        av_strerror(ret, bufferErrMsg, sizeof(bufferErrMsg));
        qDebug() << "Open " << filePath.c_str() << "failed:" << bufferErrMsg;
        return -1;
    }
    int videoIndex = -1;
    for (int i = 0; i < pFormatCtx->nb_streams; i++)
    {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
            videoIndex = i;
    }
    if (videoIndex == -1)
    {
        qDebug() << "Not found video stream.";
        return -1;
    }
    AVCodec *pCodec = nullptr;
    pCodec = avcodec_find_decoder(pFormatCtx->streams[videoIndex]->codecpar->codec_id);
    if (pCodec == nullptr)
    {
        qDebug() << "Not found video decoder.";
        return -1;
    }
    AVCodecContext *pCodecCtx = nullptr;
    pCodecCtx = avcodec_alloc_context3(pCodec);
    avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[videoIndex]->codecpar);
    ret = avcodec_open2(pCodecCtx, pCodec, nullptr);
    if (ret != 0)
    {
        memset(bufferErrMsg, 0, sizeof(bufferErrMsg));
        av_strerror(ret, bufferErrMsg, sizeof(bufferErrMsg));
        qDebug() << "Open decoder failed:" << bufferErrMsg;
        return -1;
    }
    //int totalSec = pFormatCtx->duration / AV_TIME_BASE;
    //qDebug() << "file total sec: " << totalSec; //video duration

    AVPacket *pPacket = nullptr;
    pPacket = (AVPacket*)av_malloc(sizeof AVPacket);
    while (true)
    {
        if(0 != av_read_frame(pFormatCtx, pPacket)) break;
        qDebug() << "Pts: " << pPacket->pts * r2d(pFormatCtx->streams[pPacket->stream_index]->time_base) * 1000;
        av_packet_unref(pPacket);
    }


    avformat_close_input(&pFormatCtx);



    QApplication a(argc, argv);
    XplayDialog playDlg;
    playDlg.show();
    return a.exec();
}
