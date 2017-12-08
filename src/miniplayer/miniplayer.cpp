#include "miniplayer.h"
#include "ui_PlayerMainWidget.h"

#include <QIcon>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QTimer>
#include <QTime>
#include <QDebug>
#include <QResizeEvent>
#include <QMouseEvent>

#include "qffmpeg.h"
#include "videothread.h"

namespace {
    const auto miniWidth = 300;
    const auto miniHeight = 200;
}

MiniPlayerDialog::MiniPlayerDialog(QWidget *parent)
    :QDialog(parent)
    , m_ui(new Ui::PlayerMainWidget)
{
    m_ui->setupUi(this);
    m_title = "MiniPlayerer";
    setWindowTitle(m_title);
    setWindowIcon(QIcon(":/res/images/MiniPlayerer.ico"));

    m_qFFmpeg = QFFmpeg::get();

    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &MiniPlayerDialog::updateTimeLabel);
    connect(m_timer, &QTimer::timeout, this, &MiniPlayerDialog::updateTimeProgressSlider);
    connect(m_ui->openFileButton, &QPushButton::clicked, this, &MiniPlayerDialog::openFile);
    connect(m_ui->playProgressSlider, &QSlider::sliderPressed, this, &MiniPlayerDialog::slotSilderPressed);
    connect(m_ui->playProgressSlider, &QSlider::sliderReleased, this, &MiniPlayerDialog::slotSilderReleased);
    connect(m_ui->playProgressSlider, &QSlider::sliderMoved, this, &MiniPlayerDialog::slotSilderMoved);
    connect(m_ui->playButton, &QPushButton::clicked, this, &MiniPlayerDialog::slotPlayButtonClicked);

    m_ui->playProgressSlider->installEventFilter(this);

    setMinimumSize(miniWidth,miniHeight);
}

void MiniPlayerDialog::openFile()
{
    //m_fileName = QFileDialog::getOpenFileName(this, "Choose video file", NULL, "*.mp4 *.ts *.mov *.flv *.mkv *.avi *.gif *.webm");
    m_fileName = "D:\\project\\ffmpeg\\media_data\\video\\sintel.wmv";
    if (m_fileName.isEmpty())
        return; 
    bool ret = m_qFFmpeg->openFile(m_fileName.toLocal8Bit().data());
    if (!ret)
    {
        QString msg = QString("Can not play this video\n%1").arg(m_fileName);
        QMessageBox::critical(NULL, "critical", msg, QMessageBox::Yes, QMessageBox::Yes);
        m_ui->ScreenWidget->setIsDilplayMode(false);
        return;
    }
    m_title = m_title + "  " + QFileInfo(m_fileName).fileName();
    m_totalTime = m_qFFmpeg->getTotalMS();
    m_ui->playProgressSlider->setMaximum(m_totalTime / 100);  //100ms precision
    m_ui->playProgressSlider->setMinimum(0);
    m_ui->playProgressSlider->setSingleStep(2);               //200ms precision
    m_ui->playProgressSlider->setPageStep(100);               //10s 
    setWindowTitle(m_title);

    videoStart();
    m_timer->start(timerInterval);
}

void MiniPlayerDialog::updateTimeLabel()
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
    elapseTime = elapseTime.addSecs(QFFmpeg::get()->getVideoelapseTime() / 1000);
    QString elapseTimeString = elapseTime.toString("hh:mm:ss");
    m_ui->timeLabel->setText(elapseTimeString + QString("/") + totalTimeString);

}

void MiniPlayerDialog::updateTimeProgressSlider()
{
    m_ui->playProgressSlider->setValue(QFFmpeg::get()->getVideoelapseTime() / 100);
}

void MiniPlayerDialog::videoStart()
{
    auto reset = [this]()->void
    {
        m_isPlaying = true;
        QFFmpeg::get()->setIsplay(m_isPlaying);
        m_ui->playButton->setStyleSheet("border-image: url(:/res/images/pause.png);");
    };

    QFFmpeg *qFFmpeg = QFFmpeg::get();
    VideoThread *videoThread = VideoThread::get();
    m_ui->ScreenWidget->setIsDilplayMode(true);
    videoThread->start();
    reset();
}

void MiniPlayerDialog::resizeEvent(QResizeEvent *event)
{
    m_ui->ScreenWidget->resize(size());
    m_ui->buttonWidget->setGeometry(20, size().height() - 70, size().width() - 40, 60);
    m_ui->openFileButton->setGeometry(5, 30, 27, 22);
    m_ui->playButton->setGeometry(m_ui->buttonWidget->size().width() / 2 - 7, 30, 15, 22);
    m_ui->timeLabel->setGeometry(m_ui->buttonWidget->size().width() - 120, 20, 115, 20);
    m_ui->playProgressSlider->setGeometry(5, 1, m_ui->buttonWidget->size().width() - 10, 22);
}

void MiniPlayerDialog::slotSilderPressed()
{
    m_timer->stop();
}

void MiniPlayerDialog::slotSilderReleased()
{
    m_timer->start(timerInterval);
}

bool MiniPlayerDialog::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_ui->playProgressSlider)
    {
        if (event->type() == QEvent::MouseButtonPress)
        {
            int offset = static_cast<QMouseEvent*>(event)->pos().x();
            double rate = (double)offset / (double)m_ui->playProgressSlider->width();
            emit m_ui->playProgressSlider->sliderMoved(rate* m_ui->playProgressSlider->maximum());
            m_timer->stop();
            return true;
        }
        else if(event->type() == QEvent::MouseButtonRelease)
        {
            m_timer->start(timerInterval);
            return true;
        }
        else
        {
            return false;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void MiniPlayerDialog::slotSilderMoved(int value)
{
    QFFmpeg::get()->seek(value * 100);
    updateTimeLabel();
}

void MiniPlayerDialog::slotPlayButtonClicked()
{
    m_isPlaying = !m_isPlaying;
    QFFmpeg::get()->setIsplay(m_isPlaying);
    if (m_isPlaying)
        m_ui->playButton->setStyleSheet("border-image: url(:/res/images/pause.png);");
    else
        m_ui->playButton->setStyleSheet("border-image: url(:/res/images/play.png);");
}

MiniPlayerDialog::~MiniPlayerDialog()
{
}
