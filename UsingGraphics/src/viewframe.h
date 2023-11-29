#pragma once
#include <QFrame>
#include <QGraphicsView>
#include "graphicsview.h"

QT_BEGIN_NAMESPACE
class QLabel;
class QSlider;
class QToolButton;
QT_END_NAMESPACE

class ViewFrame : public QFrame
{
    Q_OBJECT
public:
    explicit ViewFrame(const QString &name, QWidget *parent = nullptr);

    QGraphicsView *view() const;

public slots:
    void zoomIn();
    void zoomOut();
    void zoomInBy(int level);
    void zoomOutBy(int level);

private slots:
    void resetView();
    void setResetButtonEnabled();
    void setupMatrix();
    void togglePointerMode();
    void toggleAntialiasing();
    void print();
    void rotateLeft();
    void rotateRight();

private:
    GraphicsView *graphicsView;
    QLabel *label;
    QLabel *label2;
    QToolButton *selectModeButton;
    QToolButton *dragModeButton;
    QToolButton *antialiasButton;
    QToolButton *printButton;
    QToolButton *resetButton;
    QSlider *zoomSlider;
    QSlider *rotateSlider;
};
