#include "focuspoint.h"

FocusPoint::FocusPoint() {
}

FocusPoint::Position FocusPoint::position() const {
    return m_position;
}

void FocusPoint::setPosition(Position newPosition) {
    m_position = newPosition;
}
