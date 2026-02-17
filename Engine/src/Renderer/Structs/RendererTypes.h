#pragma once

namespace Kita {
    enum class Capability {
        DEPTH_TEST
    };

    enum class DepthFunction {
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
        STENCIL,
        DEPTH_STENCIL
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
