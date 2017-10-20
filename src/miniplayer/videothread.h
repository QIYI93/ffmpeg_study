#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <QThread>

class VideoThread : public QThread
{
public:
    VideoThread(QObject *parent = nullptr);
    ~VideoThread();
    static VideoThread* get();
    virtual void run() override;
private:
    static VideoThread *s_videoThread;
};

#endif // !VIDEOTHREAD_H
