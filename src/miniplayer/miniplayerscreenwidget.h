#ifndef MINIPLAYERSCREENWIDGET_H
#define MINIPLAYERSCREENWIDGET_H

#include <QOpenGLWidget>

class MiniPlayerScreenWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit MiniPlayerScreenWidget(QWidget *parent = nullptr);
    virtual ~MiniPlayerScreenWidget() override;
    virtual void paintEvent(QPaintEvent *e) override;
    virtual void timerEvent(QTimerEvent *e) override;

    void setIsDilplayMode(int mode) { m_isDilplayMode = mode; }
    int  getIsDilplayMode() { return m_isDilplayMode; }

private:
    bool m_isDilplayMode = false;
};

#endif