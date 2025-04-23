#ifndef MARKETPLACE_H
#define MARKETPLACE_H

#include <QObject>
#include <QWidget>

class marketplace : public QObject {
    Q_OBJECT

public:
    explicit marketplace(QObject *parent = nullptr);
    QWidget* createMarketplacePage();
    int windowWidth;
    int windowHeight;
signals:
    void homeButtonClicked();
    void installButtonClicked();
};

#endif // MARKETPLACE_H

