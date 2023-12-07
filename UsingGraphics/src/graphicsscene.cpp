#include "graphicsscene.h"
#include "QtDoubleSpinBoxFactory"
#include "QtSpinBoxFactory"
#include <QtTreePropertyBrowser>
#include "graphicsitem.h"
#include "focusitem.h"

GraphicsScene::GraphicsScene(QObject* parent)
    : QGraphicsScene{parent} {
    m_focusManager = new FocusManager(this);

    connect(doubleManager, SIGNAL(valueChanged(QtProperty*, double)), this, SLOT(valueChanged(QtProperty*, double)));
    connect(intManager, SIGNAL(valueChanged(QtProperty*, int)), this, SLOT(valueChanged(QtProperty*, int)));
    connect(this, &QGraphicsScene::selectionChanged, this, [=] {
        auto list = this->selectedItems();
        qDebug() << "selectedItems: " << list.count();
        if (list.count() == 0) {
            for (auto item : this->items()) {
                item->setFlag(QGraphicsItem::ItemIsMovable, false);
            }
        }

        for (int i = m_focusItemList.count() - 1; i >= 0; --i) {
            if (list.contains(m_focusItemList[i])) {
                continue;
            } else {
                m_focusItemList.remove(i);
            }
        }
        for (int i = 0; i < list.count(); ++i) {
            if (m_focusItemList.contains(list[i])) {
                continue;
            } else {
                qDebug() << "selectsetFlagedItems ItemIsMovable" << list.count();
                m_focusItemList.append(list[i]);
            }
        }
        for (auto item : m_focusItemList) {
            item->setFlag(QGraphicsItem::ItemIsMovable, m_focusItemList.count() > 1);
        }
    });
}

QVariant GraphicsScene::itemChange(GraphicsItem* item, QGraphicsItem::GraphicsItemChange change, const QVariant& value) {
    qDebug() << "GraphicsScene::itemChange:" << change;
    if (change == QGraphicsItem::ItemPositionChange) {
        // value is the new position.
        QPointF newPos = value.toPointF();
        QRectF rect = this->sceneRect();
        if (!rect.contains(newPos)) {
            // Keep the item inside the scene rect.
            newPos.setX(qMin(rect.right(), qMax(newPos.x(), rect.left())));
            newPos.setY(qMin(rect.bottom(), qMax(newPos.y(), rect.top())));

            if (item != m_currentItem)
                return QPointF();

            doubleManager->setValue(idToProperty[QLatin1String("xpos")], item->x());
            doubleManager->setValue(idToProperty[QLatin1String("ypos")], item->y());
            doubleManager->setValue(idToProperty[QLatin1String("zpos")], item->zValue());

            return newPos;
        }
    }
    if (change == QGraphicsItem::ItemSelectedChange) {

    }

    return QVariant();
}

void GraphicsScene::itemClicked(QGraphicsItem* item) {
    if (item->type() == QGraphicsItemGroup::Type) {
        QPointF groupTopLeftInScene = item->mapToScene(0, 0);
        qDebug() << "itemClicked: group pos" << groupTopLeftInScene;
        qDebug() << "itemClicked: item->pos" << item->pos();
    } else {
        qDebug() << "itemClicked: item" << item;
    }
    
    this->updateExpandState();
    this->deleteAllProperty();

    m_currentItem = item;

    QtProperty* property;

    property = doubleManager->addProperty(tr("Position X"));
    doubleManager->setValue(property, item->x());
    addProperty(property, QLatin1String("xpos"));

    property = doubleManager->addProperty(tr("Position Y"));
    doubleManager->setValue(property, item->y());
    addProperty(property, QLatin1String("ypos"));

    property = intManager->addProperty(tr("Position Z"));
    intManager->setValue(property, item->zValue());
    addProperty(property, QLatin1String("zpos"));
}

