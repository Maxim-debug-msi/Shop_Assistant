#include <QApplication>
#include <io.h>
#include <fcntl.h>
#include <string>
#include "UI/MainWindow.h"

int main(int argc, char *argv[]) {
    _setmode(_fileno(stdout), _O_U8TEXT);
    _setmode(_fileno(stdin), _O_U8TEXT);
    _setmode(_fileno(stderr), _O_U8TEXT);

    QApplication a(argc, argv);
    MainWindow window(nullptr);

    window.showMaximized();
    return QApplication::exec();
}
