#pragma once

namespace Kita {
    enum class Capability {
        DEPTH_TEST,
        CULL_FACE,
        BLEND
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

    enum class DrawMode {
        POINTS,
        LINES,
        TRIANGLES
    };

    enum class BlendFunc {
        ZERO,
        ONE,
        SRC_COLOR,
        ONE_MINUS_SRC_COLOR,
        DST_COLOR,
        ONE_MINUS_DST_COLOR,
        SRC_ALPHA,
        ONE_MINUS_SRC_ALPHA,
        DST_ALPHA,
        ONE_MINUS_DST_ALPHA,
        CONSTANT_COLOR,
        ONE_MINUS_CONSTANT_COLOR,
        CONSTANT_ALPHA,
        ONE_MINUS_CONSTANT_ALPHA
    };
}
