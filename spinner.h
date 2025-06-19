#ifndef SPINNERWIDGET_H
#define SPINNERWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPainter>

class SpinnerWidget : public QWidget {
    Q_OBJECT

public:
    explicit SpinnerWidget(QWidget *parent = nullptr)
        : QWidget(parent), angle(0) {
        setFixedSize(24, 24);  // Set default size
        setAttribute(Qt::WA_TranslucentBackground);
        setAttribute(Qt::WA_NoSystemBackground);
        setStyleSheet("background: transparent;");

        timer = new QTimer(this);
        connect(timer, &QTimer::timeout, this, &SpinnerWidget::onTimeout);
        timer->start(100);  // Adjust speed here
    }

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setBrush(Qt::NoBrush);
        painter.setPen(Qt::NoPen);

        painter.translate(width() / 2, height() / 2);
        painter.rotate(angle);

        int radius = qMin(width(), height()) / 2 - 2;
        for (int i = 0; i < 12; ++i) {
            QColor color = Qt::white;
            color.setAlphaF(1.0 - (i / 12.0));
            painter.setBrush(color);
            painter.drawEllipse(QPoint(radius, 0), 2, 2);
            painter.rotate(30);
        }
    }

private slots:
    void onTimeout() {
        angle = (angle + 30) % 360;
        update();
    }

private:
    QTimer *timer;
    int angle;
};

#endif // SPINNERWIDGET_H
