#pragma once

#include <QPoint>
#include <QCursor>
#include <QMap>
#include <QObject>

class FocusPoint : public QPoint{
public:
    enum class Position {
        LeftTop,
        Top,
        RightTop,
        Right,
        RightBottom,
        Bottom,
        LeftBottom,
        Left,
        Center,
        UserDefined
    };

    QMap<Position, Qt::CursorShape> m_cursorMap{
        {Position::LeftTop, Qt::SizeFDiagCursor},
        {Position::RightBottom, Qt::SizeFDiagCursor},
        {Position::LeftBottom, Qt::SizeBDiagCursor},
        {Position::RightTop, Qt::SizeBDiagCursor},
        {Position::Top, Qt::SizeVerCursor},
        {Position::Bottom, Qt::SizeVerCursor},
        {Position::Left, Qt::SizeHorCursor},
        {Position::Right, Qt::SizeHorCursor},
        {Position::Center, Qt::SizeAllCursor},
        {Position::UserDefined, Qt::ArrowCursor}
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
