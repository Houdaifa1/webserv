#ifndef FILEUTILS_HPP
#define FILEUTILS_HPP

#include <string>
#include <sys/stat.h>
#include <unistd.h>

bool dir_exists(const std::string &path);
bool is_writable_dir(const std::string &path);
std::string get_parent_dir(const std::string &path);

#endif