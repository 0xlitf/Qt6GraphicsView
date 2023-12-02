#pragma once
#include <graphicsscene.h>
#include <QGraphicsScene>
#include <QWidget>
#include "QtTreePropertyBrowser"
#include <QDockWidget>

QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

class DockWidget : public QDockWidget {
    Q_OBJECT
public:
    explicit DockWidget(const QString& title, QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags())
        : QDockWidget(parent) {
    }
    explicit DockWidget(QWidget* parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags())
        : QDockWidget(parent) {
    }

protected:
    void closeEvent(QCloseEvent* event) override {
        setFloating(false);
        event->ignore();
    }
};

class MainWindow : public QWidget {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);

private:
    void setupMatrix();
    void populateScene();

    GraphicsScene* m_scene = new GraphicsScene(this);
    DockWidget* m_propertyDock;
    QtTreePropertyBrowser* m_property = new QtTreePropertyBrowser(this);
};
