#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <string>
#include <vector>
#include <filesystem>

namespace fs = std::filesystem;

class Options
{
private:
    bool     m_is_project;
    fs::path m_project_path;
    fs::path m_config_path;
    fs::path m_history_path;
    fs::path m_output_path;

public:
    Options();

    void     parseArgs(int argc, char **argv);
    bool     addOption(std::string key, std::string val);

    void     setProjectPath(fs::path project_path);
    void     setConfigPath(fs::path config_path);
    void     setHistoryPath(fs::path history_path);
    void     setOutputPath(fs::path output_path);

    bool     isProject();

    fs::path getProjectPath();
    fs::path getConfigPath();
    fs::path getHistoryPath();
    fs::path getOutputPath();
};

#endif
