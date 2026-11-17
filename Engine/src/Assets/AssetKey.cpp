#include "AssetKey.h"

namespace Kita {
    AssetKey::AssetKey(const std::initializer_list<std::filesystem::path> paths) : m_paths(paths) {
        KITA_ENGINE_ASSERT(!m_paths.empty(), "[AssetKey] Key needs at least one path");
    }

    std::vector<std::filesystem::path> AssetKey::takePaths() && {
        return std::move(m_paths);
    }

    std::string AssetKey::getPathsCombined() const {
        return m_paths | std::views::transform([](const auto& p) { return p.string(); }) | std::views::join_with(' ') | std::ranges::to<std::string>();
    }

    bool AssetKey::isSingle() const {
        return m_paths.size() == 1;
    }

    std::string AssetKey::toString() const {
        std::string s;
        for (const auto& p : m_paths) {
            s += p.string();
            s += '|';
        }
        return s;
    }
} // Kita
