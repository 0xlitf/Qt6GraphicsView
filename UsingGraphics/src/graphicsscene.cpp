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
        for (int i = m_focusItemList.count() - 1; i >= 0; --i) {
            if (list.contains(m_focusItemList[i])) {
                continue;
            } else {
                m_focusItemList[i]->setFlag(QGraphicsItem::ItemIsMovable, false);
                m_focusItemList.remove(i);
            }
        }
        for (int i = 0; i < list.count(); ++i) {
            if (m_focusItemList.contains(list[i])) {
                continue;
            } else {
                m_focusItemList.append(list[i]);
            }
        }
        if (m_focusItemList.count() > 1) {
            for (auto item : m_focusItemList) {
                item->setFlag(QGraphicsItem::ItemIsMovable, true);
            }
        }
        qDebug() << "list.count()" << list.count();
        qDebug() << "m_focusItemList.count()" << m_focusItemList.count();

        return;
        if (list.count() > 1) {
            qDebug() << "m_focusItemList.count()" << m_focusItemList.count();
            if (m_focusItemList.count() != list.count()) {
                qFatal() << "m_focusItemList.count() != list";
                for (int i = 0; i < m_focusItemList.count(); ++i) {
                    if (list.contains(m_focusItemList[i])) {
                        continue;
                    } else {
                        qFatal() << "m_focusItemList.count() != list";
                    }
                }
                for (int i = 0; i < list.count(); ++i) {
                    if (m_focusItemList.contains(list[i])) {
                        continue;
                    } else {
                        qFatal() << "m_focusItemList.count() != list";
                    }
                }
            }
        } else {
            for (int i = 0; i < list.count(); ++i) {
                list[i]->setFlag(QGraphicsItem::ItemIsMovable, false);
            }
        }
    });
}

QVariant GraphicsScene::itemChange(GraphicsItem* item, QGraphicsItem::GraphicsItemChange change, const QVariant& value) {
    qDebug() << "itemChange:" << change;
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
        if (this->selectedItems().count() > 1) {
            for (int i = 0; i < this->selectedItems().count(); ++i) {
                this->selectedItems()[i]->setFlag(QGraphicsItem::ItemIsMovable, true);
            }
        } else {
            for (int i = 0; i < this->selectedItems().count(); ++i) {
                this->selectedItems()[i]->setFlag(QGraphicsItem::ItemIsMovable, false);
            }
        }
    }

    return QVariant();
}

void GraphicsScene::itemClicked(GraphicsItem* item) {
    qDebug() << "item clicked: " << item;
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
    GraphicsItemGroup* group = new GraphicsItemGroup();
    this->addItem(group);

    group->setFlags(QGraphicsItemGroup::ItemIsSelectable | QGraphicsItemGroup::ItemIsMovable);
    group->setAcceptHoverEvents(true);

    QList<QGraphicsItem*> selectedItems = this->selectedItems();
    qDebug() << "selectedItems: " << selectedItems.count();

    for (QGraphicsItem* item : selectedItems) {
        GraphicsItem* i = dynamic_cast<GraphicsItem*>(item);

        i->setSelected(false);

        group->addToGroup(item);
    }
}

void GraphicsScene::ungroupItems() {
    for (auto item : this->selectedItems()) {
        if (item->isSelected() && item->type() == QGraphicsItemGroup::Type) {
            auto i = dynamic_cast<GraphicsItemGroup*>(item);
            qDebug() << "QGraphicsItemGroup: " << i->type();

            this->destroyItemGroup(i);
        }
    }
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
    qDebug() << "GraphicsScene Key Pressed: " << Qt::Key(event->key());
    switch (Qt::Key(event->key())) {
        case Qt::Key_F1: {
            this->groupItems();
        } break;
        case Qt::Key_F2: {
            this->ungroupItems();
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
    // this->setFocus(Qt::MouseFocusReason);

    QPointF scenePos = event->scenePos();
    GraphicsItem* item = dynamic_cast<GraphicsItem*>(itemAt(scenePos, QTransform()));

    if (item) {
        m_movingItem = item;
        itemClicked(item);
    } else {
        m_movingItem = nullptr;
        // qDebug() << "no item clicked";
    }

    QGraphicsScene::mousePressEvent(event);
}

void GraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent* event) {

    if (m_movingItem == m_currentItem) {
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
