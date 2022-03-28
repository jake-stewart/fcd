#include "../include/source.hpp"
#include "../include/util.hpp"
#include "../include/levenshtein.h"

Source::Source(std::string name, std::string path, int color) {
    m_name = name;
    m_words = split(to_lower(name), ' ');
    m_path = path;
    m_color = color;
    m_heuristic = 0;
}

void Source::calcHeuristic(
        std::string& query,
        std::vector<std::string>& words
) {
    m_heuristic = levenshtein(query.c_str(), m_name.c_str());

    if (m_name.find(query) != std::string::npos) {
        m_heuristic -= 10000;
    }

    for (std::string word : words) {
        for (std::string comp_word : m_words) {
            if (comp_word.rfind(word, 0) == 0) {
                m_heuristic -= 80;
            }
        }
    }
}

int Source::getHeuristic() {
    return m_heuristic;
}

std::string& Source::getPath() {
    return m_path;
}

std::string& Source::getName() {
    return m_name;
}

int Source::getColor() {
    return m_color;
}
