#include <windows.h>
#include <QApplication>
#include <QDebug>
extern "C"
{
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

#include <filehelper.h>
#include "qffmpeg.h"
#include "miniplayer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MiniPlayerDialog playerDlg;
    playerDlg.show();
    return a.exec();
}
