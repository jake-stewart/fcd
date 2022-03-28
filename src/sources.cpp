#include "../include/sources.hpp"
#include "../include/util.hpp"

#include <fstream>

bool compare_sources(Source* l, Source* r) {
    return (l->getHeuristic() < r->getHeuristic());
}

bool compare_sources_alpha(Source* l, Source* r) {
    return (l->getName() > r->getName());
}

void Sources::sort(std::string query) {
    if (query.size() > 0) {

        std::string lower = to_lower(query);
        std::vector<std::string> words = split(lower, ' ');

        for (Source *source : m_sources) {
            source->calcHeuristic(lower, words);
        }

        std::sort(m_sources.begin(), m_sources.end(), compare_sources);
    }
    else {
        std::sort(m_sources.begin(), m_sources.end(), compare_sources_alpha);
    }
}

size_t Sources::size() {
    return m_sources.size();
}

Source* Sources::get(int idx) {
    return m_sources[idx];
}

bool Sources::read(char *src_path) {
    std::string line;
    std::ifstream file(src_path);

    if (!file) {
        return false;
    }

    std::string name;

    while (std::getline(file, line)) {
        strip(line);
        if (!line.size()) {
            continue;
        }
        if (name.size()) {
            m_sources.push_back(new Source(name, line, 4));
            name.clear();
        }
        else {
            name = line;
            if (name.size() > m_largest_source_length) {
                m_largest_source_length = name.size();
            }
        }
    }
    file.close();
    return true;
}

int Sources::get_largest_source_length() {
    return m_largest_source_length;
}
