#pragma once

#include "viewframe.h"
#include <QGraphicsView>
#include <QWheelEvent>

class ViewFrame;

class GraphicsView : public QGraphicsView {
    Q_OBJECT
public:
    GraphicsView(ViewFrame* v)
        : QGraphicsView()
        , m_viewFrame(v) {
    }

protected:
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent* e) override;
#endif

private:
    ViewFrame* m_viewFrame;
};