void GraphicsScene::groupItems() {
    qDebug() << "groupItems" << this->selectedItems().count();
    if (this->selectedItems().count() <= 1) {
        return;
    }

    GraphicsItemGroup* group = new GraphicsItemGroup();
    group->setFlag(QGraphicsItem::ItemIsMovable);

    QList<QGraphicsItem*> selectedItems = this->selectedItems();

    auto minX = this->sceneRect().bottomRight().x();
    auto minY = this->sceneRect().bottomRight().y();
    auto maxX = this->sceneRect().topLeft().x();
    auto maxY = this->sceneRect().topLeft().y();

    QRectF rect;
    qDebug() << "diag" << QPointF(minX, minY) << QPointF(maxX, maxY);
    for (QGraphicsItem* item : selectedItems) {
        if (item/*GraphicsItem* i = dynamic_cast<GraphicsItem*>(item)*/) {
            auto toScene = item->mapRectToScene(item->boundingRect());
            if (toScene.topLeft().x() < minX) {
                minX = toScene.topLeft().x();
            }
            if (toScene.topLeft().y() < minY) {
                minY = toScene.topLeft().y();
            }
            if (toScene.bottomRight().x() > maxX) {
                maxX = toScene.bottomRight().x();
            }
            if (toScene.bottomRight().y() > maxY) {
                maxY = toScene.bottomRight().y();
            }
            rect = QRectF{QPointF(minX, minY), QPointF(maxX, maxY)};
        }
    }

    group->setPos(rect.topLeft());
    group->setDiagonalPoint(QPointF(0, 0), rect.bottomRight() - rect.topLeft());
    qDebug() << "group->pos()" << group->pos();
    qDebug() << "group topleft" << QPointF(minX, minY);
    qDebug() << "group bottomRight" << QPointF(maxX, maxY);
    this->addItem(group);

    group->setFlag(QGraphicsItemGroup::ItemIsSelectable/* | QGraphicsItemGroup::ItemIsMovable*/);
    group->setHandlesChildEvents(true);
    group->setAcceptHoverEvents(true);


    for (QGraphicsItem* item : selectedItems) {
        if (GraphicsItem* i = dynamic_cast<GraphicsItem*>(item)) {
            i->setSelected(false);
            i->setFlag(QGraphicsItem::ItemIsSelectable, false);
            group->addToGroup(item);
        }
    }
    this->clearSelection();
}

void GraphicsScene::ungroupItems(QGraphicsItemGroup* group) {
    this->clearSelection();

    for (auto j : group->childItems()) {
        if (j) {
            j->setFlag(QGraphicsItem::ItemIsSelectable, true);
        } else {
            qFatal() << "j nullptr";
        }
    }
    this->destroyItemGroup(group);
    m_movingItem = nullptr;
    group = nullptr;
}

void GraphicsScene::alignItems(QList<QGraphicsItem*> selectedItems, AlignmentFlag align) {
    if (selectedItems.size() < 2) {
        return;
    }
    auto [xList, yList] = [&] {
        QList<int> xList;
        QList<int> yList;
        for (auto item : selectedItems) {
            xList.append(item->x());
            yList.append(item->y());
        }
        return std::make_tuple(xList, yList);
    }();

    auto maxValue = [](QList<int>& list) {
        return *std::max_element(list.begin(), list.end());
    };
    auto minValue = [](QList<int>& list) {
        return *std::min_element(list.begin(), list.end());
    };
    auto average = [](const QList<int>& intList) {
        if (intList.isEmpty()) {
            qDebug() << "Error: The list is empty.";
            return 0.0;
        }

        int sum = 0;
        for (const int& value : intList) {
            sum += value;
        }

        return static_cast<double>(sum) / intList.size();
    };

    switch (align) {
        case AlignLeft: {
            int v = minValue(xList);
            for (auto item : selectedItems) {
                item->setX(v);
            }
        } break;
        case AlignRight: {
            int v = maxValue(xList);
            for (auto item : selectedItems) {
                item->setX(v);
            }
        } break;
        case AlignTop: {
            int v = minValue(yList);
            for (auto item : selectedItems) {
                item->setY(v);
            }
        } break;
        case AlignBottom: {
            int v = maxValue(yList);
            for (auto item : selectedItems) {
                item->setY(v);
            }
        } break;
        case AlignCircle: {
            QPointF center{average(xList), average(yList)};
            int numPoints = selectedItems.count();
            const double angleIncrement = 2 * M_PI / numPoints;
            double radius = 40 / sin(angleIncrement / 2);

            for (int i = 0; i < numPoints; ++i) {
                double angle = i * angleIncrement;
                double x = center.x() + radius * std::cos(angle);
                double y = center.y() + radius * std::sin(angle);

                qDebug() << "Point " << i << ": (" << x << ", " << y << ")";

                auto item = selectedItems[i];
                item->setX(x);
                item->setY(y);
            }
        } break;
        case AlignHLine: {
            int x = minValue(xList);
            int y = minValue(yList);

            for (auto item : selectedItems) {
                item->setX(x);
                item->setY(y);

                x += item->boundingRect().width() + 10;
            }
        } break;
        case AlignVLine: {
            int x = minValue(xList);
            int y = minValue(yList);

            for (auto item : selectedItems) {
                item->setX(x);
                item->setY(y);

                y += item->boundingRect().height() + 10;
            }
        } break;
        default: {
            qDebug() << align << "is not support.";
        } break;
    }
}

