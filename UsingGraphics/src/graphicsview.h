#pragma once

#include "viewframe.h"
#include <QGraphicsView>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QScrollBar>

class ViewFrame;

class GraphicsView : public QGraphicsView {
    Q_OBJECT
public:
    GraphicsView(ViewFrame* v);

protected:
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent* e) override;
#endif

protected:
    void mousePressEvent(QMouseEvent *event) override {
        m_pressPoint = event->pos();

        if (event->button() == Qt::RightButton) {
            lastPos = event->pos();
        } else {
            QGraphicsView::mousePressEvent(event);
        }
    }

    void mouseMoveEvent(QMouseEvent* event) override {
        QPointF point = event->pos() - m_pressPoint;

        if (point.manhattanLength() > 1) {
            if (event->buttons() == Qt::LeftButton) {
                this->setCursor(Qt::SizeAllCursor);
            } else if (event->buttons() == Qt::RightButton) {
                this->setCursor(Qt::ClosedHandCursor);
            }

        }

        if (event->buttons() == Qt::LeftButton) {

        } else if (event->buttons() == Qt::RightButton) {
            QPointF delta = event->pos() - lastPos;
            horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
            verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
            lastPos = event->pos();

            m_sceneMovedByRightButton = true;
        } else {
            this->setCursor(Qt::ArrowCursor);
        }

        QGraphicsView::mouseMoveEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent* event) override {
        this->setCursor(Qt::ArrowCursor);

        if (m_sceneMovedByRightButton) {
            event->accept();
            m_sceneMovedByRightButton = false;
        } else {
            QGraphicsView::mouseReleaseEvent(event);
        }
    }

private:
    bool m_sceneMovedByRightButton = false;

    QPointF m_pressPoint{};

    QPointF lastPos;

    ViewFrame* m_viewFrame;
};
