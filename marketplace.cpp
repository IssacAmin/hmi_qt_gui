#include "marketplace.h"
#include "spinner.h"

marketplace::marketplace(QObject *parent)
    : QObject(parent)
{
}



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
    navLayout->addWidget(homeButton);

    // --- Scrollable List Container (empty until data is received) ---
    QScrollArea *scrollArea = new QScrollArea();
    QWidget *listContainer = new QWidget();
    listLayout = new QVBoxLayout(listContainer);  // assign to member for later population

    scrollArea->setWidget(listContainer);
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet("background-color: transparent;");

    // Send async HTTP GET request to port 8080
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &marketplace::handleMarketplaceReply);
    QNetworkRequest request(QUrl("http://localhost:8080/marketplace"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    networkManager->get(request);


    // Add scroll and nav layouts
    mainLayout->addWidget(scrollArea);
    mainLayout->addLayout(navLayout);

    // --- Spinner Overlay ---
    spinnerOverlay = new QWidget(page);
    spinnerOverlay->setObjectName("spinnerOverlay");
    spinnerOverlay->setStyleSheet("#spinnerOverlay { background-color: rgba(0, 0, 0, 160); }");
    spinnerOverlay->setAttribute(Qt::WA_TranslucentBackground);
    spinnerOverlay->setAttribute(Qt::WA_NoSystemBackground);
    spinnerOverlay->setAttribute(Qt::WA_StyledBackground, true);
    spinnerOverlay->setAutoFillBackground(false);
    spinnerOverlay->setGeometry(page->rect());
    spinnerOverlay->raise();


    loadingSpinner = new SpinnerWidget(spinnerOverlay);
    loadingSpinner->setFixedSize(64, 64);
    loadingSpinner->move((spinnerOverlay->width() - loadingSpinner->width()) / 2,
                         (spinnerOverlay->height() - loadingSpinner->height()) / 2);
    loadingSpinner->show();
    spinnerOverlay->show();

    // Track resize events so overlay resizes with page
    page->installEventFilter(this);

    return page;

}

bool marketplace::eventFilter(QObject *watched, QEvent *event) {
    if (spinnerOverlay && watched == spinnerOverlay->parent() && event->type() == QEvent::Resize) {
        QWidget *page = qobject_cast<QWidget *>(watched);
        if (page) {
            spinnerOverlay->setGeometry(page->rect());
            if (loadingSpinner) {
                loadingSpinner->move((spinnerOverlay->width() - loadingSpinner->width()) / 2,
                                     (spinnerOverlay->height() - loadingSpinner->height()) / 2);
            }
        }
    }
    return QObject::eventFilter(watched, event);
}


void marketplace::handleMarketplaceReply(QNetworkReply *reply) {
    if (loadingSpinner) loadingSpinner->hide();
    if (spinnerOverlay) spinnerOverlay->hide();
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonParseError err;
        QJsonDocument doc = QJsonDocument::fromJson(response, &err);

        if (err.error == QJsonParseError::NoError) {
            populateMarketplace(doc);
        } else {
            QVBoxLayout *centerLayout = new QVBoxLayout;
            centerLayout->addStretch();

            QLabel *emptyLabel = new QLabel("No items available in the marketplace.");
            emptyLabel->setStyleSheet("color: white; font-size: 36px;");
            emptyLabel->setAlignment(Qt::AlignCenter);

            centerLayout->addWidget(emptyLabel);
            centerLayout->addStretch();
            listLayout->addLayout(centerLayout);
            return;
            qDebug() << "JSON parse error:" << err.errorString();
        }
    } else {
        QVBoxLayout *centerLayout = new QVBoxLayout;
        centerLayout->addStretch();

        QLabel *emptyLabel = new QLabel("No items available in the marketplace.");
        emptyLabel->setStyleSheet("color: white; font-size: 36px;");
        emptyLabel->setAlignment(Qt::AlignCenter);

        centerLayout->addWidget(emptyLabel);
        centerLayout->addStretch();
        listLayout->addLayout(centerLayout);
        return;
        qDebug() << "Network error:" << reply->errorString();
    }

    reply->deleteLater();
}


void marketplace::populateMarketplace(QJsonDocument doc) {
    QJsonObject rootObj = doc.object();
    QJsonArray items = rootObj.value("marketplace").toArray();

    // Clear previous items if any
    QLayoutItem *child;
    while ((child = listLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        delete child;
    }

    if (items.isEmpty()) {
        QVBoxLayout *centerLayout = new QVBoxLayout;
        centerLayout->addStretch();

        QLabel *emptyLabel = new QLabel("No items available in the marketplace.");
        emptyLabel->setStyleSheet("color: white; font-size: 36px;");
        emptyLabel->setAlignment(Qt::AlignCenter);

        centerLayout->addWidget(emptyLabel);
        centerLayout->addStretch();
        listLayout->addLayout(centerLayout);
        return;
    }

    for (const QJsonValue &val : items) {
        QJsonObject obj = val.toObject();
        QString name = obj.value("name").toString();
        QString price = obj.value("price").toString();
        QString id = obj.value("id").toString();
        bool installed =obj.value("installed").toBool();

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
            installButton->setStyleSheet("background-color: grey; color: white; border: none; border-radius: 4px;");
        } else if (spinnerMap.contains(name)) {
            installButton->setText("");
            installButton->setEnabled(false);
            showSpinnerOnButton(installButton, name);
        } else {
            installButton->setText("Install");
            installButton->setStyleSheet(
                "QPushButton { background-color: #4CAF50; color: white; border: none; padding: 6px 12px; border-radius: 4px; }"
                "QPushButton:hover { background-color: #45a049; }");
        }

        connect(installButton, &QPushButton::clicked, this, [this, installButton, id, name]() {
            showSpinnerOnButton(installButton, id);
            emit installRequested(id, name);
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
