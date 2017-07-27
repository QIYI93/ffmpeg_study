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

using namespace std;
void SaveBmp(AVCodecContext *CodecContex, AVFrame *Picture, int width, int height, int num)
{
    AVPicture pPictureRGB;//RGB图片  

    static struct SwsContext *img_convert_ctx;
    img_convert_ctx = sws_getContext(width, height, CodecContex->pix_fmt, width, height, AV_PIX_FMT_RGB24, SWS_BICUBIC, NULL, NULL, NULL);

    // 确认所需缓冲区大小并且分配缓冲区空间  
    avpicture_alloc(&pPictureRGB, AV_PIX_FMT_RGB24, width, height);
    sws_scale(img_convert_ctx, Picture->data, Picture->linesize, \
        0, height, pPictureRGB.data, pPictureRGB.linesize);

    int lineBytes = pPictureRGB.linesize[0], i = 0;

    char fileName[1024] = { 0 };
    char * bmpSavePath = "D:\\project\\ffmpeg\\ffmpeg_study\\build\\rundir\\Debug\\1.bmp";
 
    sprintf(fileName, bmpSavePath, num);

    FILE *pDestFile = fopen(fileName, "wb");
    BITMAPFILEHEADER btfileHeader;
    btfileHeader.bfType = MAKEWORD(66, 77);
    btfileHeader.bfSize = lineBytes*height;
    btfileHeader.bfReserved1 = 0;
    btfileHeader.bfReserved2 = 0;
    btfileHeader.bfOffBits = 54;

    BITMAPINFOHEADER bitmapinfoheader;
    bitmapinfoheader.biSize = 40;
    bitmapinfoheader.biWidth = width;
    bitmapinfoheader.biHeight = height;
    bitmapinfoheader.biPlanes = 1;
    bitmapinfoheader.biBitCount = 24;
    bitmapinfoheader.biCompression = BI_RGB;
    bitmapinfoheader.biSizeImage = lineBytes*height;
    bitmapinfoheader.biXPelsPerMeter = 0;
    bitmapinfoheader.biYPelsPerMeter = 0;
    bitmapinfoheader.biClrUsed = 0;
    bitmapinfoheader.biClrImportant = 0;

    fwrite(&btfileHeader, 14, 1, pDestFile);
    fwrite(&bitmapinfoheader, 40, 1, pDestFile);
    for (i = height - 1; i >= 0; i--)
    {
        fwrite(pPictureRGB.data[0] + i*lineBytes, lineBytes, 1, pDestFile);
    }

    fclose(pDestFile);
    avpicture_free(&pPictureRGB);
}

int main(int argc, char * argv[])
{
    std::string filePath = file_Helper::getVideoFullPath("src01_480x272_22.h265");

    AVFormatContext *pInputFormatContext = avformat_alloc_context();//NULL;   
    AVCodecContext *pInputCodecContext = NULL;
    AVCodec *pInputCodec = NULL;

    //注册库中含有的所有可用的文件格式和编码器，这样当打开一个文件时，它们才能够自动选择相应的文件格式和编码器。  
    av_register_all();

    int ret;
    // 打开视频文件  
    if ((ret = avformat_open_input(&pInputFormatContext, filePath.c_str(), NULL, NULL)) != 0){
        cout << " can't open file " << endl;
        return -1;
    }
    // 取出文件流信息  
    if (avformat_find_stream_info(pInputFormatContext, NULL)<0){
        cout << " can't find suitable codec parameters" << endl;
        return -1;
    }
    //用于诊断 //产品中不可用  
    //dump_format(pInputFormatContext, 0, sourceFile, false);  

    //仅仅处理视频流  
    //只简单处理我们发现的第一个视频流  
    //  寻找第一个视频流  
    int videoIndex = -1;
    for (int i = 0; i<pInputFormatContext->nb_streams; i++) {
        if (pInputFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
            videoIndex = i;
            break;
        }
    }
    if (-1 == videoIndex){
        cout << " can't find video stream !" << endl;
        return -1;
    }
    // 得到视频流编码上下文的指针  
    pInputCodecContext = pInputFormatContext->streams[videoIndex]->codec;
    //  寻找视频流的解码器  
    pInputCodec = avcodec_find_decoder(pInputCodecContext->codec_id);

    if (NULL == pInputCodec){
        cout << "can't decode " << endl;
        return -1;
    }

    // 通知解码器我们能够处理截断的bit流，bit流帧边界可以在包中  
    //视频流中的数据是被分割放入包中的。因为每个视频帧的数据的大小是可变的，  
    //那么两帧之间的边界就不一定刚好是包的边界。这里，我们告知解码器我们可以处理bit流。  
    if (pInputCodec->capabilities & CODEC_CAP_TRUNCATED){
        pInputCodecContext->flags |= CODEC_FLAG_TRUNCATED;
    }


    //打开解码器  
    if (avcodec_open2(pInputCodecContext, pInputCodec, NULL) != 0) {
        cout << "decode error" << endl;
        return -1;
    }
    int videoHeight;
    int videoWidth;
    videoWidth = pInputCodecContext->width;
    videoHeight = pInputCodecContext->height;

    AVPacket InPack;
    int len = 0;
    AVFrame OutFrame;
    int nComplete = 0;

    //  加入这句话来纠正某些编码器产生的帧速错误  
    /* if(pInputCodecContext->frame_rate>1000 && pInputCodecContext->frame_rate_base==1)
    pInputCodecContext->frame_rate_base=1000;*/


    int nFrame = 0;
    AVRational avRation = pInputCodecContext->time_base;
    float frameRate = (float)avRation.den / avRation.num;
    //av_seek_frame(pInputFormatContext,0);  
    while ((av_read_frame(pInputFormatContext, &InPack) >= 0)){
        len = avcodec_decode_video2(pInputCodecContext, &OutFrame, &nComplete, &InPack);

        //判断是否是关键帧  
        if (nComplete > 0 && OutFrame.key_frame){
            //解码一帧成功  
            SaveBmp(pInputCodecContext, &OutFrame, videoWidth, videoHeight, nFrame);
            nFrame++;
        }
    }
    cout << " save frame number: " << nFrame << endl;
    avcodec_close(pInputCodecContext);
    av_free(pInputFormatContext);

    system("PAUSE");
}