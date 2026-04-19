#include "../kitapch.h"
#include "StringUtil.h"

namespace Kita {
    void StringUtil::replaceAll(std::string& str, const std::string_view from, const std::string_view to) {
        if (from.empty()) return;

        size_t pos = 0;
        while ((pos = str.find(from, pos)) != std::string::npos) {
            str.replace(pos, from.length(), to);
            pos += to.length();
        }
    }
} // Kita
