/**
* 最简单的基于FFmpeg的视频播放器 2
* Simplest FFmpeg Player 2
*
* 雷霄骅 Lei Xiaohua
* leixiaohua1020@126.com
* 中国传媒大学/数字电视技术
* Communication University of China / Digital TV Technology
* http://blog.csdn.net/leixiaohua1020
*
* 第2版使用SDL2.0取代了第一版中的SDL1.2
* Version 2 use SDL 2.0 instead of SDL 1.2 in version 1.
*
* 本程序实现了视频文件的解码和显示（支持HEVC，H.264，MPEG2等）。
* 是最简单的FFmpeg视频解码方面的教程。
* 通过学习本例子可以了解FFmpeg的解码流程。
* This software is a simplest video player based on FFmpeg.
* Suitable for beginner of FFmpeg.
*
* Version:2
*/


#include "header.h"

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>
    //SDL
#include <SDL.h>
#include <SDL_thread.h>

};
#include <filehelper.h>

//Output YUV420P data as a file 
#define OUTPUT_YUV420P 0

int main(int argc, TCHAR * argv[])
{

    AVFormatContext	*pFormatCtx;
    int				i, videoindex;
    AVCodecContext	*pCodecCtx;
    AVCodec			*pCodec;
    std::string filepath = getFullPath("src01_480x272_22.h265");
    av_register_all();
    avformat_network_init();
    pFormatCtx = avformat_alloc_context();
    if (avformat_open_input(&pFormatCtx, filepath.c_str(), NULL, NULL) != 0) {
        printf("Couldn't open input stream.（无法打开输入流）\n");
        return -1;
    }
    if (avformat_find_stream_info(pFormatCtx, nullptr)<0)
    {
        printf("Couldn't find stream information.（无法获取流信息）\n");
        return -1;
    }
    videoindex = -1;
    for (i = 0; i < pFormatCtx->nb_streams; i++)
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoindex = i;
            break;
        }
    if (videoindex == -1)
    {
        printf("Didn't find a video stream.（没有找到视频流）\n");
        return -1;
    }
    
    pCodec = avcodec_find_decoder(pFormatCtx->streams[videoindex]->codecpar->codec_id);
    pCodecCtx = avcodec_alloc_context3(pCodec);
    avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[videoindex]->codecpar);
    if (pCodec == NULL)
    {
        printf("Codec not found.（没有找到解码器）\n");
        return -1;
    }
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
    {
        printf("Could not open codec.（无法打开解码器）\n");
        return -1;
    }
    AVFrame	*pFrame, *pFrameYUV;
    pFrame = av_frame_alloc();
    pFrameYUV = av_frame_alloc();
    uint8_t *out_buffer = (uint8_t *)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1));
    av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, out_buffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1);
    //SDL---------------------------
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER))
    {
        printf("Could not initialize SDL - %s\n", SDL_GetError());
        return -1;
    }

    int screen_w = 0, screen_h = 0;
    //SDL 2.0 Support for multiple windows
    SDL_Window *screen;
    screen_w = pCodecCtx->width;
    screen_h = pCodecCtx->height;
    screen = SDL_CreateWindow("Simplest ffmpeg player's Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        screen_w, screen_h,
        SDL_WINDOW_OPENGL);


    if (!screen) {
        printf("SDL: could not create window - exiting:%s\n", SDL_GetError());
        return -1;
    }

    SDL_Renderer* sdlRenderer = SDL_CreateRenderer(screen, -1, 0);
    //IYUV: Y + U + V  (3 planes)
    //YV12: Y + V + U  (3 planes)
    SDL_Texture* sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, pCodecCtx->width, pCodecCtx->height);

    SDL_Rect sdlRect;
    sdlRect.x = 0;
    sdlRect.y = 0;
    sdlRect.w = screen_w;
    sdlRect.h = screen_h;
    //SDL End----------------------
    int ret;
    AVPacket *packet = (AVPacket *)av_malloc(sizeof(AVPacket));
    //Output Info-----------------------------
    printf("File Information（文件信息）---------------------\n");
    av_dump_format(pFormatCtx, 0, filepath.c_str(), 0);
    printf("-------------------------------------------------\n");

#if OUTPUT_YUV420P 
    FILE *fp_yuv = fopen("output.yuv", "wb+");
#endif  

    struct SwsContext *img_convert_ctx;
    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, NULL, NULL, NULL);
    //------------------------------
    while (av_read_frame(pFormatCtx, packet) >= 0)
    {
        if (packet->stream_index == videoindex)
        {
            ret = avcodec_send_packet(pCodecCtx, packet);
            if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)
            {
                av_packet_unref(packet);
                return -1;
            }
            avcodec_receive_frame(pCodecCtx, pFrame);
            if (ret < 0 && ret != AVERROR_EOF)
            {
                av_packet_unref(packet);
                return -1;
            }
                sws_scale(img_convert_ctx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);

#if OUTPUT_YUV420P
                int y_size = pCodecCtx->width*pCodecCtx->height;
                fwrite(pFrameYUV->data[0], 1, y_size, fp_yuv); //Y 
                fwrite(pFrameYUV->data[1], 1, y_size / 4, fp_yuv);  //U
                fwrite(pFrameYUV->data[2], 1, y_size / 4, fp_yuv);  //V
#endif
                                                                    //SDL---------------------------
                SDL_UpdateTexture(sdlTexture, &sdlRect, pFrameYUV->data[0], pFrameYUV->linesize[0]);
                SDL_RenderClear(sdlRenderer);
                SDL_RenderCopy(sdlRenderer, sdlTexture, &sdlRect, &sdlRect);
                SDL_RenderPresent(sdlRenderer);
                //SDL End-----------------------
                //Delay 40ms
                SDL_Delay(40);
        }
        av_packet_unref(packet);
    }
    sws_freeContext(img_convert_ctx);

#if OUTPUT_YUV420P 
    fclose(fp_yuv);
#endif 

    SDL_Quit();

    av_free(out_buffer);
    av_free(pFrameYUV);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);

    return 0;
}
