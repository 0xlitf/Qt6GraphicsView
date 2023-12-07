#pragma once

#include <QPoint>
#include <QCursor>
#include <QMap>
#include <QObject>

class FocusPoint : public QPoint{
public:
    enum struct Position {
        Undefined,
        TopLeft,
        Top,
        TopRight,
        Right,
        BottomRight,
        Bottom,
        BottomLeft,
        Left,
        Body,
        Rotate
    };

    QMap<Position, QCursor> m_cursorMap{
        {Position::TopLeft, QCursor(Qt::SizeFDiagCursor)},
        {Position::BottomRight, QCursor(Qt::SizeFDiagCursor)},
        {Position::BottomLeft, QCursor(Qt::SizeBDiagCursor)},
        {Position::TopRight, QCursor(Qt::SizeBDiagCursor)},
        {Position::Top, QCursor(Qt::SizeVerCursor)},
        {Position::Bottom, QCursor(Qt::SizeVerCursor)},
        {Position::Left, QCursor(Qt::SizeHorCursor)},
        {Position::Right, QCursor(Qt::SizeHorCursor)},
        {Position::Body, QCursor(Qt::SizeAllCursor)},
        {Position::Rotate, QCursor(Qt::CrossCursor)}
    };

    FocusPoint();
    FocusPoint(int xpos, int ypos, Position position) noexcept
        : QPoint(xpos, ypos)
        , m_position{position} {

    }

    Position position() const;
    void setPosition(Position newPosition);

    QCursor cursor() {
        return m_cursorMap[m_position];
    }

private:
    Position m_position;
};
