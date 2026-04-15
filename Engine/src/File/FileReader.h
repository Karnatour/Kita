#pragma once
#include "../Core/DllTemplate.h"
#include <optional>
#include <string>
#include <filesystem>

namespace Kita {
    class KITAENGINE_API FileReader {
    public:
        static std::optional<std::string> readFile(const std::filesystem::path& path);
    };
} // Kita
