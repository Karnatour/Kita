#pragma once
#include "../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API FileReader {
    public:
        static std::string readFile(const std::filesystem::path& path);
    };
} // Kita
