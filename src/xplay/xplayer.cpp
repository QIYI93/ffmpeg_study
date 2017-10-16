#include "xplayer.h"
#include "ui_PlayerMainWidget.h"

#include <QIcon>

XPlayDialog::XPlayDialog(QWidget *parent)
    :QDialog(parent)
    , m_ui(new Ui::PlayerMainWidget)
{
    m_ui->setupUi(this);
    setWindowTitle("XPlayer");
    setWindowIcon(QIcon(":/res/images/XPlayer.ico"));
}

XPlayDialog::~XPlayDialog()
{
}
