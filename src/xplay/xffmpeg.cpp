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
    {
        QMutexLocker locker(&mutex);
        m_pFormatCtx = avformat_alloc_context();
        int ret = avformat_open_input(&m_pFormatCtx, filePath, nullptr, nullptr);
        if (ret != 0)
        {
            av_strerror(ret, m_errMsgBuffer, sizeof(m_errMsgBuffer));
            return false;
        }
        m_totalMS = (m_pFormatCtx->duration / AV_TIME_BASE) * 1000;
    }
    return true;
}

void XFFmpeg::closeFile()
{
    QMutexLocker locker(&mutex);
    if(m_pFormatCtx)
        avformat_close_input(&m_pFormatCtx);
}

QString XFFmpeg::getError()
{
    QMutexLocker locker(&mutex);
    return QString(m_errMsgBuffer);
}

XFFmpeg::~XFFmpeg()
{

}