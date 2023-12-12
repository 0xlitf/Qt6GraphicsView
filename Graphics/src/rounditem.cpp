
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QGraphicsScene>
#include <graphicsitem.h>
#include <graphicsitemgroup.h>
#include <QMenu>
#include <QAction>
#include <QContextMenuEvent>
#include "graphicsscene.h"
#include "rounditem.h"


RoundItem::RoundItem(const QColor& color, int x, int y) {
    this->m_x = x;
    this->m_y = y;
    this->m_color = color;
    setZValue((x + y) % 2);

    setFlags(ItemIsSelectable | ItemIsMovable);
    setAcceptHoverEvents(true);

}

QRectF RoundItem::boundingRect() const {
    return QRectF(0, 0, m_initialWidth, m_initialHeight);
}

QPainterPath RoundItem::shape() const {
    QPainterPath path;
    path.addEllipse(this->boundingRect());
    return path;
}

void RoundItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {
    Q_UNUSED(widget);

    QColor borderColor = m_color;
    QColor fillColor = m_color;

    if (option->state & QStyle::State_MouseOver) {
        fillColor = fillColor.lighter(125);
        borderColor = Qt::black;
    }

    if (option->state & QStyle::State_Selected) {
        fillColor = m_color.darker(150);
        borderColor = Qt::green;
    }

    // QPen p = painter->pen();
    // painter->setPen(QPen(borderColor, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    // painter->setBrush(fillColor);
    // painter->drawRect(QRectF(0,0,110,70));
    // painter->setPen(p);

    m_scale = option->levelOfDetailFromTransform(painter->worldTransform());

    QBrush b = painter->brush();
    painter->setBrush(fillColor);
    // painter->drawText(QPointF(15, 15), QString::number(m_scale));

    painter->drawEllipse(0, 0, 2 * m_radius, 2 * m_radius);
    painter->setBrush(b);
}

void RoundItem::contextMenuEvent(QGraphicsSceneContextMenuEvent* event) {
    this->setSelected(true);

    m_scene = dynamic_cast<GraphicsScene*>(this->scene());
    int selectCount = m_scene->selectedItems().count();

    QMenu menu;
    QAction *groupAction = menu.addAction(QString("Group %1 Items").arg(QString::number(selectCount)));
    QAction *alignLeftAction = menu.addAction("AlignLeft");
    QAction *alignRightAction = menu.addAction("AlignRight");
    QAction *alignTopAction = menu.addAction("AlignTop");
    QAction *alignBottomAction = menu.addAction("AlignBottom");

    if (selectCount <= 1) {
        groupAction->setEnabled(false);
        alignLeftAction->setEnabled(false);
        alignRightAction->setEnabled(false);
        alignTopAction->setEnabled(false);
        alignBottomAction->setEnabled(false);
    }

    QAction *selectedAction = menu.exec(event->screenPos());

    if (selectedAction == groupAction) {
        m_scene->groupItems();
    } else if (selectedAction == alignLeftAction) {

    } else if (selectedAction == alignRightAction) {

    } else if (selectedAction == alignTopAction) {

    } else if (selectedAction == alignBottomAction) {

    } else {
        qDebug() << "RoundItem selectedAction not match";
    }
}
