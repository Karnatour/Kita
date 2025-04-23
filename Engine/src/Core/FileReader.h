#pragma once
#include "DllTemplate.h"

namespace Kita {
    class KITAENGINE_API FileReader {
    public:
        static std::string readFile(const std::string& filePath);
    };
} // Kita
