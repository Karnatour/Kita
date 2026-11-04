#pragma once
#include "../src/Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API VertexAttribute {
    public:
        enum class Count {
            ONE = 1,
            TWO = 2,
            THREE = 3,
            FOUR = 4
        };

        enum class Type {
            BYTE,
            SHORT,
            FLOAT,
        };

        VertexAttribute(Type type, Count size);

        Type getType() const;
        Count getCount() const;
        size_t getTypeSize() const;
        size_t getTotalSize() const;

    private:
        Type m_type;
        Count m_size;
    };
} // Kita
