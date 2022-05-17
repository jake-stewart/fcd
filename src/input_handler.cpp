#include "../include/input_handler.hpp"

#define CTRL_H 8
#define CTRL_J 10
#define CR     13
#define CTRL_K 11
#define CTRL_L 12
#define CTRL_U 21
#define ESC    27

bool InputHandler::isActive() {
    return m_active;
}

bool InputHandler::isSelected() {
    return m_selected;
}

int InputHandler::getOffset() {
    return m_offset;
}

std::string InputHandler::getQuery() {
    return std::string(m_query);
}

void InputHandler::scrollOffset(int delta) {
    if (m_offset_range) {
        m_offset = (m_offset + delta) % m_offset_range;
    }
}

void InputHandler::setOffsetRange(int offset_range) {
    m_offset_range = offset_range;
}

void InputHandler::handle(char c) {
    switch (c) {
        case ESC:
            m_active = false;
            break;

        // match every keyboard character including symbols
        case 32 ... 126:
            m_offset = 0;
            if (m_query_size < MAX_QUERY_SIZE) {
                m_query[m_query_size++] = c;
                m_query[m_query_size] = '\0';
            }
            break;

        case 127:
        case CTRL_H:
            m_offset = 0;

            if (m_query_size) {
                m_query_size--;
                m_query[m_query_size] = '\0';
            }

            break;

        case CTRL_U:
            m_offset = 0;
            m_query_size = 0;
            m_query[0] = '\0';
            break;

        case CTRL_K:
            scrollOffset(1);
            break;

        case CTRL_J:
        case CR:
            m_selected = true;
            m_active = false;
            break;

        default:
            break;
    }
}
