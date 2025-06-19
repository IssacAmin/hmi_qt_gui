#include "settings.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScreen>
#include <QGuiApplication>
#include <QPixmap>
#include <QImage>
#include <QColor>
#include <QLabel>
#include <QIcon>
#include <QNetworkRequest>
#include <QUrl>
#include <QDebug>
#include "QMessageBox"

Settings::Settings(QWidget *parent)
    : QWidget(parent)
{
    // Get screen dimensions
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    windowWidth = screenGeometry.width();
    windowHeight = screenGeometry.height();

    // Network manager setup
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &Settings::handleUpdateReply);

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(50, 50, 50, 50);
    mainLayout->setSpacing(20);
    mainLayout->setAlignment(Qt::AlignCenter);

    // Centered action button
    QPushButton *centerButton = new QPushButton("Start ECU Update", this);
    centerButton->setFixedSize(windowWidth * 0.3, windowHeight * 0.1);
    centerButton->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; font-size: 24px; "
        "border: none; border-radius: 10px; padding: 10px; }"
        "QPushButton:hover { background-color: #45a049; }");

    mainLayout->addStretch();
    mainLayout->addWidget(centerButton, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    // Bottom home button
    QPushButton *homeButton = new QPushButton(this);
    QPixmap homePixmap(":/icons/icons/home.png");
    QImage image = homePixmap.toImage().convertToFormat(QImage::Format_ARGB32);

    for (int y = 0; y < image.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(image.scanLine(y));
        for (int x = 0; x < image.width(); ++x) {
            QColor color = QColor::fromRgba(line[x]);
            int alpha = color.alpha();
            line[x] = QColor(255, 255, 255, alpha).rgba(); // white icon
        }
    }

    QPixmap whitePixmap = QPixmap::fromImage(image).scaled(windowHeight * 0.05, windowHeight * 0.05, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    homeButton->setIcon(QIcon(whitePixmap));
    homeButton->setIconSize(QSize(windowHeight * 0.05, windowHeight * 0.05));
    homeButton->setFixedSize(windowHeight * 0.07, windowHeight * 0.07);
    homeButton->setStyleSheet("QPushButton { border: none; background: transparent; padding: 0; }");

    connect(homeButton, &QPushButton::clicked, this, &Settings::homeButtonClicked);
    connect(centerButton, &QPushButton::clicked, this, &Settings::handleSettingsButtonPressed);

    QHBoxLayout *bottomLayout = new QHBoxLayout;
    bottomLayout->addStretch();
    bottomLayout->addWidget(homeButton);
    bottomLayout->addStretch();

    mainLayout->addLayout(bottomLayout);
}

void Settings::handleSettingsButtonPressed()
{
    QUrl url("http://localhost:8080/update");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    networkManager->get(request);
    QMessageBox::information(this, "ECU update",
                             QString("ECU is currently getting updated."));
}

void Settings::handleUpdateReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        qDebug() << "Update reply:" << QString(response);
    } else {
        qDebug() << "Error in update request:" << reply->errorString();
    }

    reply->deleteLater();
}
