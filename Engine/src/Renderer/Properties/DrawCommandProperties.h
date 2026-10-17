#pragma once

namespace Kita {
    struct DrawArraysIndirectCommandProperties {
        unsigned int count;
        unsigned int instanceCount;
        unsigned int firstVertex;
        unsigned int baseInstance;
    };

    struct DrawElementsIndirectCommandProperties {
        unsigned int count;
        unsigned int instanceCount;
        unsigned int firstIndex;
        int baseVertex;
        unsigned int baseInstance;
    };
} // Kita
