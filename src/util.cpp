#include "../include/util.hpp"

void printUsage() {
    printf("usage: fcd <sources_file> <output_file>\n");
}

std::vector<std::string> split(std::string str, char delim) {
    std::vector<std::string> words;
    std::string word="";

    for(char c : str) {
        if(c == delim){
           words.push_back(word);
           word="";
        }
        else {
           word += c;
        }
    }
    if (word.size() > 0) {
        words.push_back(word);
    }
    return words;
}

std::string toLower(std::string s) {
    for(char &c : s)
        c = tolower(c);
    return s;
}

std::string& strip(std::string& str) {
    int l_idx = 0;
    int r_idx = str.size() - 1;

    while (r_idx >= 0 && (str[r_idx] == ' ' || str[r_idx] == '\t')) {
        r_idx--;
    }
    while (l_idx < r_idx && (str[l_idx] == ' ' || str[l_idx] == '\t')) {
        l_idx++;
    }

    str.erase(r_idx + 1, str.size());
    str.erase(0, l_idx);
    return str;
}

std::vector<std::string> camelSplit(std::string& text) {
    std::vector<std::string> results;

    std::string v(text);

    int idx = 0;
    int size = 0;
    bool upper = false;
     
    for (char c : text) {
        switch (c) {
            case 'a' ... 'z':
                if (upper && size > 1) {
                    results.push_back(v.substr(idx, size - 1));
                    idx += (size - 1);
                    size = 1;
                }
                size++;
                upper = false;
                break;
            case 'A' ... 'Z':
                if (!upper && size > 1) {
                    results.push_back(v.substr(idx, size));
                    idx += size;
                    size = 0;
                }
                size++;
                upper = true;
                break;
            default:
                if (size) {
                    results.push_back(v.substr(idx, size));
                    idx += size;
                    size = 0;
                }
                idx++;
                break;
        }
    }
    if (size) {
        results.push_back(v.substr(idx, size));
    }

    return results;
}

std::vector<std::string> camelSplitLower(std::string& text, std::string& lower) {
    std::vector<std::string> results;

    std::string v(lower);

    int idx = 0;
    int size = 0;
    bool upper = false;
     
    for (char c : text) {
        switch (c) {
            case 'a' ... 'z':
                if (upper && size > 1) {
                    results.push_back(v.substr(idx, size - 1));
                    idx += (size - 1);
                    size = 1;
                }
                size++;
                upper = false;
                break;
            case 'A' ... 'Z':
                if (!upper && size > 1) {
                    results.push_back(v.substr(idx, size));
                    idx += size;
                    size = 0;
                }
                size++;
                upper = true;
                break;
            default:
                if (size) {
                    results.push_back(v.substr(idx, size));
                    idx += size;
                    size = 0;
                }
                idx++;
                break;
        }
    }
    if (size) {
        results.push_back(v.substr(idx, size));
    }

    return results;
}

bool getNonEmptyLine(std::istream& is, std::string& line) {
    while (true) {
        if (!std::getline(is, line)) {
            return false;
        }
        line = strip(line);
        if (line.size()) {
            return true;
        }
    }
}

void recursiveGetFiles(
        fs::path               root,
        std::vector<fs::path>& paths,
        std::set<fs::path>&    ignores
) {
    for (const fs::directory_entry& p : fs::directory_iterator(root)) {
        if (ignores.contains(p.path())) {
            continue;
        }
        if (fs::is_directory(p)) {
            recursiveGetFiles(p, paths, ignores);
        }
        else {
            paths.push_back(p.path());
        }
    }
}

bool findLineStartsWith(
        std::string target,
        std::string& line,
        std::istream& file
) {
    while (getNonEmptyLine(file, line)) {
        if (target.starts_with(line)) {
            return true;
        }
    }
    return false;
}

fs::path expandUserPath(std::string user_path) {
    if (user_path.starts_with("~/")) {
        return getenv("HOME") / fs::path(user_path.substr(2));
    }
    return user_path;
}