QtTreePropertyBrowser* GraphicsScene::getPropertyEditor() const {
    return m_propertyEditor;
}

void GraphicsScene::setPropertyEditor(QtTreePropertyBrowser* newPropertyEditor) {
    m_propertyEditor = newPropertyEditor;

    m_propertyEditor->setFactoryForManager(doubleManager, new QtDoubleSpinBoxFactory(this));
    m_propertyEditor->setFactoryForManager(intManager, new QtSpinBoxFactory(this));
}

void GraphicsScene::keyPressEvent(QKeyEvent* event) {
    QGraphicsScene::keyPressEvent(event);
    switch (Qt::Key(event->key())) {
        case Qt::Key_F1: {
            this->groupItems();
        } break;
        case Qt::Key_F2: {
            for (auto item : this->selectedItems()) {
                if (item->type() == QGraphicsItemGroup::Type) {
                    auto i = dynamic_cast<QGraphicsItemGroup*>(item);
                    this->ungroupItems(i);
                }
            }
        } break;
        case Qt::Key_1: {
            this->alignItems(this->selectedItems(), AlignLeft);
        } break;
        case Qt::Key_2: {
            this->alignItems(this->selectedItems(), AlignRight);
        } break;
        case Qt::Key_3: {
            this->alignItems(this->selectedItems(), AlignTop);
        } break;
        case Qt::Key_4: {
            this->alignItems(this->selectedItems(), AlignBottom);
        } break;
        case Qt::Key_5: {
            this->alignItems(this->selectedItems(), AlignCircle);
        } break;
        case Qt::Key_6: {
            this->alignItems(this->selectedItems(), AlignHLine);
        } break;
        case Qt::Key_7: {
            this->alignItems(this->selectedItems(), AlignVLine);
        } break;
        default: {

        } break;
    }
}


void GraphicsScene::keyReleaseEvent(QKeyEvent* event) {
    QGraphicsScene::keyReleaseEvent(event);

    qDebug() << "GraphicsScene Key Released: " << Qt::Key(event->key());
}

void GraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    QPointF scenePos = event->scenePos();
    QGraphicsItem* item = this->itemAt(scenePos, QTransform());

    if (item) {
        m_movingItem = item;
        if (item->parentItem()) {
            this->itemClicked(item->parentItem());
        } else {
            this->itemClicked(item);
        }

        if (GraphicsItemGroup* g = qgraphicsitem_cast<GraphicsItemGroup*>(item)) {

        } else if (GraphicsItem* g = qgraphicsitem_cast<GraphicsItem*>(item)) {

        }

    } else {
        m_movingItem = nullptr;
        // qDebug() << "no item clicked";
    }

    QGraphicsScene::mousePressEvent(event);
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {
    if (m_movingItem && m_movingItem == m_currentItem) {
        auto x = m_movingItem->x();
        auto y = m_movingItem->y();
        auto z = m_movingItem->zValue();
        doubleManager->setValue(idToProperty[QLatin1String("xpos")], m_movingItem->x());
        doubleManager->setValue(idToProperty[QLatin1String("ypos")], m_movingItem->y());
        intManager->setValue(idToProperty[QLatin1String("zpos")], m_movingItem->zValue());
    }

    QGraphicsScene::mouseMoveEvent(event);
}

void GraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* event) {

    QGraphicsScene::mouseReleaseEvent(event);
}

void GraphicsScene::valueChanged(QtProperty* property, double value) {
    if (!propertyToId.contains(property))
        return;

    if (!m_currentItem)
        return;

    QString id = propertyToId[property];
    if (id == QLatin1String("xpos")) {
        m_currentItem->setX(value);
    } else if (id == QLatin1String("ypos")) {
        m_currentItem->setY(value);
    }
    this->update();
}

void GraphicsScene::valueChanged(QtProperty* property, int value) {
    if (!propertyToId.contains(property))
        return;

    if (!m_currentItem)
        return;

    QString id = propertyToId[property];
    if (id == QLatin1String("zpos")) {
        m_currentItem->setZValue(value);
    }
    this->update();
}

void GraphicsScene::dragEnterEvent(QGraphicsSceneDragDropEvent* event) {
    event->accept();
    QGraphicsScene::dragEnterEvent(event);
}

void GraphicsScene::dragMoveEvent(QGraphicsSceneDragDropEvent* event) {
    event->accept();
}

void GraphicsScene::dropEvent(QGraphicsSceneDragDropEvent* event) {
    event->acceptProposedAction();
    QGraphicsScene::dropEvent(event);
}
