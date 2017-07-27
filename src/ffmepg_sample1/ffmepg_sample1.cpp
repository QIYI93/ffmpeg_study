#include "header.h"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>

};
#include <filehelper.h>

void saveFrame(AVFrame* pFrame, int width, int height, int iFrame)
{
    FILE *pFile;
    char szFilename[32];
    int y;

    sprintf(szFilename, "frame%d.ppm", iFrame);
    pFile = fopen(szFilename, "wb");
    if (pFile == NULL)
        return;

    fprintf(pFile, "P6\n%d %d\n255\n", width, height);

    for (y = 0; y < height; y++)
        fwrite(pFrame->data[0] + y*pFrame->linesize[0], 1, width * 3, pFile);

    fclose(pFile);
}

int main(int argc, TCHAR * argv[])
{
    AVFormatContext *pFormatCtx;
    int i, videoIndex;
    AVCodecContext *pCodecCtx = NULL;
    AVCodec *pCodec = NULL;

    AVPacket packet;
    int frameFinished;
    int numBytes;
    uint8_t *buffer;

    std::string filePath = file_Helper::getVideoFullPath("cuc_ieschool.flv");
    pFormatCtx = avformat_alloc_context();
    av_register_all();
    if (avformat_open_input(&pFormatCtx, filePath.c_str(), NULL, 0) != 0)
        return -1;
    if (avformat_find_stream_info(pFormatCtx, NULL) < 0)
        return -1;
    av_dump_format(pFormatCtx, 0, filePath.c_str(), 0);

    videoIndex = -1;
    for (i = 0; i < pFormatCtx->nb_streams; ++i) {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoIndex = i;
            break;
        }
    }
    if (videoIndex == -1)
    {
        fprintf(stderr, "unsupport codec\n");
        return -1;
    }
    //AVStream *stream = pFormatCtx->streams[videoIndex];
    //int frame_rate = stream->avg_frame_rate.num / stream->avg_frame_rate.den;

    pCodecCtx = pFormatCtx->streams[videoIndex]->codec;
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
        return -1;

    AVFrame *pFrameRGB, *pFrame;
    pFrame = av_frame_alloc();
    pFrameRGB = av_frame_alloc();
    if (pFrame == NULL)
        return -1;

    numBytes = avpicture_get_size(AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);
    buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));

    avpicture_fill((AVPicture*)pFrameRGB, buffer, AV_PIX_FMT_RGB24, pCodecCtx->width, pCodecCtx->height);

    i = 0;
    struct SwsContext* img_convert_ctx;
    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
        pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);

    while (av_read_frame(pFormatCtx, &packet) >= 0) {
        if (packet.stream_index == videoIndex) {
            avcodec_decode_video2(pCodecCtx, pFrame, &frameFinished, &packet);

            if (frameFinished)
            {
                sws_scale(img_convert_ctx, pFrame->data, pFrame->linesize, 0, pCodecCtx->height,
                    pFrameRGB->data, pFrameRGB->linesize);

                if (++i <= 100)
                    saveFrame(pFrameRGB, pCodecCtx->width, pCodecCtx->height, i);
            }
        }
        av_free_packet(&packet);
    }

    av_free(buffer);
    av_free(pFrameRGB);
    av_free(pFrame);
    avcodec_close(pCodecCtx);

    return 0;
}
