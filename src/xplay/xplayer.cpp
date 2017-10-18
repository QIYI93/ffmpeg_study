#include "xplayer.h"
#include "ui_PlayerMainWidget.h"

#include <QIcon>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QTimer>
#include <QTime>

#include "xffmpeg.h"
#include "videothread.h"

XPlayDialog::XPlayDialog(QWidget *parent)
    :QDialog(parent)
    , m_ui(new Ui::PlayerMainWidget)
{
    m_ui->setupUi(this);
    m_title = "XPlayer";
    setWindowTitle(m_title);
    setWindowIcon(QIcon(":/res/images/XPlayer.ico"));

    m_xFFmpeg = XFFmpeg::get();

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &XPlayDialog::updateTimeLabel);
    connect(m_ui->openFileButton, &QPushButton::clicked, this, &XPlayDialog::openFile);
}

void XPlayDialog::openFile()
{
    m_fileName = QFileDialog::getOpenFileName(this, "Choose video file", NULL, "*.mp4 *.ts *.mov *.flv *.mkv *.avi *.gif *.webm");
    if (m_fileName.isEmpty())
        return;
    bool ret = m_xFFmpeg->openFile(m_fileName.toLocal8Bit().data());
    if (!ret)
    {
        QString msg = QString("Can not play this video\n%1").arg(m_fileName);
        QMessageBox::critical(NULL, "critical", msg, QMessageBox::Yes, QMessageBox::Yes);
        m_ui->ScreenWidget->setIsDilplayMode(false);
        return;
    }
    m_title = m_title + "  " + QFileInfo(m_fileName).fileName();
    m_totalTime = m_xFFmpeg->getTotalMS() / 1000;
    setWindowTitle(m_title);

    videoStart();
    m_timer->start(100);
}

void XPlayDialog::updateTimeLabel()
{
    static QTime totalTime;
    static QString totalTimeString("00:00:00");
    if (!totalTime.isValid())
    {
        totalTime.setHMS(0, 0, 0);
        totalTime = totalTime.addSecs(m_totalTime);
        totalTimeString = totalTime.toString("hh:mm:ss");
    }

    QTime elapseTime(0, 0, 0);
    elapseTime = elapseTime.addSecs(m_elapseTime);
    QString elapseTimeString = elapseTime.toString("hh:mm:ss");
    m_ui->timeLabel->setText(elapseTimeString + QString("/") + totalTimeString);
}

void XPlayDialog::videoStart()
{
    XFFmpeg *xFFmpeg = XFFmpeg::get();
    VideoThread *videoThread = VideoThread::get();
    m_ui->ScreenWidget->setIsDilplayMode(true);
    videoThread->start();
}

XPlayDialog::~XPlayDialog()
{
}
