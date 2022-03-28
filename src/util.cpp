#include "../include/util.hpp"

void print_usage() {
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

std::string to_lower(std::string s) {
    for(char &c : s)
        c = tolower(c);
    return s;
}

std::string& strip(std::string &str) {
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
