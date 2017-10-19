#ifndef XPLAY_H
#define XPLAY_H

#include <QDialog>
#include <QString>

namespace Ui {
    class PlayerMainWidget;
}

class XFFmpeg;
class QTimer;

class XPlayDialog : public QDialog
{
    Q_OBJECT


public:
    explicit XPlayDialog(QWidget *parent = nullptr);
    ~XPlayDialog();

private slots:
    void openFile();
    void videoStart();
    void updateTimeLabel();
    void updateTimeProgressSlider();
    void slotSilderPressed();
    void slotSilderReleased();
    void slotSilderMoved(int value);
    void slotPlayButtonClicked();

private:
    Ui::PlayerMainWidget *m_ui;
    XFFmpeg *m_xFFmpeg = nullptr;
    QTimer *m_timer = nullptr;
    QString m_fileName;
    QString m_title;
    int timerInterval = 20;
    int m_elapseTime = 0;  //ms
    int m_totalTime = 0;   //ms
    bool m_isPlaying = false;

};













#endif // !XPLAY_H
