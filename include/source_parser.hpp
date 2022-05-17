#ifndef SOURCE_PARSER
#define SOURCE_PARSER

#include "source.hpp"
#include "sources.hpp"
#include <vector>
#include <string>
#include <filesystem>
#include <map>

namespace fs = std::filesystem;

class SourceParser
{
protected:
    fs::path m_config_path;
    fs::path m_history_path;
    Sources  m_sources;

    std::map<std::string, int> m_history_lookup;
    int createHistoryLookup();

public:
    Sources& getSources();
    virtual int readSources() = 0;
    int writeHistory(Sources& sources);
};

#endif
