#include "mainwindow.h"
#include "marketplace.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    // Get the screen dimensions
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    windowWidth = screenWidth;
    windowHeight = screenHeight;

    this->resize(windowWidth, windowHeight);
    this->move((screenWidth - windowWidth) / 2, (screenHeight - windowHeight) / 2);

    // Set theme
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(30, 30, 30));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(45, 45, 45));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    QApplication::setPalette(darkPalette);

    // Central Widget
    QWidget *centralWidget = new QWidget(this);

    // Time and Date Labels
    timeLabel = new QLabel();
    QFont font("Arial", 45, QFont::Bold);
    timeLabel->setFont(font);
    timeLabel->setStyleSheet("color: white;");

    dateLabel = new QLabel();
    dateLabel->setFont(font);
    dateLabel->setStyleSheet("color: white;");

    // Layout for Time and Date
    QVBoxLayout *timeLayout = new QVBoxLayout();
    timeLayout->addWidget(dateLabel);
    timeLayout->addWidget(timeLabel);
    timeLayout->setAlignment(Qt::AlignCenter);

    // Buttons
    QPushButton *callButton = createIconButton(":/icons/icons/call.png");
    QPushButton *mediaButton = createmediaButton();
    QPushButton *settingsButton = createIconButton(":/icons/icons/settings.png");
    QPushButton *bluetoothButton = createIconButton(":/icons/icons/bluetooth.png");
    QPushButton *marketPlaceButton = createIconButton(":/icons/icons/marketplace.png");
    QPushButton *mapsButton = createIconButton(":/icons/icons/maps.png");
    QPushButton *weatherButton = createIconButton(":/icons/icons/weather.png");

    // Layout for Buttons
    QVBoxLayout *buttonsLayout = new QVBoxLayout();

    // First row of buttons
    QHBoxLayout *row1 = new QHBoxLayout();
    row1->addLayout(timeLayout);
    row1->addWidget(mediaButton);

    // Second row of buttons
    QHBoxLayout *row2 = new QHBoxLayout();
    row2->addWidget(callButton);
    row2->addWidget(weatherButton);
    row2->addWidget(settingsButton);
    row2->setSpacing(windowWidth *0.02);

    // Third row of buttons
    QHBoxLayout *row3 = new QHBoxLayout();
    row3->addWidget(mapsButton);
    row3->addWidget(bluetoothButton);
    row3->addWidget(marketPlaceButton);
    row3->setSpacing(windowWidth *0.02);


    // Add rows to the buttons layout
    buttonsLayout->addLayout(row1);
    buttonsLayout->addLayout(row2);
    buttonsLayout->addLayout(row3);


    // Create stacked widget and pages
    stackedWidget = new QStackedWidget(this);
    QWidget *homePage = new QWidget();
    QWidget *mediaPage = new QLabel("Media Page");
    QWidget *settingsPage = new QLabel("Settings Page");
    QWidget *weatherPage = new QLabel("Weather Page");
    QWidget *mapsPage = new QLabel("Maps Page");
    marketplace *mp = new marketplace(this);
    connect(mp, &marketplace::homeButtonClicked, this, &MainWindow::goToHomePage);
    connect(mp, &marketplace::installRequested,
            this, &MainWindow::handleInstallRequest);

    QWidget *marketplacePage = mp->createMarketplacePage();


    // Set up layout of homePage with your buttons
    homePage->setLayout(buttonsLayout);  // reuse your existing layout

    // Add pages to stacked widget
    stackedWidget->addWidget(homePage);        // index 0
    stackedWidget->addWidget(mediaPage);       // index 1
    stackedWidget->addWidget(settingsPage);    // index 2
    stackedWidget->addWidget(weatherPage);     // index 3
    stackedWidget->addWidget(mapsPage);        // index 4
    stackedWidget->addWidget(marketplacePage); // index 5

    // Layout stacking and navigation
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addWidget(stackedWidget);
    centralWidget->setLayout(mainLayout);

    // Timer for DateTime
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateDateTime);
    timer->start(1000);
    updateDateTime();

    connect(mediaButton, &QPushButton::clicked, this, [=]() {
        stackedWidget->setCurrentIndex(1);
    });
    connect(settingsButton, &QPushButton::clicked, this, [=]() {
        stackedWidget->setCurrentIndex(2);
    });
    connect(weatherButton, &QPushButton::clicked, this, [=]() {
        stackedWidget->setCurrentIndex(3);
    });
    connect(mapsButton, &QPushButton::clicked, this, [=]() {
        stackedWidget->setCurrentIndex(4);
    });
    connect(marketPlaceButton, &QPushButton::clicked, this, [=]() {
        stackedWidget->setCurrentIndex(5);
    });

    stackedWidget->setCurrentIndex(0);
    setCentralWidget(centralWidget);

}

