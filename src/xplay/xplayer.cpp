#include "xplayer.h"
#include "ui_PlayerMainWidget.h"

#include <QIcon>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QTimer>
#include <QTime>
#include <QDebug>

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
    connect(m_timer, &QTimer::timeout, this, &XPlayDialog::updateTimeProgressSlider);
    connect(m_ui->openFileButton, &QPushButton::clicked, this, &XPlayDialog::openFile);
    connect(m_ui->playProgressSlider, &QSlider::sliderPressed, this, &XPlayDialog::slotSilderPressed);
    connect(m_ui->playProgressSlider, &QSlider::sliderReleased, this, &XPlayDialog::slotSilderReleased);
    connect(m_ui->playProgressSlider, &QSlider::sliderMoved, this, &XPlayDialog::slotSilderMoved);
    connect(m_ui->playButton, &QPushButton::clicked, this, &XPlayDialog::slotPlayButtonClicked);
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
    m_totalTime = m_xFFmpeg->getTotalMS();
    m_ui->playProgressSlider->setMaximum(m_totalTime / 100);  //100ms precision
    m_ui->playProgressSlider->setMinimum(0);
    m_ui->playProgressSlider->setSingleStep(2);               //200ms precision
    m_ui->playProgressSlider->setPageStep(100);               //10s 
    setWindowTitle(m_title);

    videoStart();
    m_timer->start(timerInterval);
}

void XPlayDialog::updateTimeLabel()
{
    static QTime totalTime;
    static QString totalTimeString("00:00:00");
    if (!totalTime.isValid())
    {
        totalTime.setHMS(0, 0, 0);
        totalTime = totalTime.addSecs(m_totalTime / 1000);
        totalTimeString = totalTime.toString("hh:mm:ss");
    }

    QTime elapseTime(0, 0, 0);
    elapseTime = elapseTime.addSecs(XFFmpeg::get()->getVideoelapseTime() / 1000);
    QString elapseTimeString = elapseTime.toString("hh:mm:ss");
    m_ui->timeLabel->setText(elapseTimeString + QString("/") + totalTimeString);

}

void XPlayDialog::updateTimeProgressSlider()
{
    m_ui->playProgressSlider->setValue(XFFmpeg::get()->getVideoelapseTime() / 100);
}

void XPlayDialog::videoStart()
{
    auto reset = [this]()->void
    {
        m_isPlaying = true;
        XFFmpeg::get()->setIsplay(m_isPlaying);
        m_ui->playButton->setStyleSheet("border-image: url(:/res/images/pause.png);");
    };

    XFFmpeg *xFFmpeg = XFFmpeg::get();
    VideoThread *videoThread = VideoThread::get();
    m_ui->ScreenWidget->setIsDilplayMode(true);
    videoThread->start();
    reset();
}

void XPlayDialog::slotSilderPressed()
{
    m_timer->stop();
}

void XPlayDialog::slotSilderReleased()
{
    m_timer->start(timerInterval);
}

void XPlayDialog::slotSilderMoved(int value)
{
    XFFmpeg::get()->seek(value * 100);
    updateTimeLabel();
}

void XPlayDialog::slotPlayButtonClicked()
{
    m_isPlaying = !m_isPlaying;
    XFFmpeg::get()->setIsplay(m_isPlaying);
    if (m_isPlaying)
        m_ui->playButton->setStyleSheet("border-image: url(:/res/images/pause.png);");
    else
        m_ui->playButton->setStyleSheet("border-image: url(:/res/images/play.png);");
}

XPlayDialog::~XPlayDialog()
{
}
