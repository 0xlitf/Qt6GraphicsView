#pragma once

#include "viewframe.h"
#include <QGraphicsView>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QScrollBar>
#include <QElapsedTimer>
#include "graphics_global.h"

class ViewFrame;

class GRAPHICS_EXPORT GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(ViewFrame* v);
    QPixmap pix;
    QPoint lasetPoint;
    QPoint endPoint;
    QPixmap tempPix;
    bool isDrawing;
    bool isDoubleClick;
protected:
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent* e) override;
#endif

protected:
    void paintEvent(QPaintEvent* event) override {
        QElapsedTimer t;
        t.start();
        // qDebug() << "this->viewport()" << this->viewport()->rect();
        QGraphicsView::paintEvent(event);
		QRectF viewRect = this->rect();
        QPainter painter(viewport());
        // painter.setPen(Qt::blue);
        painter.setBrush(QColor(0, 0, 255, 63));
        QFont f = painter.font();
        f.setPixelSize(30);
        painter.setFont(f);
        painter.setPen(Qt::black);

        painter.drawRect(QRect(10, 10, 200, 40));

        int ms = t.elapsed();
        if (ms > 0) {
            painter.drawText(QRectF(20, 10, 200, 40), QString(QString::number(ms) + " ms, " + QString::number(1000 / ms) + " fps"));
        }
    }

    void enterEvent(QEnterEvent *event) override {
#ifdef DEBUG_VIEW_EVENT
        qDebug() << "GraphicsView::enterEvent";
#endif

        QGraphicsView::enterEvent(event);
    }

    void moveEvent(QMoveEvent *event) override {
#ifdef DEBUG_VIEW_EVENT
        qDebug() << "GraphicsView::moveEvent";
#endif

        QGraphicsView::moveEvent(event);
    }

    void leaveEvent(QEvent *event) override {
#ifdef DEBUG_VIEW_EVENT
        qDebug() << "GraphicsView::leaveEvent";
#endif
        unsetCursor();
        QGraphicsView::leaveEvent(event);
    }

    void mousePressEvent(QMouseEvent *event) override {
#ifdef DEBUG_VIEW_EVENT
        qDebug() << "GraphicsView::mousePressEvent" << event->buttons();
#endif
        m_pressPoint = event->pos();

        if (event->buttons() == Qt::LeftButton) {
            QGraphicsView::mousePressEvent(event);
        } else if (event->button() == Qt::RightButton) {
            this->unsetCursor();
            lastPos = event->pos();

            event->accept();
        } else {
            QGraphicsView::mousePressEvent(event);
        }
    }

    void mouseMoveEvent(QMouseEvent* event) override {
#ifdef DEBUG_VIEW_EVENT
        qDebug() << "GraphicsView::mouseMoveEvent" << event->buttons();
#endif
        QPointF point = event->pos() - m_pressPoint;

        // if (point.manhattanLength() > 1) {
        //     if (event->buttons() == Qt::LeftButton) {
        //         this->setCursor(Qt::SizeAllCursor);
        //     } else if (event->buttons() == Qt::RightButton) {
        //         this->setCursor(Qt::ClosedHandCursor);
        //     }

        // }

        if (event->buttons() == Qt::LeftButton) {

        } else if (event->buttons() == Qt::RightButton) {
            QPointF delta = event->pos() - lastPos;
            horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
            verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
            lastPos = event->pos();

            if (point.manhattanLength() > 1) {
                m_sceneMovedByRightButton = true;

                this->setCursor(Qt::ClosedHandCursor);
                this->viewport()->setCursor(Qt::ClosedHandCursor);
            }
            qDebug() << "GraphicsView::mouseMoveEvent" << event->buttons() << point.manhattanLength() << this->cursor();

            event->accept();
            this->update();
        }

        QGraphicsView::mouseMoveEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent* event) override {
#ifdef DEBUG_VIEW_EVENT
        qDebug() << "GraphicsView::mouseReleaseEvent" << m_sceneMovedByRightButton ;
#endif
        this->setCursor(Qt::ArrowCursor);
        this->viewport()->setCursor(Qt::ArrowCursor);

        if (m_sceneMovedByRightButton) {
            event->accept();
        } else {
            QGraphicsView::mouseReleaseEvent(event);
        }
    }

    void contextMenuEvent(QContextMenuEvent *event) {
#ifdef DEBUG_VIEW_EVENT
        qDebug() << "GraphicsView::contextMenuEvent" << m_sceneMovedByRightButton ;
#endif
        if (m_sceneMovedByRightButton) {
            event->accept();
            m_sceneMovedByRightButton = false;
        } else {
            QGraphicsView::contextMenuEvent(event);
        }
    }

private:
    bool m_sceneMovedByRightButton = false;

    QPointF m_pressPoint{};

    QPointF lastPos;

    ViewFrame* m_viewFrame;
};
