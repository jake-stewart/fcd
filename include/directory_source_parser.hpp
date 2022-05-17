#ifndef DIRECTORY_SOURCE_PARSER
#define DIRECTORY_SOURCE_PARSER

#include "source_parser.hpp"

class DirectorySourceParser : public SourceParser
{
public:
    DirectorySourceParser(fs::path config_path);
    int readSources() override;
};

#endif
