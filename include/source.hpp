#ifndef SOURCE_HPP
#define SOURCE_HPP

#include <string>
#include <vector>

class Source
{
private:
    std::string               m_name;
    std::vector<std::string>  m_words;
    std::string               m_path;
    int                       m_color;
    int                       m_heuristic;

public:
    Source(std::string name, std::string path, int color);

    void         calcHeuristic(std::string& query,
                               std::vector<std::string>& words);
    int          getHeuristic();
    std::string& getPath();
    std::string& getName();
    int          getColor();
};

#endif
