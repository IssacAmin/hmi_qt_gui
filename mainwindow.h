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

private slots:


private:
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
};

#endif // MAINWINDOW_H
