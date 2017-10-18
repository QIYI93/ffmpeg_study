#ifndef XPLAYERSCREENWIDGET_H
#define XPLAYERSCREENWIDGET_H

#include <QOpenGLWidget>

class XPlayerScreenWidget : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit XPlayerScreenWidget(QWidget *parent = nullptr);
    virtual ~XPlayerScreenWidget() override;
    virtual void paintEvent(QPaintEvent *e) override;
    virtual void timerEvent(QTimerEvent *e) override;

    void setIsDilplayMode(int mode) { m_isDilplayMode = mode; }
    int  getIsDilplayMode() { return m_isDilplayMode; }

private:
    bool m_isDilplayMode = false;
};

#endif