#include <iostream>

extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libavutil/imgutils.h>

#include <SDL.h>
}

#include "filehelper.h"

#define SFM_REFRESH_EVENT (SDL_USEREVENT + 1)
#define SFM_BREAK_EVENT (SDL_USEREVENT + 2)

int thread_pause = 0;
int thread_exit = 0;

int fps_refresh_thread(void* obaque)
{
    thread_pause = 0;
    thread_exit = 0;
    while (!thread_exit)
    {
        if (!thread_pause)
        {
            SDL_Event event;
            event.type = SFM_REFRESH_EVENT;
            SDL_PushEvent(&event);
        }
        SDL_Delay(40); //25fps
    }
    thread_pause = 0;
    thread_exit = 0;
    SDL_Event event;
    event.type = SFM_BREAK_EVENT;
    SDL_PushEvent(&event);
    return 0;
}

int main(int argc, char * argv[])
{
    AVFormatContext* pFormatCtx;
    AVCodec* pcodec;
    AVCodecContext* pCodecCtx;
    int videoIndex;
    AVFrame* pFrame, *pFrameYUV;
    unsigned char* outBuffer;
    AVPacket* pPacket;

    //SDL
    int screenWidth, screenHeignt;
    SDL_Window* pSDLWindow;
    SDL_Renderer* pSDLRenderer;
    SDL_Texture* pSDLTexture;
    SDL_Rect SDLRect;
    SDL_Thread* pSDLVideoTid;
    SDL_Event SDLEvent;

    struct SwsContext* pImgConvertCtx;

    std::string filePath = file_Helper::getVideoFullPath("src01_480x272_22.h265");

    av_register_all();
    avformat_network_init();
    pFormatCtx = avformat_alloc_context();

    if (avformat_open_input(&pFormatCtx, filePath.c_str(), nullptr, nullptr) != 0) { std::cout << "Couldn't open input stream" << std::endl; return 1; }
    if (avformat_find_stream_info(pFormatCtx, nullptr) < 0) {std::cout << "Couldn't find stream information" << std::endl; return 1; }
    videoIndex = -1;
    for (int i = 0; i < pFormatCtx->nb_streams; i++)
    {
        if (pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
        {
            videoIndex = i;
            break;
        }
    }
    if(videoIndex == -1) { std::cout << "Couldn't find video stream" << std::endl; return 1; }
    pcodec = avcodec_find_decoder(pFormatCtx->streams[videoIndex]->codecpar->codec_id);
    if(pcodec == nullptr) { std::cout << "Couldn't find decoder" << std::endl; return 1; }
    pCodecCtx = avcodec_alloc_context3(pcodec);
    avcodec_parameters_to_context(pCodecCtx, pFormatCtx->streams[videoIndex]->codecpar);
    if(avcodec_open2(pCodecCtx, pcodec, nullptr) < 0) { std::cout << "Couldn't open codec" << std::endl; return 1; }

    pFrame = av_frame_alloc();
    pFrameYUV = av_frame_alloc();
    outBuffer = (unsigned char*)av_malloc(av_image_get_buffer_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1));
    av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, outBuffer, AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1);
    //file info
    std::cout << "---------------------file info---------------------" << std::endl;
    av_dump_format(pFormatCtx, videoIndex, filePath.c_str(), 0);
    std::cout << "---------------------------------------------------" << std::endl;
    pImgConvertCtx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P, SWS_BICUBIC, nullptr, nullptr, nullptr);

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { std::cout << "Couldn't initialize SDL:" << SDL_GetError() << std::endl; return 1; }
    screenWidth = pCodecCtx->width;
    screenHeignt = pCodecCtx->height;
    pSDLWindow = SDL_CreateWindow("Player", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeignt, SDL_WINDOW_SHOWN);
    if (pSDLWindow == nullptr) { std::cout << "Couldn't create window:" << SDL_GetError() << std::endl; return 1; }
    pSDLRenderer = SDL_CreateRenderer(pSDLWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    pSDLTexture = SDL_CreateTexture(pSDLRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING, pCodecCtx->width, pCodecCtx->height);

    SDLRect.x = 0;
    SDLRect.y = 0;
    SDLRect.w = screenWidth;
    SDLRect.h = screenHeignt;

    pPacket = (AVPacket*)av_malloc(sizeof(AVPacket));

    pSDLVideoTid = SDL_CreateThread(fps_refresh_thread, NULL, NULL);

    int ret;
    //loop
    while (true)
    {
        SDL_WaitEvent(&SDLEvent);
        if (SDLEvent.type == SFM_REFRESH_EVENT)
        {
            while (true)
            {
                if (av_read_frame(pFormatCtx, pPacket) < 0)
                    thread_exit = 1;
                if(pPacket->stream_index == videoIndex)
                    break;
            }
            ret = avcodec_send_packet(pCodecCtx, pPacket);
            if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF) { av_packet_unref(pPacket); return 1; }
            ret = avcodec_receive_frame(pCodecCtx, pFrame);
            if (ret < 0 && ret != AVERROR_EOF) { av_packet_unref(pPacket); return 1; }
            sws_scale(pImgConvertCtx, (const uint8_t* const*)pFrame->data, pFrame->linesize, 0, pCodecCtx->height, pFrameYUV->data, pFrameYUV->linesize);
            SDL_UpdateTexture(pSDLTexture, NULL, pFrameYUV->data[0], pFrameYUV->linesize[0]);
            SDL_RenderClear(pSDLRenderer);
            SDL_RenderCopy(pSDLRenderer, pSDLTexture, nullptr, nullptr);
            SDL_RenderPresent(pSDLRenderer);
            av_packet_unref(pPacket);
        }
        else if (SDLEvent.type == SDL_KEYDOWN)
        {
            if (SDLEvent.key.keysym.sym == SDLK_SPACE)
                thread_pause = !thread_pause;
        }
        else if (SDLEvent.type == SDL_QUIT)
        {
            thread_exit = 1;
        }
        else if (SDLEvent.type == SFM_BREAK_EVENT)
        {
            break;
        }
    }
    sws_freeContext(pImgConvertCtx);

    SDL_DestroyTexture(pSDLTexture);
    SDL_DestroyRenderer(pSDLRenderer);
    SDL_DestroyWindow(pSDLWindow);
    SDL_Quit();

    av_frame_free(&pFrame);
    av_frame_free(&pFrameYUV);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);

    return 0;
}
