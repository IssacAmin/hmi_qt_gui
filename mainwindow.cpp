#include "mainwindow.h"
#include <QTimer>
#include <QDateTime>
#include <QMessageBox>
#include <QApplication>
#include <QPainter>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // Set theme (Dark Modern Look)
    QPalette darkPalette;
    darkPalette.setColor(QPalette::Window, QColor(30, 30, 30));
    darkPalette.setColor(QPalette::WindowText, Qt::white);
    darkPalette.setColor(QPalette::Button, QColor(45, 45, 45));
    darkPalette.setColor(QPalette::ButtonText, Qt::white);
    QApplication::setPalette(darkPalette);

    // Central Layout
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    // Navigation Bar
    mainLayout->addWidget(createNavBar());

    // Stack of Pages
    stackedWidget = new QStackedWidget(this);
    stackedWidget->addWidget(createHomePage());
    stackedWidget->addWidget(createMediaPage());
    stackedWidget->addWidget(createSettingsPage());

    mainLayout->addWidget(stackedWidget);

    setCentralWidget(centralWidget);

    // Timer for Updating Date and Time
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateDateTime);
    timer->start(1000);
    updateDateTime();
}

// Navigation Bar on the Left
QWidget* MainWindow::createNavBar()
{
    QWidget *navBar = new QWidget(this);
    navBar->setFixedWidth(200);

    QVBoxLayout *layout = new QVBoxLayout(navBar);

    QIcon homeIcon("/home/issac/hmi_gui/icons/home.png");
    QIcon mediaIcon("/home/issac/hmi_gui/icons/media.png");
    QIcon settingsIcon("/home/issac/hmi_gui/icons/settings.png");

    QPixmap pixmap1 = homeIcon.pixmap(100, 100);  // Create a pixmap with the icon
    QPainter painter(&pixmap1);
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn); // SourceIn will apply color filter
    painter.fillRect(pixmap1.rect(), QColor(255, 255, 255));  // White color
    painter.end();

    QPixmap pixmap2 = mediaIcon.pixmap(100, 100);  // Create a pixmap with the icon
    QPainter painter2(&pixmap2);
    painter2.setCompositionMode(QPainter::CompositionMode_SourceIn); // SourceIn will apply color filter
    painter2.fillRect(pixmap2.rect(), QColor(255, 255, 255));  // White color
    painter2.end();

    QPixmap pixmap3 = settingsIcon.pixmap(100, 100);  // Create a pixmap with the icon
    QPainter painter3(&pixmap3);
    painter3.setCompositionMode(QPainter::CompositionMode_SourceIn); // SourceIn will apply color filter
    painter3.fillRect(pixmap3.rect(), QColor(255, 255, 255));  // White color
    painter3.end();
    // Create Buttons

    QPushButton *homeButton = new QPushButton();
    QPushButton *mediaButton = new QPushButton();
    QPushButton *settingsButton = new QPushButton();

    homeButton->setIcon(QIcon(pixmap1));
    mediaButton->setIcon(QIcon(pixmap2));
    settingsButton->setIcon(QIcon(pixmap3));

    // Optional: Set icon size to ensure they fit nicely
    homeButton->setIconSize(QSize(30, 30));  // Adjust size as needed
    mediaButton->setIconSize(QSize(30, 30));
    settingsButton->setIconSize(QSize(30, 30));

    // Transparent Button Style
    QString buttonStyle = "QPushButton {"
                          "  font-size: 18px; "
                          "  padding: 10px; "
                          "  background-color: rgba(66, 66, 66, 180); " // Adds transparency
                          "  color: white; "
                          "  border: none; "
                          "  border-radius: 15px; "
                          "  text-align: center; "  // Ensure icon is centered
                          "}"
                          "QPushButton:hover { background-color: rgba(97, 97, 97, 200); }";



    homeButton->setStyleSheet(buttonStyle);
    mediaButton->setStyleSheet(buttonStyle);
    settingsButton->setStyleSheet(buttonStyle);

    // Connect Buttons
    connect(homeButton, &QPushButton::clicked, this, &MainWindow::showHomePage);
    connect(mediaButton, &QPushButton::clicked, this, &MainWindow::showMediaPage);
    connect(settingsButton, &QPushButton::clicked, this, &MainWindow::showSettingsPage);

    // Stretch Buttons Vertically
    layout->addStretch(5);
    layout->addWidget(homeButton);
    layout->addStretch(1);
    layout->addWidget(mediaButton);
    layout->addStretch(1);
    layout->addWidget(settingsButton);
    layout->addStretch(5);

    navBar->setStyleSheet("background-color: #212121;");
    return navBar;
}

