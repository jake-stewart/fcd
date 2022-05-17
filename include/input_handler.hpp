#ifndef INPUT_HANDLER
#define INPUT_HANDLER

#include <string>

const int MAX_QUERY_SIZE = 256;

class InputHandler
{
private:
    char m_query[MAX_QUERY_SIZE];
    int  m_query_size   = 0;
    int  m_offset       = 0;
    int  m_offset_range = 0;
    bool m_active       = true;
    bool m_selected     = false;


public:
    bool        isActive();
    bool        isSelected();
    int         getOffset();
    std::string getQuery();
    void        handle(char c);
    void        setOffsetRange(int offset_range);
    void        scrollOffset(int delta);
};

#endif
