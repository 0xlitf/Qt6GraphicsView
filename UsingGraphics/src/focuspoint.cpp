#include "focuspoint.h"

FocusPointF::FocusPointF() {

}

FocusPointF::Position FocusPointF::position() const {
    return m_position;
}

void FocusPointF::setPosition(Position newPosition) {
    m_position = newPosition;
}
