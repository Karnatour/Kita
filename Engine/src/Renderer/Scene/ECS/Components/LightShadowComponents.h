#pragma once
#include "../../../Properties/LightShadowProperties.h"

namespace Kita {
    struct LightComponent {
        LightProperties properties;
    };

    struct DirectionalShadowComponent {
        DirectionalShadowProperties properties;
    };

    struct CastsShadows {

    };

} //Kita