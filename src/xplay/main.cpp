#include <windows.h>
#include <QApplication>
#include <QDebug>

extern "C"
{
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

#include <filehelper.h>
#include "xffmpeg.h"
#include "xplayer.h"

int main(int argc, char *argv[])
{
    //XFFmpeg *xFFmpeg = XFFmpeg::get();
    //bool rrr = xFFmpeg->openFile(file_Helper::getVideoFullPath("Titanic.mkv").c_str());
    //char *bgra_ = (char*)malloc(600 * 400 * 4 * sizeof(char));
    //while (true)
    //{
    //    AVPacket* pAVPkt = xFFmpeg->readFrame();
    //    if(pAVPkt->size == NULL)
    //        break;
    //    if (pAVPkt->stream_index != xFFmpeg->getVideoStream())
    //    {
    //        av_packet_unref(pAVPkt);
    //        continue;
    //    }
    //    AVFrame *pAVFrame = xFFmpeg->decoder(pAVPkt);
    //    if (pAVFrame)
    //    {
    //        xFFmpeg->YUVtoRGBA(pAVFrame, bgra_, 600, 400);
    //    }
    //}
    //qDebug() << "[D]Pts: " << pPacket->pts * r2d(pFormatCtx->streams[pPacket->stream_index]->time_base) * 1000;

    QApplication a(argc, argv);
    XPlayDialog playDlg;
    playDlg.show();
    return a.exec();
}
