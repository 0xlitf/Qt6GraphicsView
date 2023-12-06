#pragma once

#include <QGraphicsItem>
#include <QGraphicsItemGroup>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <algorithm>
#include <tuple>
#include "qtpropertybrowser.h"
#include "focusmanager.h"
#include "QtDoublePropertyManager"
#include "QtProperty"
#include "QtTreePropertyBrowser"
#include "graphicsitemgroup.h"

class GraphicsItem;
class FocusManager;

class GraphicsScene : public QGraphicsScene {
    Q_OBJECT
public:
    enum AlignmentFlag {
        AlignLeft = 0x0001,
        AlignRight = 0x0002,
        AlignTop = 0x0003,
        AlignBottom = 0x0004,
        AlignCircle = 0x0005,
        AlignHLine = 0x0006,
        AlignVLine = 0x0007
    };
    Q_ENUM(AlignmentFlag)

    explicit GraphicsScene(QObject* parent = nullptr);

    void deleteAllProperty() {
        QMap<QtProperty*, QString>::ConstIterator itProp = propertyToId.constBegin();
        while (itProp != propertyToId.constEnd()) {
            delete itProp.key();
            itProp++;
        }
        propertyToId.clear();
        idToProperty.clear();
    }

    QVariant itemChange(GraphicsItem* item, QGraphicsItem::GraphicsItemChange change, const QVariant& value);

    void itemClicked(QGraphicsItem* item);

    void addProperty(QtProperty* property, const QString& id) {
        propertyToId[property] = id;
        idToProperty[id] = property;
        QtBrowserItem* item = m_propertyEditor->addProperty(property);
        if (idToExpanded.contains(id))
            m_propertyEditor->setExpanded(item, idToExpanded[id]);
    }

    void updateExpandState() {
        QList<QtBrowserItem*> list = m_propertyEditor->topLevelItems();
        QListIterator<QtBrowserItem*> it(list);
        while (it.hasNext()) {
            QtBrowserItem* item = it.next();
            QtProperty* property = item->property();
            // qDebug() << "property: " << property->valueText();
            // qDebug() << "propertyToId[property]: " << propertyToId[property];
            idToExpanded[propertyToId[property]] = m_propertyEditor->isExpanded(item);
        }
    }

    void groupItems();

    void ungroupItems(QGraphicsItemGroup* group);

    void alignItems(QList<QGraphicsItem*> selectedItems, AlignmentFlag align);

    QtTreePropertyBrowser* getPropertyEditor() const;

    void setPropertyEditor(QtTreePropertyBrowser* newPropertyEditor);

public slots:
    void valueChanged(QtProperty* property, double value);
    void valueChanged(QtProperty* property, int value);

protected:
    void keyPressEvent(QKeyEvent* event) override;

    void keyReleaseEvent(QKeyEvent* event) override;

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseMoveEvent(QGraphicsSceneMouseEvent* event) override;

    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

    void dragEnterEvent(QGraphicsSceneDragDropEvent* event);

    void dragMoveEvent(QGraphicsSceneDragDropEvent* event);

    void dropEvent(QGraphicsSceneDragDropEvent* event);

private:
    QGraphicsItem* m_movingItem = nullptr;

    QMap<QtProperty*, QString> propertyToId;
    QMap<QString, QtProperty*> idToProperty;
    QMap<QString, bool> idToExpanded;
    class QtTreePropertyBrowser* m_propertyEditor;

    QGraphicsItem* m_currentItem;

    QtDoublePropertyManager* doubleManager = new QtDoublePropertyManager(this);
    QtIntPropertyManager* intManager = new QtIntPropertyManager(this);

    FocusManager* m_focusManager = nullptr;

    QList<QGraphicsItem*>  m_focusItemList{};
};

