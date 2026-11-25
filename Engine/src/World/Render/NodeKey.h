#pragma once
#include "../../Core/Hash.h"
#include <functional>

namespace Kita {
    struct NodeKey {
        int lodLevel = 0;
        int chunkPosX = 0;
        int chunkPosZ = 0;
        bool operator==(const NodeKey& other) const = default;
    };
} // Kita

template <>
struct std::hash<Kita::NodeKey> {
    size_t operator()(const Kita::NodeKey& nodeKey) const noexcept {
        size_t seed = 0;

        seed = Kita::Hash::hashCombine(seed, std::hash<int>{}(nodeKey.lodLevel));
        seed = Kita::Hash::hashCombine(seed, std::hash<int>{}(nodeKey.chunkPosX));
        seed = Kita::Hash::hashCombine(seed, std::hash<int>{}(nodeKey.chunkPosZ));

        return seed;
    }
};
