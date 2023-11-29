#pragma once
#include "graphicsview.h"
#include <QFrame>
#include <QGraphicsView>

QT_BEGIN_NAMESPACE
class QLabel;
class QSlider;
class QToolButton;
QT_END_NAMESPACE

class GraphicsView;

class ViewFrame : public QFrame {
    Q_OBJECT
public:
    explicit ViewFrame(const QString& name, QWidget* parent = nullptr);

    QGraphicsView* view() const;

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
    GraphicsView* m_graphicsView = nullptr;
    QLabel* m_label = nullptr;
    QLabel* m_label2 = nullptr;
    QToolButton* m_selectModeButton = nullptr;
    QToolButton* m_dragModeButton = nullptr;
    QToolButton* m_antialiasButton = nullptr;
    QToolButton* m_printButton = nullptr;
    QToolButton* m_resetButton = nullptr;
    QSlider* m_zoomSlider = nullptr;
    QSlider* m_rotateSlider = nullptr;
};
