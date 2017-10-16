#ifndef XPLAY_H
#define XPLAY_H

#include <QDialog>

namespace Ui {
    class PlayerMainWidget;
}

class XPlayDialog : public QDialog
{
    Q_OBJECT


public:
    explicit XPlayDialog(QWidget *parent = nullptr);

    ~XPlayDialog();

private:
    Ui::PlayerMainWidget *m_ui;

};













#endif // !XPLAY_H
