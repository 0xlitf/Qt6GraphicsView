#include "graphicsview.h"
#include "viewframe.h"
#include <QWheelEvent>

#if QT_CONFIG(wheelevent)
void GraphicsView::wheelEvent(QWheelEvent* e) {
    if (e->modifiers() & Qt::ControlModifier) {
        if (e->angleDelta().y() > 0)
            m_viewFrame->zoomInBy(6);
        else
            m_viewFrame->zoomOutBy(6);
        e->accept();
    } else {
        QGraphicsView::wheelEvent(e);
    }
}
#endif

GraphicsView::GraphicsView(ViewFrame* v)
    : QGraphicsView()
    , m_viewFrame(v) {
    setRenderHint(QPainter::Antialiasing, true);
    setDragMode(QGraphicsView::ScrollHandDrag);  // 使用 ScrollHandDrag 模式，使鼠标右键可以拖动
    setRenderHint(QPainter::SmoothPixmapTransform, true);
    // setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setOptimizationFlag(QGraphicsView::DontAdjustForAntialiasing, true);

    setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
}
