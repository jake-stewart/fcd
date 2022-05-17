#ifndef SOURCES_HPP
#define SOURCES_HPP

#include "source.hpp"
#include <vector>

class Sources
{
private:
    std::vector<Source> m_sources;
    bool                m_has_heuristic   = false;
    int                 m_last_query_size = 0;

    void    resetHeuristics();

public:
    void    updateHistory(int selected_index);
    void    sort(std::string query);
    size_t  size();
    Source& get(int idx);
    bool    read(char *src_path);
    int     getLargestSourceLength();
    bool    hasHeuristic();
    void    push_back(Source& source);

    // auto    begin() { return m_sources.begin(); }
    // auto    end() { return m_sources.end(); }
};

#endif