void MainWindow::updateDateTime()
{
    QString dateTime = QDateTime::currentDateTime().toString("ddd, MMM dd yyyy");
    QString time = QDateTime::currentDateTime().toString("hh:mm AP");
    dateLabel->setText(dateTime);
    timeLabel->setText(time);
}

QPushButton* MainWindow::createIconButton(const QString &iconPath)
{
    QPushButton *button = new QPushButton();
    QIcon buttonIcon(iconPath);
    button->setIcon(buttonIcon);

    int iconSize = qMin(this->width(), this->height()) * 0.15;
    button->setIconSize(QSize(iconSize, iconSize));

    button->setStyleSheet(
        "QPushButton {"
        "  background-color: #2f2236;"
        "  border: none;"
        "  padding: 10px;"
        "  color: white;"
        "  font-size: 18px;"
        "  border-radius: 20px"
        "}"
        "QPushButton:hover {"
        "  background-color: #404040;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #1F1F1F;"
        "  border-bottom: 3px solid #00CED1;"
        "}"
        );

    return button;
}

QPushButton* MainWindow::createmediaButton()
{
    // Main media button that will hold the inner buttons
    QPushButton *button = new QPushButton();
    button->setMinimumHeight(windowHeight * 0.4);
    button->setStyleSheet(
        "QPushButton {"
        "  border: none;"
        "  border-radius: 20px;"
        "  background: transparent;"
        "}"
        );


    QPushButton *playPauseButton = createWhiteIconButton(":/icons/icons/play.png");
    QPushButton *nextButton = createWhiteIconButton(":/icons/icons/next.png");
    QPushButton *prevButton = createWhiteIconButton(":/icons/icons/previous.png");



    QVBoxLayout *mediaLayout = new QVBoxLayout();
    QHBoxLayout *innerLayout = new QHBoxLayout();
    innerLayout->addWidget(prevButton);
    innerLayout->addWidget(playPauseButton);
    innerLayout->addWidget(nextButton);

    QLabel *songLabel = new QLabel();
    QFont font("Arial", 25, QFont::Bold);
    songLabel->setFont(font);
    songLabel->setStyleSheet("color: white;");
    songLabel->setText("Song Name...");

    mediaLayout->setAlignment(Qt::AlignCenter);
    mediaLayout->addWidget(songLabel);
    mediaLayout->addLayout(innerLayout);

    QWidget *mediaWidget = new QWidget();
    mediaWidget->setLayout(mediaLayout);


    button->setLayout(new QVBoxLayout());
    button->layout()->addWidget(mediaWidget);


    connect(playPauseButton, &QPushButton::clicked, this, &MainWindow::togglePlayPause);
    connect(nextButton, &QPushButton::clicked, this, &MainWindow::playNextTrack);
    connect(prevButton, &QPushButton::clicked, this, &MainWindow::playPreviousTrack);

    return button;
}



