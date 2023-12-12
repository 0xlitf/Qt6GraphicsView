#include "focusmanager.h"

FocusManager::FocusManager(QGraphicsScene* parent)
    : QObject{parent}
    , m_scene{parent} {
}

FocusManager::~FocusManager() {
}

void FocusManager::addItem(QGraphicsItem* item) {
    return;
    auto i = dynamic_cast<GraphicsItem*>(item);
    QList<FocusPointF> pointList = i->recalculateFocusPoint();
    for (int i = 0; i < pointList.count(); ++i) {
        m_scene->addItem(new FocusItem(pointList[i]));
    }
}
