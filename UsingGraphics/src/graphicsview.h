#pragma once

#include <QGraphicsView>

class ViewFrame;

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    GraphicsView(ViewFrame *v) : QGraphicsView(), view(v) { }

protected:
#if QT_CONFIG(wheelevent)
    void wheelEvent(QWheelEvent *) override;
#endif

private:
    ViewFrame *view;
};
