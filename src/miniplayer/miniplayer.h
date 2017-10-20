#ifndef MINIPLAYER_H
#define MINIPLAYER_H

#include <QDialog>
#include <QString>

namespace Ui {
    class PlayerMainWidget;
}

class QFFmpeg;
class QTimer;

class MiniPlayerDialog : public QDialog
{
    Q_OBJECT


public:
    explicit MiniPlayerDialog(QWidget *parent = nullptr);
    ~MiniPlayerDialog();

private slots:
    void openFile();
    void videoStart();
    void updateTimeLabel();
    void updateTimeProgressSlider();
    void slotSilderPressed();
    void slotSilderReleased();
    void slotSilderMoved(int value);
    void slotPlayButtonClicked();

protected:
    void resizeEvent(QResizeEvent *) override;
    bool eventFilter(QObject *, QEvent *) override;


private:
    Ui::PlayerMainWidget *m_ui;
    QFFmpeg *m_qFFmpeg = nullptr;
    QTimer *m_timer = nullptr;
    QString m_fileName;
    QString m_title;
    int timerInterval = 20;
    int m_elapseTime = 0;  //ms
    int m_totalTime = 0;   //ms
    bool m_isPlaying = false;

};













#endif // !MiniPlayer_H
