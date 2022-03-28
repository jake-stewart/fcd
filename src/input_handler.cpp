#include "../include/input_handler.hpp"

#define CTRL_H 8
#define CTRL_J 10
#define CTRL_K 11
#define CTRL_L 12
#define CTRL_U 21
#define ESC 27

bool InputHandler::is_active() {
    return m_active;
}

bool InputHandler::is_selected() {
    return m_selected;
}

int InputHandler::get_offset() {
    return m_offset;
}

std::string InputHandler::get_query() {
    return std::string(m_query);
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
            if (m_query_size)
                m_query_size--;
                m_query[m_query_size] = '\0';
            break;

        case CTRL_U:
            m_offset = 0;
            m_query_size = 0;
            m_query[0] = '\0';
            break;

        case CTRL_K:
            m_offset += 1;
            break;

        case CTRL_J:
            m_selected = true;
            m_active = false;
            break;

        default:
            break;
    }
}
