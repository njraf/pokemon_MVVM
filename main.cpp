#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QFile styles(":/styles.qss");
    if (styles.open(QFile::ReadOnly)) {
        w.setStyleSheet(QLatin1String(styles.readAll()));
    }
    w.show();
    return a.exec();
}
