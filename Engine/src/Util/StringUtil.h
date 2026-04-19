#pragma once
#include "../Core/DllTemplate.h"
#include <string>

namespace Kita {
    class KITAENGINE_API StringUtil {
    public:
        static void replaceAll(std::string& str,std::string_view from,std::string_view to);
    };
} // Kita