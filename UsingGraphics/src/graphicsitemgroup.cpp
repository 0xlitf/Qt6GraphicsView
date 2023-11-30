#include "graphicsitemgroup.h"
#include "graphicsscene.h"

GraphicsItemGroup::GraphicsItemGroup() {
    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);

}

void GraphicsItemGroup::contextMenuEvent(QGraphicsSceneContextMenuEvent* event) {
    QMenu menu;
    QAction* ungroupAction = menu.addAction("Ungroup");
    QAction* otherAction = menu.addAction("Other Option");

    qDebug() << "event->screenPos(): " << event->screenPos();
    QAction* selectedAction = menu.exec(event->screenPos());

    if (selectedAction == ungroupAction) {
        ungroupItems();
    } else if (selectedAction == otherAction) {

    }
}

void GraphicsItemGroup::ungroupItems() {
    m_scene = dynamic_cast<GraphicsScene*>(this->scene());
    m_scene->destroyItemGroup(this);
}
