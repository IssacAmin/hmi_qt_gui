#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle("Fusion"); // Use the Fusion style for modern UI look

    MainWindow window;
    window.setWindowTitle("Car Infotainment System");
    window.showFullScreen(); // Fullscreen for infotainment experience

    return app.exec();
}
