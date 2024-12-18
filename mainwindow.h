#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private slots:
    void showHomePage();
    void showSettingsPage();
    void showMediaPage();
    void checkForUpdates();

private:
    QWidget* createNavBar();
    QWidget* createHomePage();
    QWidget* createSettingsPage();
    QWidget* createMediaPage();

    QStackedWidget *stackedWidget;
    QLabel *timeLabel;

    void updateDateTime();
};

#endif // MAINWINDOW_H
