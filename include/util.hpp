#ifndef UTIL_HPP
#define UTIL_HPP

#include <string>
#include <vector>

std::vector<std::string> split(std::string str, char delim);
std::string              to_lower(std::string s);
std::string&             strip(std::string &str);
void                     print_usage();

#endif
