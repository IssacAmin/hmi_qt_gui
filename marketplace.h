#ifndef MARKETPLACE_H
#define MARKETPLACE_H

#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QMap>
#include <QString>

class SpinnerWidget; // Forward declare SpinnerWidget

class marketplace : public QObject
{
    Q_OBJECT

public:
    explicit marketplace(QObject *parent = nullptr);

    QWidget* createMarketplacePage();
    void markFeatureAsInstalled(const QString &name);

signals:
    void homeButtonClicked();
    void installRequested(const QString &featureName);

private:
private:
    QWidget *marketplacePage = nullptr;  // <-- store the created page


    void showSpinnerOnButton(QPushButton *button, const QString &name);
    bool isFeatureInstalled(const QString &name);

    QMap<QString, SpinnerWidget*> spinnerMap;    // Maps feature name -> spinner
    QMap<QString, QPushButton*> featureButtons;  // Maps feature name -> install button
    QString installedFeaturesPath;

    int windowWidth;
    int windowHeight;

};

#endif // MARKETPLACE_H


