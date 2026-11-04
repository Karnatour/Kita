#include "VertexAttribute.h"

namespace Kita {
    VertexAttribute::VertexAttribute(const Type type, const Count size) : m_type(type), m_size(size) {
    }

    VertexAttribute::Type VertexAttribute::getType() const {
        return m_type;
    }

    VertexAttribute::Count VertexAttribute::getCount() const {
        return m_size;
    }

    size_t VertexAttribute::getTypeSize() const {
        switch (m_type) {
            case Type::BYTE:
                return 1;
            case Type::SHORT:
                return 2;
            case Type::FLOAT:
                return 4;
            default:
                KITA_ENGINE_ERROR("Trying to convert unknown type to GL variation {} ", magic_enum::enum_name(m_type));
                return 1;
        }
    }

    size_t VertexAttribute::getTotalSize() const {
        return getTypeSize() * static_cast<size_t>(getCount());
    }
} // Kita
