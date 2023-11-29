#pragma once
#include <QWidget>
#include <QGraphicsScene>

QT_BEGIN_NAMESPACE

QT_END_NAMESPACE

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);

private:
    void setupMatrix();
    void populateScene();

    QGraphicsScene *scene = new QGraphicsScene(this);
};
