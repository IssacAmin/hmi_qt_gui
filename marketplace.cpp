#include <QObject>
#include <QPushButton>
#include "marketplace.h"
#include "mainwindow.h"
#include <QLabel>
#include <QVariant>
#include <QMovie>
#include "spinner.h"

marketplace::marketplace(QObject *parent)
    : QObject(parent)
{}



    QWidget* marketplace::createMarketplacePage(){

        // Get the screen dimensions
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

        // --- Scrollable List Placeholder --
        QScrollArea *scrollArea = new QScrollArea();
        QWidget *listContainer = new QWidget();
        QVBoxLayout *listLayout = new QVBoxLayout(listContainer);

        QString jsonPath = QCoreApplication::applicationDirPath() + "/json/marketplace.json";
        installedFeaturesPath = QCoreApplication::applicationDirPath() + "/json/installed_features.json";
        QFile file(jsonPath);
        QFile installed_features_file(installedFeaturesPath);
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

            // Add vertical stretch above the label
            centerLayout->addStretch();

            QLabel *emptyLabel = new QLabel("No items available in the marketplace.");
            emptyLabel->setStyleSheet("color: white; font-size: 36px;");
            emptyLabel->setAlignment(Qt::AlignCenter);

            // Add the label
            centerLayout->addWidget(emptyLabel);

            // Add vertical stretch below the label
            centerLayout->addStretch();

            // Clear the existing layout if needed (optional)
            QLayoutItem *child;
            while ((child = listLayout->takeAt(0)) != nullptr) {
                delete child->widget();  // removes existing buttons
                delete child;
            }

            // Add the new centered layout
            listLayout->addLayout(centerLayout);
        }

        for (const QJsonValue &val : items) {
            QJsonObject obj = val.toObject();
            QString name = obj.value("name").toString();
            QString price = obj.value("price").toString();



            bool installed = false;

            if (installed_features_file.open(QIODevice::ReadOnly)) {
                QByteArray installedData = installed_features_file.readAll();
                installed_features_file.close();

                QJsonParseError installedParseError;
                QJsonDocument installedDoc = QJsonDocument::fromJson(installedData, &installedParseError);
                QJsonObject rootObj = installedDoc.object();
                QJsonArray items = rootObj.value("features").toArray();

                if (installedParseError.error == QJsonParseError::NoError) {
                    QJsonArray installedArray = installedDoc.array();
                    for (const QJsonValue &val : items) {
                        QJsonObject installedObj = val.toObject();
                        if (installedObj.contains("name") && installedObj.value("name").toString() == name) {
                            installed = installedObj.value("installed").toBool();
                            break;
                        }
                    }
                } else {
                    qWarning() << "Installed features parse error:" << installedParseError.errorString();
                }
            } else {
                qWarning() << "Failed to open installed features file:" << installedFeaturesPath;
            }



            QWidget *itemWidget = new QWidget;
            itemWidget->setStyleSheet("background: transparent; padding: 10px;");

            QHBoxLayout *itemLayout = new QHBoxLayout;
            // Left column: name and price
            QVBoxLayout *textLayout = new QVBoxLayout;
            QLabel *nameLabel = new QLabel(name);
            nameLabel->setStyleSheet("color: white; font-size: 18px; font-weight: bold;");
            QLabel *priceLabel = new QLabel(price);
            priceLabel->setStyleSheet("color: white; font-size: 14px;");

            textLayout->addWidget(nameLabel);
            textLayout->addWidget(priceLabel);
            textLayout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

            // Right: Install button
            QPushButton *installButton = new QPushButton;
            if (installed) {
                installButton->setFixedSize(windowHeight * 0.1, windowWidth * 0.03);
                installButton->setText("Installed");
                installButton->setEnabled(false);
                installButton->setStyleSheet(
                    "QPushButton {"
                    "  background-color: grey;"
                    "  color: white;"
                    "  border: none;"
                    "  padding: 6px 12px;"
                    "  border-radius: 4px;"
                    "}"
                    );
            } else {
                installButton->setFixedSize(windowHeight * 0.1, windowWidth * 0.03);
                installButton->setText("Install");
                installButton->setStyleSheet(
                    "QPushButton {"
                    "  background-color: #4CAF50;"
                    "  color: white;"
                    "  border: none;"
                    "  padding: 6px 12px;"
                    "  border-radius: 4px;"
                    "}"
                    "QPushButton:hover {"
                    "  background-color: #45a049;"
                    "}"
                    );
            }
            connect(installButton, &QPushButton::clicked, this, [this, installButton, name]() {
                showSpinnerOnButton(installButton);
                emit installRequested(name);
            });

            featureButtons[name] = installButton;

            // Add both to the item layout
            itemLayout->addLayout(textLayout);
            itemLayout->addStretch();  // pushes button to the right
            itemLayout->addWidget(installButton);

            // Apply layout to the item container widget
            itemWidget->setLayout(itemLayout);

            // Add item widget to the list
            listLayout->addWidget(itemWidget);

            QFrame *separator = new QFrame();
            separator->setFrameShape(QFrame::HLine);
            separator->setFrameShadow(QFrame::Plain);
            separator->setStyleSheet("color: white; background-color: white; height: 1px;");
            separator->setFixedHeight(1);

            listLayout->addWidget(separator);


        }

        setupFileWatcher();

        listContainer->setLayout(listLayout);
        scrollArea->setWidget(listContainer);
        scrollArea->setWidgetResizable(true);
        scrollArea->setStyleSheet("background-color: transparent;");

        mainLayout->addWidget(scrollArea);
        navLayout->addWidget(homeButton);
        mainLayout->addLayout(navLayout);

        return page;
    }



    void marketplace::showSpinnerOnButton(QPushButton *button) {
        button->setEnabled(false);
        button->setText("");

        SpinnerWidget *spinner = new SpinnerWidget(button);
        spinner->move((button->width() - spinner->width()) / 2,
                      (button->height() - spinner->height()) / 2);
        spinner->show();

        // Store spinner so we can remove it later
        QString name = featureButtons.key(button);
        if (!name.isEmpty()) {
            spinnerMap[name] = spinner;
        }
    }


    void marketplace::setupFileWatcher() {
        if (!fileWatcher) {
            fileWatcher = new QFileSystemWatcher(this);
            connect(fileWatcher, &QFileSystemWatcher::fileChanged, this, [this]() {
                // qDebug() << "installed_features.json changed!";
                updateInstalledStates();

                // QFileSystemWatcher may stop watching if the file was overwritten
                QTimer::singleShot(100, this, [this]() {
                    if (!fileWatcher->files().contains(installedFeaturesPath)) {
                        fileWatcher->addPath(installedFeaturesPath);
                    }
                });
            });
        }

        if (!fileWatcher->files().contains(installedFeaturesPath)) {
            fileWatcher->addPath(installedFeaturesPath);
        }

        updateInstalledStates();  // initial state
    }


    void marketplace::updateInstalledStates() {
        QFile installedFile(installedFeaturesPath);
        if (!installedFile.open(QIODevice::ReadOnly)) {
            qWarning() << "Failed to open installed features file:" << installedFeaturesPath;
            return;
        }

        QByteArray data = installedFile.readAll();
        installedFile.close();

        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(data, &error);
        if (error.error != QJsonParseError::NoError) {
            qWarning() << "Failed to parse installed_features.json:" << error.errorString();
            return;
        }

        QSet<QString> installedSet;
        QJsonObject root = doc.object();
        QJsonArray features = root.value("features").toArray();
        for (const QJsonValue &val : features) {
            QJsonObject obj = val.toObject();
            if (obj.contains("name") && obj["installed"].toBool()) {
                installedSet.insert(obj["name"].toString());
            }
        }

        for (const QString &name : featureButtons.keys()) {
            QPushButton *btn = featureButtons[name];
            if (!btn) continue;

            if (installedSet.contains(name)) {
                if (spinnerMap.contains(name)) {
                    SpinnerWidget *spinner = spinnerMap.take(name);
                    if (spinner) {
                        spinner->deleteLater();
                    }
                }

                btn->setText("Installed");
                btn->setEnabled(false);
                btn->setStyleSheet(
                    "QPushButton {"
                    "  background-color: grey;"
                    "  color: white;"
                    "  border: none;"
                    "  padding: 6px 12px;"
                    "  border-radius: 4px;"
                    "}"
                    );
            } else {
                if (spinnerMap.contains(name)) {
                    SpinnerWidget *spinner = spinnerMap.take(name);
                    if (spinner) {
                        spinner->deleteLater();
                    }
                }

                btn->setText("Install");
                btn->setEnabled(true);
                btn->setStyleSheet(
                    "QPushButton {"
                    "  background-color: #4CAF50;"
                    "  color: white;"
                    "  border: none;"
                    "  padding: 6px 12px;"
                    "  border-radius: 4px;"
                    "}"
                    "QPushButton:hover {"
                    "  background-color: #45a049;"
                    "}"
                    );
            }

        }
    }





