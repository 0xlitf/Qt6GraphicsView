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
