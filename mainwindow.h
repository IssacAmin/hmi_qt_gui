#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QDateTime>
#include <QMessageBox>
#include <QApplication>
#include <QPainter>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QGridLayout>
#include <QSpacerItem>
#include <QIcon>
#include <QScreen>
#include <QStackedWidget>
#include <QScrollArea>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPushButton>
#include <QDebug>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    void showSettingsPage();
    void showMediaPage();
    void checkForUpdates();
    void paintEvent(QPaintEvent *event);
    QPushButton* createIconButton(const QString &iconPath);
    QPushButton* createmediaButton();
    void goToHomePage();
    void showNewsPage();
    void showWeatherPage();
    void showMapsPage();
    void togglePlayPause();
    void playNextTrack();
    QPushButton*createWhiteIconButton(const QString &iconPath);
    void playPreviousTrack();
    QIcon createWhiteIcon(const QString &iconPath, const QSize &size);
    QWidget* createNavBar();
    QWidget* createHomePage();
    QWidget* createSettingsPage();
    QWidget* createMediaPage();
    QString getAssetPath(const QString &filename);
    QStackedWidget *stackedWidget;
    QLabel *timeLabel;
    QLabel *dateLabel;
    int windowWidth;
    int windowHeight;
    QPushButton* createButton(const QString &text);
    void updateDateTime();

private slots:


private:

};

#endif // MAINWINDOW_H
