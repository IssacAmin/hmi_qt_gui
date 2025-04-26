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


#include "marketplace.h"
#include "spinner.h"

marketplace::marketplace(QObject *parent)
    : QObject(parent)
{
}

// Global spinner map is already a member variable:
// QMap<QString, SpinnerWidget*> spinnerMap;
// QMap<QString, QPushButton*> featureButtons;
// QString installedFeaturesPath;
// int windowWidth, windowHeight;

QWidget* marketplace::createMarketplacePage() {

    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    windowWidth = screenWidth;
    windowHeight = screenHeight;

    QWidget *page = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(page);

    // --- Top Nav Bar with Home Button ---
    QHBoxLayout *navLayout = new QHBoxLayout();

    QPushButton *homeButton = new QPushButton();
    QPixmap homePixmap(":/icons/icons/home.png");
    QImage image = homePixmap.toImage().convertToFormat(QImage::Format_ARGB32);

    for (int y = 0; y < image.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(image.scanLine(y));
        for (int x = 0; x < image.width(); ++x) {
            QColor originalColor = QColor::fromRgba(line[x]);
            int alpha = originalColor.alpha();
            line[x] = QColor(255, 255, 255, alpha).rgba(); // make white
        }
    }

    QPixmap whiteHomePixmap = QPixmap::fromImage(image).scaled(windowHeight * 0.05, windowHeight * 0.05, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    homeButton->setIcon(QIcon(whiteHomePixmap));
    homeButton->setIconSize(QSize(windowHeight * 0.05, windowHeight * 0.05));
    homeButton->setFixedSize(windowHeight * 0.07, windowHeight * 0.07);
    homeButton->setStyleSheet("QPushButton { border: none; background: transparent; padding: 0; }");

    QObject::connect(homeButton, &QPushButton::clicked, this, &marketplace::homeButtonClicked);

    // --- Scrollable List ---
    QScrollArea *scrollArea = new QScrollArea();
    QWidget *listContainer = new QWidget();
    QVBoxLayout *listLayout = new QVBoxLayout(listContainer);

    QString jsonPath = QCoreApplication::applicationDirPath() + "/json/marketplace.json";
    installedFeaturesPath = QCoreApplication::applicationDirPath() + "/json/installed_features.json";

    QFile file(jsonPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open JSON file:" << jsonPath;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "JSON parse error:" << parseError.errorString();
    }

    QJsonObject rootObj = doc.object();
    QJsonArray items = rootObj.value("marketplace").toArray();

    if (items.isEmpty()) {
        QVBoxLayout *centerLayout = new QVBoxLayout;
        centerLayout->addStretch();

        QLabel *emptyLabel = new QLabel("No items available in the marketplace.");
        emptyLabel->setStyleSheet("color: white; font-size: 36px;");
        emptyLabel->setAlignment(Qt::AlignCenter);

        centerLayout->addWidget(emptyLabel);
        centerLayout->addStretch();

        QLayoutItem *child;
        while ((child = listLayout->takeAt(0)) != nullptr) {
            delete child->widget();
            delete child;
        }
        listLayout->addLayout(centerLayout);
    }

    for (const QJsonValue &val : items) {
        QJsonObject obj = val.toObject();
        QString name = obj.value("name").toString();
        QString price = obj.value("price").toString();

        bool installed = isFeatureInstalled(name);

        QWidget *itemWidget = new QWidget;
        itemWidget->setStyleSheet("background: transparent; padding: 10px;");

        QHBoxLayout *itemLayout = new QHBoxLayout;
        QVBoxLayout *textLayout = new QVBoxLayout;

        QLabel *nameLabel = new QLabel(name);
        nameLabel->setStyleSheet("color: white; font-size: 18px; font-weight: bold;");
        QLabel *priceLabel = new QLabel(price);
        priceLabel->setStyleSheet("color: white; font-size: 14px;");

        textLayout->addWidget(nameLabel);
        textLayout->addWidget(priceLabel);
        textLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

        QPushButton *installButton = new QPushButton;
        installButton->setFixedSize(windowHeight * 0.1, windowWidth * 0.03);

        if (installed) {
            installButton->setText("Installed");
            installButton->setEnabled(false);
            installButton->setStyleSheet(
                "QPushButton { background-color: grey; color: white; border: none; padding: 6px 12px; border-radius: 4px; }"
                );
        } else if (spinnerMap.contains(name)) {
            installButton->setText("");
            installButton->setEnabled(false);
            showSpinnerOnButton(installButton, name);
        } else {
            installButton->setText("Install");
            installButton->setStyleSheet(
                "QPushButton { background-color: #4CAF50; color: white; border: none; padding: 6px 12px; border-radius: 4px; }"
                "QPushButton:hover { background-color: #45a049; }"
                );
        }

        connect(installButton, &QPushButton::clicked, this, [this, installButton, name]() {
            showSpinnerOnButton(installButton, name);
            emit installRequested(name);
        });

        featureButtons[name] = installButton;

        itemLayout->addLayout(textLayout);
        itemLayout->addStretch();
        itemLayout->addWidget(installButton);

        itemWidget->setLayout(itemLayout);
        listLayout->addWidget(itemWidget);

        QFrame *separator = new QFrame();
        separator->setFrameShape(QFrame::HLine);
        separator->setFrameShadow(QFrame::Plain);
        separator->setStyleSheet("color: white; background-color: white; height: 1px;");
        separator->setFixedHeight(1);

        listLayout->addWidget(separator);
    }

    listContainer->setLayout(listLayout);
    scrollArea->setWidget(listContainer);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("background-color: transparent;");

    mainLayout->addWidget(scrollArea);
    navLayout->addWidget(homeButton);
    mainLayout->addLayout(navLayout);

    return page;
}

void marketplace::showSpinnerOnButton(QPushButton *button, const QString &name) {
    button->setEnabled(false);
    button->setText("");

    if (spinnerMap.contains(name)) {
        SpinnerWidget *spinner = spinnerMap.value(name);
        spinner->setParent(button);
        spinner->move((button->width() - spinner->width()) / 2,
                      (button->height() - spinner->height()) / 2);
        spinner->show();
    } else {
        SpinnerWidget *spinner = new SpinnerWidget(button);
        spinner->move((button->width() - spinner->width()) / 2,
                      (button->height() - spinner->height()) / 2);
        spinner->show();
        spinnerMap[name] = spinner;
    }
}

bool marketplace::isFeatureInstalled(const QString &name) {
    QFile installed_features_file(installedFeaturesPath);
    if (!installed_features_file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open installed features file:" << installedFeaturesPath;
        return false;
    }

    QByteArray installedData = installed_features_file.readAll();
    installed_features_file.close();

    QJsonParseError installedParseError;
    QJsonDocument installedDoc = QJsonDocument::fromJson(installedData, &installedParseError);

    if (installedParseError.error != QJsonParseError::NoError) {
        qWarning() << "Installed features parse error:" << installedParseError.errorString();
        return false;
    }

    QJsonObject rootObj = installedDoc.object();
    QJsonArray features = rootObj.value("features").toArray();

    for (const QJsonValue &feature : features) {
        QJsonObject installedObj = feature.toObject();
        if (installedObj.contains("name") && installedObj.value("name").toString() == name) {
            return installedObj.value("installed").toBool();
        }
    }

    return false;
}

void marketplace::markFeatureAsInstalled(const QString &name) {
    if (featureButtons.contains(name)) {
        QPushButton *button = featureButtons[name];
        button->setEnabled(false);
        button->setText("Installed");
        button->setStyleSheet("QPushButton { background-color: grey; color: white; border: none; }");

        if (spinnerMap.contains(name)) {
            SpinnerWidget *spinner = spinnerMap[name];
            spinner->deleteLater();
            spinnerMap.remove(name);
        }
    }
}
