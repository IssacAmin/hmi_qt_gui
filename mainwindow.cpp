#include "mainwindow.h"
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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Get the screen dimensions
    QScreen *screen = QGuiApplication::primaryScreen();
    QRect screenGeometry = screen->geometry();
    int screenWidth = screenGeometry.width();
    int screenHeight = screenGeometry.height();

    int windowWidth = screenWidth * 0.8;
    int windowHeight = screenHeight * 0.8;

    this->resize(windowWidth, windowHeight);
    this->move((screenWidth - windowWidth) / 2, (screenHeight - windowHeight) / 2); // Center the window

    // Set theme (Navy Blue Background with Gradient & Haze)
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(30, 30, 30));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(45, 45, 45));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    QApplication::setPalette(darkPalette);

    // Central Layout (for Home Page)
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    mainLayout->addSpacing(windowHeight * 0.15);

    // Clock Label
    timeLabel = new QLabel(centralWidget);
    QFont font("Arial", 36, QFont::Bold);
    timeLabel->setFont(font);
    timeLabel->setAlignment(Qt::AlignCenter);
    timeLabel->setStyleSheet("color: white;");
    mainLayout->addWidget(timeLabel);

    mainLayout->addSpacing(windowHeight * 0.10);

    // Button Grid
    QWidget *buttonContainer = new QWidget(centralWidget);
    QGridLayout *buttonLayout = new QGridLayout(buttonContainer);

    QPushButton *homeButton = createIconButton(":/icons/icons/call.png");
    QPushButton *mediaButton = createIconButton(":/icons/icons/media.png");
    QPushButton *settingsButton = createIconButton(":/icons/icons/settings.png");
    QPushButton *newsButton = createIconButton(":/icons/icons/bluetooth.png");
    QPushButton *weatherButton = createIconButton(":/icons/icons/weather.png");
    QPushButton *mapsButton = createIconButton(":/icons/icons/maps.png");

    buttonLayout->addWidget(homeButton, 0, 0);
    buttonLayout->addWidget(mediaButton, 0, 1);
    buttonLayout->addWidget(settingsButton, 0, 2);
    buttonLayout->addWidget(newsButton, 1, 0);
    buttonLayout->addWidget(weatherButton, 1, 1);
    buttonLayout->addWidget(mapsButton, 1, 2);

    buttonLayout->setVerticalSpacing(windowHeight * 0.05);
    buttonLayout->setHorizontalSpacing(0);
    buttonLayout->setContentsMargins(0, 0, 0, 0);

    buttonContainer->setLayout(buttonLayout);
    mainLayout->addWidget(buttonContainer);

    mainLayout->addSpacing(windowHeight * 0.40);

    // Home Page
    QWidget *homePage = new QWidget(this);
    homePage->setLayout(mainLayout);

    // Media Page
    QWidget *mediaPage = new QWidget(this);
    QVBoxLayout *mediaLayout = new QVBoxLayout(mediaPage);
    mediaLayout->addWidget(new QLabel("Media Page", mediaPage));

    QPushButton *backMediaButton = new QPushButton("Back to Home", mediaPage);
    mediaLayout->addWidget(backMediaButton);
    mediaLayout->addStretch();
    mediaPage->setStyleSheet("color: white; font-size: 24px;");

    // Settings Page
    QWidget *settingsPage = new QWidget(this);
    QVBoxLayout *settingsLayout = new QVBoxLayout(settingsPage);
    settingsLayout->addWidget(new QLabel("Settings Page", settingsPage));

    QPushButton *backSettingsButton = new QPushButton("Back to Home", settingsPage);
    settingsLayout->addWidget(backSettingsButton);
    settingsLayout->addStretch();
    settingsPage->setStyleSheet("color: white; font-size: 24px;");

    // News Page
    QWidget *newsPage = new QWidget(this);
    QVBoxLayout *newsLayout = new QVBoxLayout(newsPage);
    newsLayout->addWidget(new QLabel("News Page", newsPage));
    QPushButton *backNewsButton = new QPushButton("Back to Home", newsPage);
    newsLayout->addWidget(backNewsButton);
    newsLayout->addStretch();
    newsPage->setStyleSheet("color: white; font-size: 24px;");

    // Weather Page
    QWidget *weatherPage = new QWidget(this);
    QVBoxLayout *weatherLayout = new QVBoxLayout(weatherPage);
    weatherLayout->addWidget(new QLabel("Weather Page", weatherPage));
    QPushButton *backWeatherButton = new QPushButton("Back to Home", weatherPage);
    weatherLayout->addWidget(backWeatherButton);
    weatherLayout->addStretch();
    weatherPage->setStyleSheet("color: white; font-size: 24px;");

    // Maps Page
    QWidget *mapsPage = new QWidget(this);
    QVBoxLayout *mapsLayout = new QVBoxLayout(mapsPage);
    mapsLayout->addWidget(new QLabel("Maps Page", mapsPage));
    QPushButton *backMapsButton = new QPushButton("Back to Home", mapsPage);
    mapsLayout->addWidget(backMapsButton);
    mapsLayout->addStretch();
    mapsPage->setStyleSheet("color: white; font-size: 24px;");

    // Stack all pages
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(homePage);     // Index 0
    stackedWidget->addWidget(mediaPage);    // Index 1
    stackedWidget->addWidget(settingsPage); // Index 2
    stackedWidget->addWidget(newsPage);     // Index 3
    stackedWidget->addWidget(weatherPage);  // Index 4
    stackedWidget->addWidget(mapsPage);     // Index 5

    setCentralWidget(stackedWidget);

    // Connect buttons to corresponding pages
    connect(homeButton, &QPushButton::clicked, this, &MainWindow::goToHomePage);
    connect(mediaButton, &QPushButton::clicked, this, &MainWindow::showMediaPage);
    connect(settingsButton, &QPushButton::clicked, this, &MainWindow::showSettingsPage);
    connect(newsButton, &QPushButton::clicked, this, &MainWindow::showNewsPage);
    connect(weatherButton, &QPushButton::clicked, this, &MainWindow::showWeatherPage);
    connect(mapsButton, &QPushButton::clicked, this, &MainWindow::showMapsPage);

    // Connect back buttons to home page
    connect(backMediaButton, &QPushButton::clicked, this, &MainWindow::goToHomePage);
    connect(backSettingsButton, &QPushButton::clicked, this, &MainWindow::goToHomePage);
    connect(backNewsButton, &QPushButton::clicked, this, &MainWindow::goToHomePage);
    connect(backWeatherButton, &QPushButton::clicked, this, &MainWindow::goToHomePage);
    connect(backMapsButton, &QPushButton::clicked, this, &MainWindow::goToHomePage);

    // Timer for DateTime
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateDateTime);
    timer->start(1000);
    updateDateTime();
}

