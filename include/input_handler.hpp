#ifndef INPUT_HANDLER
#define INPUT_HANDLER

#include <string>

const int MAX_QUERY_SIZE = 256;

class InputHandler
{
private:
    char m_query[MAX_QUERY_SIZE];
    int  m_query_size = 0;
    int  m_offset     = 0;
    bool m_active     = true;
    bool m_selected   = false;

public:
    bool        is_active();
    bool        is_selected();
    int         get_offset();
    std::string get_query();
    void        handle(char c);
};

#endif
