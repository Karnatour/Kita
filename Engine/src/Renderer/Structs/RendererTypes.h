#pragma once

namespace Kita {
    enum class Capabilities {
        DEPTH_TEST
    };

    enum class DepthFunctions {
        NEVER,
        LESS,
        EQUAL,
        LEQUAL,
        GREATER,
        NOTEQUAL,
        GEQUAL,
        ALWAYS
    };

    enum class BufferType {
        COLOR,
        DEPTH,
        STENCIL
    };

    enum class ClearBit {
        COLOR,
        DEPTH,
        STENCIL
    };

    enum class CullMode {
        FRONT,
        BACK,
        FRONT_BACK
    };
}
