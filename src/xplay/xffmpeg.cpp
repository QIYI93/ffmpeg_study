#include "xffmpeg.h"

XFFmpeg* XFFmpeg::s_xFFMpeg = new XFFmpeg();

XFFmpeg::XFFmpeg()
{
    av_register_all();
    m_errMsgBuffer[0] = '\0';
}

XFFmpeg* XFFmpeg::get()
{
    return s_xFFMpeg;
}

bool XFFmpeg::openFile(const char* filePath)
{
    closeFile();

    QMutexLocker locker(&m_mutex);
    m_pFormatCtx = avformat_alloc_context();
    int ret = avformat_open_input(&m_pFormatCtx, filePath, nullptr, nullptr);
    if (ret != 0)
    {
        av_strerror(ret, m_errMsgBuffer, sizeof(m_errMsgBuffer));
        return false;
    }
    m_totalMS = (m_pFormatCtx->duration / AV_TIME_BASE) * 1000;
    //Find video stream
    for (int i = 0; i < m_pFormatCtx->nb_streams; i++)
    {
        if (m_pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
            m_videoIndex = i;
    }
    if (m_videoIndex == -1)
    {
        strcpy(m_errMsgBuffer, "Not found video stream.");
        return false;
    }
    //Open video codec
    m_videoCodec = avcodec_find_decoder(m_pFormatCtx->streams[m_videoIndex]->codecpar->codec_id);
    if (m_videoCodec == nullptr)
    {
        strcpy(m_errMsgBuffer, "Not found video decoder.");
        return false;
    }
    m_videoCodecCtx = avcodec_alloc_context3(m_videoCodec);
    avcodec_parameters_to_context(m_videoCodecCtx, m_pFormatCtx->streams[m_videoIndex]->codecpar);
    ret = avcodec_open2(m_videoCodecCtx, m_videoCodec, nullptr);
    if (ret != 0)
    {
        av_strerror(ret, m_errMsgBuffer, sizeof(m_errMsgBuffer));
        return false;
    }
    return true;
}

void XFFmpeg::closeFile()
{
    QMutexLocker locker(&m_mutex);
    if(m_pFormatCtx)
        avformat_close_input(&m_pFormatCtx);
    if (m_AVFrameYUV)
        av_frame_free(&m_AVFrameYUV);
}

QString XFFmpeg::getError()
{
    QMutexLocker locker(&m_mutex);
    return QString(m_errMsgBuffer);
}

AVPacket* XFFmpeg::readFrame()
{
    AVPacket *pAVPacket = (AVPacket*)av_malloc(sizeof(AVPacket));
    memset(pAVPacket, 0, sizeof(AVPacket));
    QMutexLocker locker(&m_mutex);
    if (!m_pFormatCtx)
        return pAVPacket;
    int ret = av_read_frame(m_pFormatCtx, pAVPacket);
    if (ret != NULL)
        av_strerror(ret, m_errMsgBuffer, sizeof(m_errMsgBuffer));
    return pAVPacket;
}

AVFrame* XFFmpeg::decoder(const AVPacket* pAVPkt)
{
    QMutexLocker locker(&m_mutex);
    if (!m_pFormatCtx)
        return nullptr;
    if (m_AVFrameYUV == nullptr)
        m_AVFrameYUV = av_frame_alloc();
    if (pAVPkt->stream_index == m_videoIndex)
    {
        if (0 != avcodec_send_packet(m_videoCodecCtx, pAVPkt))
            return nullptr;
        if (0 != avcodec_receive_frame(m_videoCodecCtx, m_AVFrameYUV))
            return nullptr;
    }

    return m_AVFrameYUV;

}

XFFmpeg::~XFFmpeg()
{

}