#include "../include/source.hpp"
#include "../include/util.hpp"
#include "../include/project_source_parser.hpp"

#include <fstream>

ProjectSourceParser::ProjectSourceParser(fs::path config_path, fs::path project_path) {
    m_config_path = config_path / "projects";
    m_history_path = config_path / "project_history";
    m_project_path = project_path;
}

void extractRootsAndIgnores(
        std::ifstream& file,
        std::string& project_root,
        std::vector<fs::path>& src_roots,
        std::set<fs::path>& ignores
) {
    std::string line;
    while (getNonEmptyLine(file, line)) {
        if (line[0] != '+' && line[0] != '-') {
            break;
        }
        bool add = line[0] == '+';
        line = line.substr(1, line.size() - 1);
        line = strip(line);
        fs::path src_path = project_root / fs::path(line);
        if (add) {
            src_roots.push_back(src_path);
        }
        else {
            ignores.insert(src_path);
        }
    }
}

void ProjectSourceParser::generateSources(std::vector<fs::path>& paths) {
    for (const fs::path& file : paths) {
        std::string name = file.parent_path().filename()
            .string() + '/' + file.filename().string();
        Source source;
        source.setName(name);
        source.setPath(file.string());
        int history = 0;
        auto it = m_history_lookup.find(file);
        if (it != m_history_lookup.end()) {
            history = it->second;
        }
        source.setHistory(history);
        m_sources.push_back(source);
    } 
}

int ProjectSourceParser::readSources() {
    std::ifstream projects_file(m_config_path);
    std::string project_root;
    std::set<fs::path> ignores;
    std::vector<fs::path> src_roots;
    std::vector<fs::path> paths;

    if (!projects_file.is_open()) {
        fprintf(stderr, "Error reading source project sources file '%s'\n",
                m_config_path.c_str());
        return 1;
    }

    if (!findLineStartsWith(m_project_path, project_root, projects_file)) {
        fprintf(stderr, "Could not match project.\n");
        projects_file.close();
        return 1;
    }

    m_history_path = (std::string)m_history_path + project_root;
    m_history_path /= "history";
    createHistoryLookup();

    extractRootsAndIgnores(projects_file, project_root, src_roots, ignores);
    projects_file.close();

    for (const fs::path& root : src_roots) {
        recursiveGetFiles(root, paths, ignores);
    }

    generateSources(paths);
    return 0;
}
