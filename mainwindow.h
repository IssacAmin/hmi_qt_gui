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
    QPushButton* createLargeButton(const QString &text, const QString &iconName);
    QPushButton* createIconButton(const QString &iconPath);
    void goToHomePage();
    void showNewsPage();
    void showWeatherPage();
    void showMapsPage();

private slots:


private:
    QWidget* createNavBar();
    QWidget* createHomePage();
    QWidget* createSettingsPage();
    QWidget* createMediaPage();
    QString getAssetPath(const QString &filename);
    QStackedWidget *stackedWidget;
    QLabel *timeLabel;
    QPushButton* createButton(const QString &text);

    void updateDateTime();
};

#endif // MAINWINDOW_H