// Slot implementations for each page

void MainWindow::goToHomePage() {
    stackedWidget->setCurrentIndex(0); // Switch to Home Page (index 0)
}

void MainWindow::showMediaPage() {
    stackedWidget->setCurrentIndex(1); // Switch to Media Page
}

void MainWindow::showSettingsPage() {
    stackedWidget->setCurrentIndex(2); // Switch to Settings Page
}

void MainWindow::showNewsPage() {
    stackedWidget->setCurrentIndex(3); // Switch to News Page
}

void MainWindow::showWeatherPage() {
    stackedWidget->setCurrentIndex(4); // Switch to Weather Page
}

void MainWindow::showMapsPage() {
    stackedWidget->setCurrentIndex(5); // Switch to Maps Page
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
        "  border-bottom: 3px solid #00BFFF;"
        "}"
        "QPushButton:hover {"
        "  background-color: #404040;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #1F1F1F;"
        "  border-bottom: 3px solid #00CED1;"
        "}"
        );

    int buttonWidth = this->width() * 0.2;
    int buttonHeight = this->height() * 0.2;
    button->setFixedSize(buttonWidth, buttonHeight);

    return button;
}


void MainWindow::updateDateTime()
{
    QString dateTime = QDateTime::currentDateTime().toString("ddd, MMM dd yyyy - hh:mm AP");
    timeLabel->setText(dateTime);
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
    gradient.setColorAt(1, QColor(79, 51, 79));  // deep purplish gradient

    painter.setBrush(gradient);
    painter.setPen(Qt::NoPen);
    painter.drawRect(rect());

    QMainWindow::paintEvent(event);
}
