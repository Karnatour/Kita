#pragma once
#include "../../Core/DllTemplate.h"
#include "Shader.h"
#include <unordered_map>
#include <memory>

namespace Kita {

class KITAENGINE_API ShaderManager {
public:
    void addShader(Shader&& shader);
private:
    std::unordered_map<unsigned int,std::shared_ptr<Shader>> m_shaderMap;
};

} // Kita
