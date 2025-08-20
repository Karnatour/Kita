#pragma once

#include "../Core/DllTemplate.h"
#include "../Renderer/Model.h"

namespace Kita {
    class KITAENGINE_API AssetImporter {
        std::shared_ptr<Model> importModel(const std::string& path);
    };
} // Kita