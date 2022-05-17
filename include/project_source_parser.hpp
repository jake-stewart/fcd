#ifndef PROJECT_SOURCE_PARSER
#define PROJECT_SOURCE_PARSER

#include "source_parser.hpp"

class ProjectSourceParser : public SourceParser
{
private:
    fs::path m_project_path;

public:
    ProjectSourceParser(fs::path m_config_path, fs::path project_path);
    void generateSources(std::vector<fs::path>& paths);
    int readSources() override;
};

#endif
