#include "../../kitapch.h"
#include "Entity.h"

namespace Kita {
    Entity::Entity() {
        m_id = ++m_idCounter;
    }

    Entity::Entity(const Entity& original) : m_model(original.m_model) {
        m_id = ++m_idCounter;
    }

    Entity::Entity(const std::shared_ptr<Model>& model) : m_model(model) {
        m_id = ++m_idCounter;
    }

    Entity::Entity(const std::shared_ptr<Model>& model, const Transformation& transformation) : m_model(model), m_transformation(transformation) {
        m_id = ++m_idCounter;
    }

    bool Entity::onRender(RendererAPI& rendererApi) {
        return false;
    }

    void Entity::setModel(const std::shared_ptr<Model>& model) {
        m_model = model;
    }

    void Entity::setTransformation(const Transformation& transformation) {
        m_transformation = transformation;
    }

    bool Entity::shouldRenderOnce() const {
        return m_renderOnce;
    }

    bool Entity::isFirstFrame() const {
        return m_firstFrame;
    }

    std::shared_ptr<Model>& Entity::getModel() {
        return m_model;
    }

    Transformation& Entity::getTransformation() {
        return m_transformation;
    }

    unsigned int Entity::getID() const {
        return m_id;
    }

    std::shared_ptr<Entity> Entity::clone() const {
        return std::make_shared<Entity>(*this);
    }

    void Entity::setFirstFrame(const bool isFirstFrame) {
        m_firstFrame = isFirstFrame;
    }
} // Kita
