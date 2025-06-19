#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QPushButton>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    QWidget* createSettingsPage();

signals:
    void homeButtonClicked();

private slots:
    void handleSettingsButtonPressed();
    void handleUpdateReply(QNetworkReply *reply);

private:
    int windowWidth;
    int windowHeight;
    QNetworkAccessManager *networkManager;
};

#endif // SETTINGS_H
