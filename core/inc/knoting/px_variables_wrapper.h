#pragma once

#include <PxPhysicsAPI.h>
#include <iostream>
#include <vector>

using namespace physx;

namespace knot {
class PxScene_ptr_wrapper {
   public:
    PxScene_ptr_wrapper(PxScene* scene = nullptr) : m_pxScene(scene) {}
    ~PxScene_ptr_wrapper() {
        if (m_pxScene) {
            m_pxScene->release();
            m_pxScene = nullptr;
        }
    }

    void set(PxScene* scene) { m_pxScene = scene; }

    PxScene* get() { return m_pxScene; }

   private:
    PxScene* m_pxScene;
};

class PxPhysics_ptr_wrapper {
   public:
    PxPhysics_ptr_wrapper(PxPhysics* physics = nullptr) : m_pxPhysics(physics) {}
    ~PxPhysics_ptr_wrapper() { clean(); }

    void set(PxPhysics* physics) {
        clean();
        m_pxPhysics = physics;
    }

    PxPhysics* get() { return m_pxPhysics; }

    void clean() {
        if (m_pxPhysics) {
            m_pxPhysics->release();
            m_pxPhysics = nullptr;
        }
    }

   private:
    PxPhysics* m_pxPhysics;
};
class PxFoundation_ptr_wrapper {
   public:
    PxFoundation_ptr_wrapper(PxFoundation* foundation = nullptr) : m_foundation(foundation) {}
    ~PxFoundation_ptr_wrapper() { clean(); }
    void set(PxFoundation* foundation) {
        clean();
        m_foundation = foundation;
    }

    PxFoundation* get() { return m_foundation; }

    void clean() {
        if (m_foundation) {
            m_foundation->release();
            m_foundation = nullptr;
        }
    }

   private:
    PxFoundation* m_foundation;
};
class PxDispatcher_ptr_wrapper {
   public:
    PxDispatcher_ptr_wrapper(PxDefaultCpuDispatcher* dispatcher = nullptr) : m_dispatcher(dispatcher) {}
    ~PxDispatcher_ptr_wrapper() { clean(); }
    void set(PxDefaultCpuDispatcher* dispatcher) {
        clean();
        m_dispatcher = dispatcher;
    }

    PxDefaultCpuDispatcher* get() { return m_dispatcher; }

    void clean() {
        if (m_dispatcher) {
            m_dispatcher->release();
            m_dispatcher = nullptr;
        }
    }

   private:
    PxDefaultCpuDispatcher* m_dispatcher;
};

class PxDynamic_ptr_wrapper {
   public:
    PxDynamic_ptr_wrapper(PxRigidDynamic* dynamic = nullptr) : m_dynamic(dynamic) {}
    ~PxDynamic_ptr_wrapper() { clean(); }
    void set(PxRigidDynamic* dynamic) {
        clean();
        m_dynamic = dynamic;
    }

    PxRigidDynamic* get() { return m_dynamic; }

    void clean() {
        if (m_dynamic) {
            m_dynamic->release();
            m_dynamic = nullptr;
        }
    }

   private:
    PxRigidDynamic* m_dynamic;
};

class PxStatic_ptr_wrapper {
   public:
    PxStatic_ptr_wrapper(PxRigidStatic* s = nullptr) : m_static(s) {}
    ~PxStatic_ptr_wrapper() { clean(); }
    void set(PxRigidStatic* s) {
        clean();
        m_static = s;
    }

    PxRigidStatic* get() { return m_static; }

    void clean() {
        if (m_static) {
            m_static->release();
            m_static = nullptr;
        }
    }

   private:
    PxRigidStatic* m_static;
};

class PxShape_ptr_wrapper {
   public:
    PxShape_ptr_wrapper(PxShape* shape = nullptr) : m_shape(shape) {}
    ~PxShape_ptr_wrapper() { clean(); }
    void set(PxShape* shape) {
        clean();
        m_shape = shape;
    }

    PxShape* get() { return m_shape; }

    void clean() {
        if (m_shape) {
            m_shape->release();
            m_shape = nullptr;
        }
    }

   private:
    PxShape* m_shape;
};

class PxMaterial_ptr_wrapper {
   public:
    PxMaterial_ptr_wrapper(PxMaterial* material = nullptr) : m_material(material) {}
    ~PxMaterial_ptr_wrapper() { clean(); }
    void set(PxMaterial* material) {
        clean();
        m_material = material;
    }

    PxMaterial* get() { return m_material; }

    void clean() {
        if (m_material) {
            m_material->release();
            m_material = nullptr;
        }
    }

   private:
    PxMaterial* m_material;
};

class PxAggregate_ptr_wrapper {
   public:
    PxAggregate_ptr_wrapper(PxAggregate* aggregate = nullptr, const std::string& name = "")
        : m_aggregate(aggregate), m_aggregate_name(name) {}
    ~PxAggregate_ptr_wrapper() { clean(); }
    void set(PxAggregate* aggregate, std::string name) {
        clean();
        m_aggregate = aggregate;
        m_aggregate_name = name;
    }
    PxAggregate* get_aggregate() { return m_aggregate; }
    std::string get_name() { return m_aggregate_name; }
    void clean() {
        if (m_aggregate) {
            m_aggregate->release();
            m_aggregate = nullptr;
        }
    }

   private:
    PxAggregate* m_aggregate;
    std::string m_aggregate_name;
};

class PxActor_ptr_wrapper {
   public:
    PxActor_ptr_wrapper(PxActor* actor = nullptr) : m_actor(actor) {}
    ~PxActor_ptr_wrapper() { clean(); }

    void set(PxActor* actor) {
        clean();
        m_actor = actor;
    }

    PxActor* get() { return m_actor; }

    void clean() {
        if (m_actor) {
            m_actor->release();
            m_actor = nullptr;
        }
    }

   private:
    PxActor* m_actor;
};
};  // namespace knot
