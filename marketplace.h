#ifndef MARKETPLACE_H
#define MARKETPLACE_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QFileSystemWatcher>
#include <QMap>
#include "spinner.h"

class marketplace : public QObject {
    Q_OBJECT

public:
    explicit marketplace(QObject *parent = nullptr);
    QWidget* createMarketplacePage();
    int windowWidth;
    int windowHeight;
    void showSpinnerOnButton(QPushButton *button);
    QFileSystemWatcher* fileWatcher = nullptr;
    QMap<QString, QPushButton*> featureButtons;
    void setupFileWatcher();
    void updateInstalledStates();
    QMap<QString, SpinnerWidget*> spinnerMap;
private:
    QString installedFeaturesPath;
signals:
    void homeButtonClicked();
    void installRequested(const QString &featureName);
};

#endif // MARKETPLACE_H