// Top Bar with Date and Time
/*QWidget* MainWindow::createTopBar()
{
    QWidget *topBar = new QWidget(this);
    QHBoxLayout *layout = new QHBoxLayout(topBar);
    layout->setAlignment(Qt::AlignCenter);

    timeLabel = new QLabel(topBar);
    QFont font("Arial", 20, QFont::Bold);
    timeLabel->setFont(font);
    timeLabel->setAlignment(Qt::AlignCenter);

    layout->addWidget(timeLabel);
    topBar->setFixedHeight(60);
    topBar->setStyleSheet("background-color: #212121; color: white;");

    return topBar;
}*/

// Home Page
QWidget* MainWindow::createHomePage()
{
    QWidget *homePage = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(homePage);
    layout->setAlignment(Qt::AlignCenter);


    // Centered Clock
    timeLabel = new QLabel(homePage);
    QFont font("Arial", 36, QFont::Bold);
    timeLabel->setFont(font);
    timeLabel->setAlignment(Qt::AlignCenter);
    timeLabel->setStyleSheet("color: white;");

    layout->addStretch(1);  // Push Clock to Center Vertically
    layout->addWidget(timeLabel);
    layout->addStretch(1);

    return homePage;
}

// Media Page
QWidget* MainWindow::createMediaPage()
{
    QWidget *mediaPage = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(mediaPage);



    QLabel *title = new QLabel("Media Player");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 22px; color: white;");
    layout->addWidget(title);

    QString buttonStyle = "QPushButton {"
                          "  font-size: 25px; "
                          "  padding: 10px; "
                          "  background-color: rgba(66, 66, 66, 180); " // Adds transparency
                          "  color: white; "
                          "  border: none; "
                          "  border-radius: 15px; "  // Adds rounded borders
                          "}"
                          "QPushButton:hover { background-color: rgba(97, 97, 97, 200); }";

    QPushButton *playButton = new QPushButton("Play Media");
    playButton->setFixedSize(300, 80);
    playButton->setStyleSheet(buttonStyle);

    layout->addStretch(1);
    layout->addWidget(playButton, 0, Qt::AlignCenter);
    layout->addStretch(1);

    return mediaPage;
}

// Settings Page
QWidget* MainWindow::createSettingsPage()
{
    QWidget *settingsPage = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(settingsPage);

    QLabel *title = new QLabel("Settings");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size: 22px; color: white;");
    layout->addWidget(title);

    QString buttonStyle = "QPushButton {"
                          "  font-size: 25px; "
                          "  padding: 10px; "
                          "  background-color: rgba(66, 66, 66, 180); " // Adds transparency
                          "  color: white; "
                          "  border: none; "
                          "  border-radius: 15px; "  // Adds rounded borders
                          "}"
                          "QPushButton:hover { background-color: rgba(97, 97, 97, 200); }";

    QPushButton *updateButton = new QPushButton("Check for Updates");
    updateButton->setFixedSize(300, 80);
    updateButton->setStyleSheet(buttonStyle);

    connect(updateButton, &QPushButton::clicked, this, &MainWindow::checkForUpdates);
    layout->addStretch(1);
    layout->addWidget(updateButton, 0, Qt::AlignCenter);

    layout->addStretch(1);

    return settingsPage;
}

// Slots for Navigation
void MainWindow::showHomePage()
{
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::showMediaPage()
{
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::showSettingsPage()
{
    stackedWidget->setCurrentIndex(2);
}

void MainWindow::checkForUpdates()
{
    QMessageBox::information(this, "Update", "Your system is up to date!");
}

// Update Date and Time
void MainWindow::updateDateTime()
{
    QString dateTime = QDateTime::currentDateTime().toString("ddd, MMM dd yyyy - hh:mm:ss AP");
    timeLabel->setText(dateTime);
}
