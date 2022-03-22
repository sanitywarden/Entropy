#ifndef _GLOBAL_UTILITIES_HPP_
#define _GLOBAL_UTILITIES_HPP_

#include <string>

namespace iso {
    bool startsWith(const std::string& str, const std::string& phrase);
    bool containsWord(const std::string& str, const std::string& phrase);
    std::string toLower(const std::string& str);
}

#endif