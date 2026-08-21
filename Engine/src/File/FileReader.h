#pragma once
#include "../Core/DllTemplate.h"
#include <optional>
#include <string>
#include <filesystem>
#include <stdint.h>

namespace Kita {
    class KITAENGINE_API FileReader {
    public:
        static std::optional<std::vector<char>> readFileRaw(const std::filesystem::path& path);
        static std::optional<std::string> readFileString(const std::filesystem::path& path);
        static std::optional<uint64_t> getFileHash(const std::filesystem::path& path);
    };
} // Kita
