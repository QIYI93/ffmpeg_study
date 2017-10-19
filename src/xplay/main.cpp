#include <windows.h>
#include <QApplication>
#include <QDebug>

extern "C"
{
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

#include <filehelper.h>
#include "xffmpeg.h"
#include "xplayer.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    XPlayDialog playDlg;
    playDlg.show();
    return a.exec();
}
