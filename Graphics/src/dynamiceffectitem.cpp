#include <QPainter>
#include "dynamiceffectitem.h"

DynamicEffectItem::DynamicEffectItem()
    : GraphicsItem() {
    this->setFlag(QGraphicsItem::ItemIsFocusable);
    this->setFlag(QGraphicsItem::ItemIsSelectable);
    this->setFlag(QGraphicsItem::ItemSendsGeometryChanges);

    this->setAcceptHoverEvents(true);

    this->startTimer();

    this->setInitialWidth(500);
    this->setInitialHeight(500);

    this->setTopLeft({-this->initialWidth() / 2, -this->initialHeight() / 2});
    this->setBottomRight({this->initialWidth() / 2, this->initialHeight() / 2});
}

QRectF DynamicEffectItem::shapeRect() const {
    return QRectF(this->topLeft(), this->bottomRight()) + QMarginsF(2., 2., 2., 2.);
}

QRectF DynamicEffectItem::boundingRect() const {
    return QRectF(this->topLeft(), this->bottomRight()) + QMarginsF(30., 30., 30., 30.);
}

QPainterPath DynamicEffectItem::shape() const {
    QPainterPath path;
    path.addRect(this->shapeRect());

    QPointF center{(this->topLeft() + this->bottomRight()) / 2};
    path.addRect(QRectF(center.x(), this->topLeft().y() - 25, 0, 0) + QMarginsF(2., 2., 2., 2.));

    return path;
}

void DynamicEffectItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) {

    QPen pen;

    pen.setColor(Qt::transparent);
    painter->setPen(pen);
    painter->setRenderHint(QPainter::Antialiasing);

    Eigen::array<Eigen::Index, 3> dims = m_frame.dimensions();
    auto offset = this->bottomRight() - this->topLeft();
    double mosaicWidth = offset.x() / dims[0];
    double mosaicHeight = offset.y() / dims[1];
    for (int i = 0; i < dims[0]; ++i) {
        for (int j = 0; j < dims[1]; ++j) {
            auto rect = QPointF(j * mosaicWidth + this->topLeft().x(), i * mosaicHeight + this->topLeft().y());
            if (auto c = QColor(m_frame(i, j, 0), m_frame(i, j, 1), m_frame(i, j, 2)); c != QColor()) {
                painter->setBrush(c);
            } else {
                painter->setBrush(Qt::gray);
            }
            painter->drawRect(QRectF(rect, QSizeF(mosaicWidth, mosaicHeight)));
        }
    }
}

void DynamicEffectItem::startTimer() {
    m_timer->setInterval(1000 / m_bps);
    // m_timer->setSingleShot(true);
    connect(m_timer, &QTimer::timeout, [&]() {
        m_frame = m_colorScrolling->getNextFrame();

        QList<QGraphicsItem*> itemsAboveEllipse = findItemsAbove(this);

        // 打印结果
        qDebug() << "Items above ellipseItem:" << itemsAboveEllipse.count();
        // foreach (QGraphicsItem *item, itemsAboveEllipse)
        // {
        //     qDebug() << item;
        // }

        this->update();
    });
    m_timer->start();
}
