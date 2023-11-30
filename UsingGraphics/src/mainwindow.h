#pragma once
#include <graphicsscene.h>
#include <QGraphicsScene>
#include <QWidget>

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
};
