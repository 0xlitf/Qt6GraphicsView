#pragma once
#include <graphicsscene.h>
#include <QGraphicsScene>
#include <QWidget>
#include "QtTreePropertyBrowser"
#include <QDockWidget>

QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

private:
    void setupMatrix();
    void populateScene();

    GraphicsScene* m_scene = new GraphicsScene(this);
    QDockWidget* m_propertyDock;
    QtTreePropertyBrowser* m_property = new QtTreePropertyBrowser(this);
};
