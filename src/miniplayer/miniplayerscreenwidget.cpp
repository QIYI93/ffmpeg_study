#include "miniplayerscreenwidget.h"
#include <QPainter>
#include <QImage>
#include <QPoint>

#include "filehelper.h"
#include "qffmpeg.h"

MiniPlayerScreenWidget::MiniPlayerScreenWidget(QWidget *parent)
    :QOpenGLWidget(parent)
{
    startTimer(20);
}

MiniPlayerScreenWidget::~MiniPlayerScreenWidget()
{

}

void MiniPlayerScreenWidget::paintEvent(QPaintEvent *e)
{
    static QImage *img = nullptr;
    static int h = 0;
    static int w = 0;
    if (w != width() && h != height())
    {
        if (img)
        {
            delete img->bits();
            delete img;
            img = nullptr;
        }
    }
    if (img == nullptr)
    {
        uchar *buffer = (uchar*)malloc(width() * height() * 4 * sizeof(uchar));
        img = new QImage(buffer, width(), height(), QImage::Format_ARGB32);
    }
    if (m_isDilplayMode)
    {
        QFFmpeg *qFFmpeg = QFFmpeg::get();
        qFFmpeg->YUVtoRGBA((char*)img->bits(), width(), height());

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

void MiniPlayerScreenWidget::timerEvent(QTimerEvent *e)
{
    this->update();
}