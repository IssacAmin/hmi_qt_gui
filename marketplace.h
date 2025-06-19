#ifndef MARKETPLACE_H
#define MARKETPLACE_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QMap>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QObject>
#include <QPushButton>
#include <QScreen>
#include <QGuiApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QLabel>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QCoreApplication>
#include <QPixmap>
#include <QImage>
#include <QColor>
#include <QFrame>
class SpinnerWidget;

class marketplace : public QObject
{
    Q_OBJECT

public:
    explicit marketplace(QObject *parent = nullptr);
    QWidget* createMarketplacePage();

signals:
    void homeButtonClicked();
    void installRequested(const QString &id, const QString &featureName);

private slots:
    void handleMarketplaceReply(QNetworkReply *reply);
private:
    QWidget *marketplacePage = nullptr;
private:
    SpinnerWidget *loadingSpinner = nullptr;
    QWidget *spinnerOverlay = nullptr;
    void populateMarketplace(QJsonDocument doc);

    QNetworkAccessManager *networkManager = nullptr;
    QVBoxLayout *listLayout = nullptr;

    void showSpinnerOnButton(QPushButton *button, const QString &name);
    bool isFeatureInstalled(const QString &name);

    QMap<QString, SpinnerWidget*> spinnerMap;
    QMap<QString, QPushButton*> featureButtons;

    int windowWidth;
    int windowHeight;
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
};

#endif // MARKETPLACE_H