QPushButton* MainWindow::createWhiteIconButton(const QString &iconPath)
{
    QPushButton *button = new QPushButton();

    // Load and recolor icon to white
    QPixmap pixmap(iconPath);
    QImage image = pixmap.toImage().convertToFormat(QImage::Format_ARGB32);

    for (int y = 0; y < image.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(image.scanLine(y));
        for (int x = 0; x < image.width(); ++x) {
            QColor originalColor = QColor::fromRgba(line[x]);
            int alpha = originalColor.alpha();
            line[x] = QColor(255, 255, 255, alpha).rgba();
        }
    }

    QPixmap whitePixmap = QPixmap::fromImage(image);

    // Scale the pixmap to desired icon size
    QSize iconSize(windowHeight* 0.07,windowHeight* 0.07); // desired icon size
    QPixmap scaledPixmap = whitePixmap.scaled(iconSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    button->setIcon(QIcon(scaledPixmap));
    button->setIconSize(iconSize);
    button->setFixedSize(windowHeight* 0.25, windowHeight* 0.25); // button size


    // Apply flat icon-only style
    button->setStyleSheet(
        "QPushButton {"
        "  border: none;"
        "  background: transparent;"
        "  padding: 0;"
        "}"
        );

    return button;
}

QIcon MainWindow::createWhiteIcon(const QString &iconPath, const QSize &size)
{
    QPixmap pixmap(iconPath);
    QImage image = pixmap.toImage().convertToFormat(QImage::Format_ARGB32);

    for (int y = 0; y < image.height(); ++y) {
        QRgb *line = reinterpret_cast<QRgb *>(image.scanLine(y));
        for (int x = 0; x < image.width(); ++x) {
            QColor originalColor = QColor::fromRgba(line[x]);
            int alpha = originalColor.alpha();
            line[x] = QColor(255, 255, 255, alpha).rgba();
        }
    }

    QPixmap whitePixmap = QPixmap::fromImage(image).scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    return QIcon(whitePixmap);
}

void MainWindow::togglePlayPause() {
    static bool isPlaying = false;
    isPlaying = !isPlaying;

    QString iconPath = isPlaying ? ":/icons/icons/pause.png" : ":/icons/icons/play.png";
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        button->setIcon(createWhiteIcon(iconPath, QSize(windowHeight* 0.07,windowHeight* 0.07)));
    }
}

void MainWindow::playNextTrack() {
    //TODO: Logic for playing the next track
    qDebug() << "Next track";
}

void MainWindow::playPreviousTrack() {
    //TODO: Logic for playing the previous track
    qDebug() << "Previous track";
}





// Slot implementations for each page

void MainWindow::goToHomePage() {
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::showMediaPage() {
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::showSettingsPage() {
    stackedWidget->setCurrentIndex(2);
}

void MainWindow::showNewsPage() {
    stackedWidget->setCurrentIndex(3);
}

void MainWindow::showWeatherPage() {
    stackedWidget->setCurrentIndex(4);
}

void MainWindow::showMapsPage() {
    stackedWidget->setCurrentIndex(5);
}



void MainWindow::checkForUpdates() {
    QMessageBox::information(this, "Update", "Your system is up to date!");
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPointF center(width() / 2, height() / 2);
    qreal radius = qMin(width(), height()) / 2;

    QRadialGradient gradient(center, radius * 5);
    gradient.setColorAt(0, QColor(10, 10, 10));
    gradient.setColorAt(1, QColor(79, 51, 79));

    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());

    QMainWindow::paintEvent(event);
}



void MainWindow::handleInstallRequest(const QString &featureName) {
    QString path = QCoreApplication::applicationDirPath() + "/json/requests.json";

    QFile file(path);
    QJsonArray requestArray;

    if (file.exists() && file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        file.close();

        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(data, &error);
        if (error.error == QJsonParseError::NoError && doc.isArray()) {
            requestArray = doc.array();
        }
    }

    bool featurefound = false;
    for (const QJsonValue &val : requestArray) {
        QJsonObject reqObj = val.toObject();
        if (reqObj.contains("name") && reqObj.value("name").toString() == featureName) {
            featurefound = true;
            break;
        }
    }

    if(!featurefound){
        // Add new item to array
        QJsonObject newRequest;
        newRequest["name"] = featureName;
        newRequest["timestamp"] = QDateTime::currentDateTime().toString(Qt::ISODate);
        newRequest["status"] = "pending";
        requestArray.append(newRequest);

        // Write back to file
        if (file.open(QIODevice::WriteOnly)) {
            QJsonDocument saveDoc(requestArray);
            file.write(saveDoc.toJson());
            file.close();
            QMessageBox::information(this, "Feature Installing",
                                     QString("The feature '%1' will be installed shortly...").arg(featureName));
        } else {
            qWarning() << "Failed to open install_requests.json for writing.";
        }
    }else{
        QMessageBox::information(this, "Feature Installing",
                                 QString("PLease wait feature is currently getting installed..."));

    }

}

