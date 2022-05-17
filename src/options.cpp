#include "../include/options.hpp"
#include "../include/util.hpp"

Options::Options() {
    fs::path home = getenv("HOME");
    m_config_path = home / ".config" / "fcd";
    m_output_path = home / ".cache" / "fcd_dest";
    m_is_project = false;
}

bool Options::addOption(std::string key, std::string val) {
    if (key == "--project") {
        m_is_project = true;
        m_project_path = expandUserPath(val);
    }
    else if (key == "--config") {
        m_config_path = expandUserPath(val);
    }
    else if (key == "--output") {
        m_output_path = expandUserPath(val);
    }
    else {
        return false;
    }
    return true;
}

void Options::parseArgs(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        std::string lhs;
        std::string rhs;
        std::string arg = argv[i];

        int it = arg.find("=");
        if (it != std::string::npos) {
            lhs = arg.substr(0, it);
            rhs = arg.substr(it + 1, arg.size() - it - 1);
        }
        else {
            lhs = argv[i];
            if (++i < argc) {
                rhs = argv[i];
            }
        }

        lhs = strip(lhs);
        rhs = strip(rhs);

        if (!addOption(lhs, rhs)) {
            fprintf(stderr, "'%s' is an unknown option.\n", lhs.c_str());
            exit(1);
        }
        if (!rhs.size()) {
            fprintf(stderr, "'%s' expected a value.\n", lhs.c_str());
            exit(1);
        }
    }
}

void Options::setProjectPath(fs::path project_path) {
    m_is_project = true;
    m_project_path = project_path;
}

void Options::setConfigPath(fs::path config_path) {
    m_config_path = config_path;
}

void Options::setOutputPath(fs::path output_path) {
    m_output_path = output_path;
}

bool Options::isProject() {
    return m_is_project;
}

fs::path Options::getProjectPath() {
    return m_project_path;
}

fs::path Options::getConfigPath() {
    return m_config_path;
}

fs::path Options::getOutputPath() {
    return m_output_path;
}
