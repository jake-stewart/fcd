#ifndef SOURCES_HPP
#define SOURCES_HPP

#include "source.hpp"
#include <vector>

class Sources
{
private:
    std::vector<Source*> m_sources;
    int                  m_largest_source_length;

public:
    void    sort(std::string query);
    size_t  size();
    Source* get(int idx);
    bool    read(char *src_path);
    int     get_largest_source_length();
};

#endif
