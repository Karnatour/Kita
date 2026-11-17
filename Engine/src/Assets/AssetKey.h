#pragma once

#include <filesystem>
#include <span>
#include <string>
#include <vector>

#include "../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API AssetKey {
    public:
        AssetKey(std::initializer_list<std::filesystem::path> paths);

        std::vector<std::filesystem::path> takePaths() &&;
        std::string getPathsCombined() const;
        bool isSingle() const;

        std::string toString() const;

    private:
        std::vector<std::filesystem::path> m_paths;
    };
} // Kita
