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
        if (event->button() == Qt::RightButton) {
            // 在这里处理鼠标右键按下事件
            // 例如记录下按下时的坐标
            lastPos = event->pos();
        } else {
            QGraphicsView::mousePressEvent(event);
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (event->buttons() == Qt::RightButton) {
            // 在这里处理鼠标右键拖动事件
            // 例如根据拖动距离进行视图的平移
            QPoint delta = event->pos() - lastPos;
            horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
            verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
            lastPos = event->pos();
        } else {
            QGraphicsView::mouseMoveEvent(event);
        }
    }

private:
    QPoint lastPos;
private:
    ViewFrame* m_viewFrame;
};
