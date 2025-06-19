#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle("Fusion");
    MainWindow window;
    window.setWindowTitle("Car Infotainment System");
    window.showFullScreen();

    return app.exec();
}
