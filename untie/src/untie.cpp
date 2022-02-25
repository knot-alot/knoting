#include "untie.h"

#include <knoting/game_object.h>
#include <knoting/log.h>
#include <knoting/mesh.h>
#include <knoting/px_variables_wrapper.h>
#include <knoting/scene.h>
#include <knoting/texture.h>

#include <knoting/components.h>
#include <knoting/scene.h>

#include <iostream>

namespace knot {
Scene scene;
Untie::Untie() {
    Scene::set_active_scene(scene);
    log::Logger::setup();

    m_engine = std::make_unique<knot::Engine>();
    Engine::set_active_engine(*m_engine);
    std::shared_ptr<PxPhysics_ptr_wrapper> px_physics = m_engine->get_physics_module().lock()->get_physics().lock();
    std::shared_ptr<PxScene_ptr_wrapper> px_scene = m_engine->get_physics_module().lock()->get_active_Scene().lock();
    {
        auto editorCamera = scene.create_game_object("camera");
        auto& cam = editorCamera.add_component<components::EditorCamera>();
        editorCamera.get_component<components::Transform>().set_position(glm::vec3(-10.0f, 15.0f, -30.0f));
    }

    {
        auto cubeObj = scene.create_game_object("ground");
        cubeObj.get_component<components::Transform>().set_position(glm::vec3(0, -10, 0));
        cubeObj.get_component<components::Transform>().set_scale(glm::vec3(15.0f, 1.0f, 15.0f));
        auto& mesh = cubeObj.add_component<components::Mesh>();
        mesh.create_cube();
        auto material = cubeObj.add_component<components::Material>();
        auto& physics_material = cubeObj.add_component<components::PhysicsMaterial>();
        auto& shape = cubeObj.add_component<components::Shape>();
        vec3 halfsize = vec3(15.0, 2.0f, 15.0);
        shape.set_geometry(shape.create_cube_geometry(halfsize));

        auto& rigidbody = cubeObj.add_component<components::RigidBody>();

        rigidbody.create_actor(false);
    }
    {
        auto cubeObj = scene.create_game_object("cube_1");
        cubeObj.get_component<components::Transform>().set_position(glm::vec3(0.0f, 3.0f, 0.0f));
        auto& mesh = cubeObj.add_component<components::Mesh>();
        mesh.create_cube();
        auto material = cubeObj.add_component<components::Material>();
        auto& physics_material = cubeObj.add_component<components::PhysicsMaterial>();

        auto& shape = cubeObj.add_component<components::Shape>();
        vec3 halfsize = vec3(1.0f, 1.0f, 1.0f);
        shape.set_geometry(shape.create_cube_geometry(halfsize));

        auto& rigidbody = cubeObj.add_component<components::RigidBody>();

        rigidbody.create_actor(true, 5.0f);
    }
    {
        auto cubeObj = scene.create_game_object("cube_0");
        cubeObj.get_component<components::Transform>().set_position(glm::vec3(1.0f, 7.0f, 1.0f));
        auto& mesh = cubeObj.add_component<components::Mesh>();
        mesh.create_cube();
        auto material = cubeObj.add_component<components::Material>();
        auto& physics_material = cubeObj.add_component<components::PhysicsMaterial>();
        auto& shape = cubeObj.add_component<components::Shape>();
        vec3 halfsize = vec3(1.0f, 1.0f, 1.0f);
        shape.set_geometry(shape.create_cube_geometry(halfsize));
        auto& rigidbody = cubeObj.add_component<components::RigidBody>();
        rigidbody.create_actor(true, 5.0f);
    }
}
void Untie::run() {
    log::debug("RUN");
    while (m_engine->is_open()) {
        m_engine->update_modules();
    }
}

}  // namespace knot
