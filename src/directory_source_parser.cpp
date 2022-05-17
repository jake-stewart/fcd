#include "../include/source.hpp"
#include "../include/util.hpp"
#include "../include/directory_source_parser.hpp"

#include <fstream>
#include <iostream>

DirectorySourceParser::DirectorySourceParser(fs::path config_path) {
    m_config_path = config_path / "sources";
    m_history_path = config_path / "history";
}

int DirectorySourceParser::readSources() {
    createHistoryLookup();
    std::ifstream sources_file(m_config_path);
    if (!sources_file.is_open()) {
        fprintf(stderr, "Error reading sources file '%s'\n",
                m_config_path.c_str());
        return 1;
    }

    std::string line;
    while (getNonEmptyLine(sources_file, line)) {
        Source source;
        source.setName(line);
        if (getNonEmptyLine(sources_file, line)) {
            source.setPath(line);
            auto it = m_history_lookup.find(line);
            if (it != m_history_lookup.end()) {
                source.setHistory(it->second);
            }
            m_sources.push_back(source);
        }
    }
    sources_file.close();

    return 0;
}

