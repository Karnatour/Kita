#pragma once
#include "Transformation.h"
#include "../RendererAPI.h"
#include "Primitives/Model.h"
#include "../../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API Entity {
    public:
        Entity();
        Entity(const Entity& original);
        explicit Entity(const std::shared_ptr<Model>& model);
        Entity(const std::shared_ptr<Model>& model, const Transformation& transformation);
        virtual ~Entity() = default;

        virtual bool onRender(RendererAPI& rendererApi);

        void setModel(const std::shared_ptr<Model>& model);
        void setTransformation(const Transformation& transformation);

        bool shouldRenderOnce() const;
        bool isFirstFrame() const;
        void setFirstFrame(bool isFirstFrame);
        std::shared_ptr<Model>& getModel();
        Transformation& getTransformation();

        unsigned int getID() const;

        std::shared_ptr<Entity> clone() const;

    protected:
        inline static unsigned int m_idCounter = 0;
        unsigned int m_id;
        std::shared_ptr<Model> m_model;
        Transformation m_transformation;
        bool m_renderOnce = false;
        bool m_firstFrame = true;
    };
} // Kita
