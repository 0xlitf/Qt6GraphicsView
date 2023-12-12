#ifndef ANCHORITEM_H
#define ANCHORITEM_H
#include <QCursor>
#include <QGraphicsItem>
#include <QGraphicsSceneContextMenuEvent>
#include <QGraphicsSceneHoverEvent>
#include <QKeyEvent>
#include <QObject>
#include <QPainter>
#include <QPixmap>
#include <QStyleOptionGraphicsItem>
#include <QTimer>
#include <QTransform>
#include "graphics_global.h"

class GRAPHICS_EXPORT AnchorItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    AnchorItem(QRectF rect = QRectF(0, 0, 200, 200), QPixmap map = QPixmap(":/icons/circle_green.png"));
    QRectF boundingRect() const override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

    //    void setColor(const QColor &color) { brushColor = color; }

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent* event) override;
    QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

private slots:


private:
    QRectF getItemRect() const;
    void InitPixmap();
    void ItemResize();
    Qt::CursorShape GetResizeCursorShape(qreal angle);

private:
    QColor brushColor;
    qreal m_itemRatio;
    int m_internal = 15;
    int m_ellipseWidth = 30;
    int m_lineLen = 30;
    int m_rotateEllipseWidth = 20;

    QPixmap m_rotatePixmap;
    QPixmap m_scalePixmap;
    QPixmap m_itemPixmap;
    QSizeF m_itemSize;
    QTransform m_transform;

    enum ItemOperation {
        ITEM_NONE = 0x00,
        ITEM_MOVE = 0x01,
        ITEM_RESIZE = 0x02,
        ITEM_TOTATE = 0x03
    };

    enum CursorType {
        ARROR_CURSOR = 0x00,
        RESIZE_TOPLEFT_CURSOR = 0x01,
        RESIZE_TOPRIGHT_CURSOR,
        RESIZE_BOTTOMLEFT_CURSOR,
        RESIZE_BOTTOMRIGHT_CURSOR,
        HAND_OPEN_CURSOR,
        HAND_CLOSE_CURSOR,
        ROTATE_CURSOR,
    };

    int m_operation;
    int m_cursorShape;
    QRectF m_topLeftRect, m_topRightRect, m_bottomLeftRect, m_bottomRightRect, m_rotateRect;
    qreal m_topLeftAngle, m_topRightAngle, m_bottomLeftAngle, m_bottomRightAngle, m_rotateAngle;
    QCursor m_rotateHoverCursor;
    QCursor m_rotatePressCursor;

    QPointF m_currentScenePos, m_pressScenePos;
    QPointF m_pressItemPos;
};

#endif // ANCHORITEM_H
