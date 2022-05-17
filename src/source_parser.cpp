#include "../include/source_parser.hpp"
#include "../include/util.hpp"

#include <fstream>
#include <map>

// SourceParser::SourceParser(fs::path config_path) {
//     m_config_path = config_path;
// }

int SourceParser::createHistoryLookup() {
    std::ifstream history_file(m_history_path);
    // printf("project root: %s\n", project_root.c_str());
    if (!history_file.is_open()) {
        return 1;
    }

    std::string line;
    std::string src_path;
    int history;

    while (getNonEmptyLine(history_file, line)) {
        src_path = line;
        if (getNonEmptyLine(history_file, line)) {
            sscanf(line.c_str(), "%d", &history);
            m_history_lookup[src_path] = history;
        }
    }
    history_file.close();

    return 0;
}

int SourceParser::writeHistory(Sources& sources) {
    fs::create_directories(m_history_path.parent_path());
    std::ofstream history_file(m_history_path);
    if (!history_file.is_open()) {
        return 1;
    }
    for (int i = 0; i < sources.size(); i++) {
        history_file << sources.get(i).getPath() << std::endl;
        history_file << sources.get(i).getHistory() << std::endl;
    }
    history_file.close();

    return 0;
}

Sources& SourceParser::getSources() {
    return m_sources;
}
