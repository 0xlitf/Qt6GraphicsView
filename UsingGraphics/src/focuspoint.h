#pragma once

#include <QPoint>
#include <QCursor>
#include <QMap>
#include <QObject>

class FocusPoint : public QPoint{
public:
    enum struct Position {
        Undefined,
        LeftTop,
        Top,
        RightTop,
        Right,
        RightBottom,
        Bottom,
        LeftBottom,
        Left,
        Body,
        Rotate
    };

    QMap<Position, QCursor> m_cursorMap{
        {Position::LeftTop, QCursor(Qt::SizeFDiagCursor)},
        {Position::RightBottom, QCursor(Qt::SizeFDiagCursor)},
        {Position::LeftBottom, QCursor(Qt::SizeBDiagCursor)},
        {Position::RightTop, QCursor(Qt::SizeBDiagCursor)},
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
        // m_cursorMap.insert(Position::LeftTop, Qt::SizeFDiagCursor);
        // m_cursorMap.insert(Position::RightBottom, Qt::SizeFDiagCursor);
        // m_cursorMap.insert(Position::LeftBottom, Qt::SizeBDiagCursor);
        // m_cursorMap.insert(Position::RightTop, Qt::SizeBDiagCursor);
        // m_cursorMap.insert(Position::Top, Qt::SizeVerCursor);
        // m_cursorMap.insert(Position::Bottom, Qt::SizeVerCursor);
        // m_cursorMap.insert(Position::Left, Qt::SizeHorCursor);
        // m_cursorMap.insert(Position::Right, Qt::SizeHorCursor);
        // m_cursorMap.insert(Position::Center, Qt::SizeAllCursor);
        // m_cursorMap.insert(Position::UserDefined, Qt::ArrowCursor);
    }

    Position position() const;
    void setPosition(Position newPosition);

    QCursor cursor() {
        return m_cursorMap[m_position];
    }

private:
    Position m_position;
};
