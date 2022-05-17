#include "../include/sources.hpp"
#include "../include/util.hpp"

#include <fstream>

bool compareSources(Source& l, Source& r) {
    return (l.getHeuristic() < r.getHeuristic());
}

bool compareSourcesAlpha(Source& l, Source& r) {
    return (l.getName() > r.getName());
}

bool Sources::hasHeuristic() {
    return m_has_heuristic;
}

void Sources::resetHeuristics() {
    for (Source& source : m_sources) {
        source.setHeuristic(0);
    }
}

void Sources::sort(std::string query) {
    if (query.size() < m_last_query_size) {
        resetHeuristics();
    }
    m_last_query_size = query.size();

    std::string lower = toLower(query);
    std::vector<std::string> words = camelSplit(lower);

    if (m_has_heuristic) {
        for (int i = 0; i < m_sources.size(); i++) {
            if (m_sources[i].getHeuristic() == INT_MAX) {
                break;
            }
            m_sources[i].calcHeuristic(lower, words);
        }
    }
    else {
        for (Source& source : m_sources) {
            source.calcHeuristic(lower, words);
        }
    }
    std::sort(m_sources.begin(), m_sources.end(), compareSources);
    m_has_heuristic = true;
}

size_t Sources::size() {
    return m_sources.size();
}

Source& Sources::get(int idx) {
    return m_sources[idx];
}

void Sources::push_back(Source& source) {
    return m_sources.push_back(source);
}

bool Sources::read(char *src_path) {
    std::string line;
    std::ifstream file(src_path);

    if (!file) {
        return false;
    }

    int state = 0;
    std::string name;
    std::string source_path;
    int history = 0;

    while (std::getline(file, line)) {
        strip(line);
        if (!line.size()) {
            continue;
        }
        switch (state) {
            case 0:
                name = line;
                state++;
                break;
            case 1:
                source_path = line;
                state++;
                break;
            case 2:
                sscanf(line.c_str(), "%d", &history);
                Source source;
                source.setName(name);
                source.setPath(src_path);
                source.setColor(4);
                source.setHistory(history);
                m_sources.push_back(source);
                state = 0;
                break;
        }
    }
    file.close();
    return true;
}

void Sources::updateHistory(int selected_index) {
    for (int i = 0; i < m_sources.size(); i++) {
        Source& source = m_sources[i];
        if (i == selected_index) {
            source.setHistory(10);
        }
        else {
            int history = source.getHistory() - 1;
            if (history < 0) {
                history = 0;
            }
            source.setHistory(history);
        }
    }
}
