#pragma once
#include "Transformation.h"
#include "Primitives/Model.h"
#include "../../Core/DllTemplate.h"

namespace Kita {
    class KITAENGINE_API Entity {
    public:
        Entity();
        Entity(const Entity& original);
        Entity(const std::shared_ptr<Model>& model);
        Entity(const std::shared_ptr<Model>& model, const Transformation& transformation);
        void setModel(const std::shared_ptr<Model>& model);
        void setTransformation(const Transformation& transformation);

        std::shared_ptr<Model>& getModel();
        Transformation& getTransformation();

        unsigned int getID() const;

        std::shared_ptr<Entity> clone() const;
    private:
        inline static unsigned int m_idCounter = 0;
        unsigned int m_id;
        std::shared_ptr<Model> m_model;
        Transformation m_transformation;
    };
} // Kita
