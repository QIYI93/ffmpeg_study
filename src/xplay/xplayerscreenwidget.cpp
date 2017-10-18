#include "xplayerscreenwidget.h"
#include <QPainter>
#include <QImage>
#include <QPoint>

#include "filehelper.h"
#include "xffmpeg.h"

XPlayerScreenWidget::XPlayerScreenWidget(QWidget *parent)
    :QOpenGLWidget(parent)
{
    startTimer(20);
}

XPlayerScreenWidget::~XPlayerScreenWidget()
{

}

void XPlayerScreenWidget::paintEvent(QPaintEvent *e)
{
    static QImage *img = nullptr;
    if (img == nullptr)
    {
        uchar *buffer = (uchar*)malloc(width() * height() * 4 * sizeof(uchar));
        img = new QImage(buffer, width(), height(), QImage::Format_ARGB32);
    }
    if (m_isDilplayMode)
    {
        XFFmpeg *xFFmpeg = XFFmpeg::get();
        xFFmpeg->YUVtoRGBA((char*)img->bits(), width(), height());

        QPainter painter;
        painter.begin(this);
        painter.drawImage(QPoint(0, 0), *img);
        painter.end();
    }
    else
    {
        QOpenGLWidget::paintEvent(e);
    }
}

void XPlayerScreenWidget::timerEvent(QTimerEvent *e)
{
    this->update();
}