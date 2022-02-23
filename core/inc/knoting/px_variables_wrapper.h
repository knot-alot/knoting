#pragma once
#include <PxPhysicsAPI.h>

using namespace physx;

namespace knot {
class PxScene_ptr_wrapper {
   public:
    PxScene_ptr_wrapper(PxScene* scene = nullptr) : m_px_scene(scene) {}
    ~PxScene_ptr_wrapper() {
        if (m_px_scene) {
            m_px_scene->release();
            m_px_scene = nullptr;
        }
    }

    void set(PxScene* scene) { m_px_scene = scene; }

    PxScene* get() { return m_px_scene; }

   private:
    PxScene* m_px_scene;
};

class PxPhysics_ptr_wrapper {
   public:
    PxPhysics_ptr_wrapper(PxPhysics* physics = nullptr) : m_px_physics(physics) {}
    ~PxPhysics_ptr_wrapper() {
        if (m_px_physics) {
            m_px_physics->release();
            m_px_physics = nullptr;
        }
    }

    void set(PxPhysics* physics) {
        if (m_px_physics) {
            m_px_physics->release();
            m_px_physics = nullptr;
        }
        m_px_physics = physics;
    }

    PxPhysics* get() { return m_px_physics; }

   private:
    PxPhysics* m_px_physics;
};
class PxFoundation_ptr_wrapper {
   public:
    PxFoundation_ptr_wrapper(PxFoundation* foundation = nullptr) : m_Foundation(foundation) {}
    ~PxFoundation_ptr_wrapper() {
        if (m_Foundation) {
            m_Foundation->release();
            m_Foundation = nullptr;
        }
    }
    void set(PxFoundation* foundation) {
        if (m_Foundation) {
            m_Foundation->release();
            m_Foundation = nullptr;
        }
        m_Foundation = foundation;
    }

    PxFoundation* get() { return m_Foundation; }

   private:
    PxFoundation* m_Foundation;
};
class PxDispatcher_ptr_wrapper {
   public:
    PxDispatcher_ptr_wrapper(PxDefaultCpuDispatcher* dispatcher = nullptr) : m_Dispatcher(dispatcher) {}
    ~PxDispatcher_ptr_wrapper() {
        if (m_Dispatcher) {
            m_Dispatcher->release();
            m_Dispatcher = nullptr;
        }
    }
    void set(PxDefaultCpuDispatcher* dispatcher) {
        if (m_Dispatcher) {
            m_Dispatcher->release();
            m_Dispatcher = nullptr;
        }
        m_Dispatcher = dispatcher;
    }

    PxDefaultCpuDispatcher* get() { return m_Dispatcher; }

   private:
    PxDefaultCpuDispatcher* m_Dispatcher;
};

class PxDynamic_ptr_wrapper {
   public:
    PxDynamic_ptr_wrapper(PxRigidDynamic* dynamic = nullptr) : m_dynamic(dynamic) {}
    ~PxDynamic_ptr_wrapper() {
        if (m_dynamic) {
            m_dynamic->release();
            m_dynamic = nullptr;
        }
    }
    void set(PxRigidDynamic* dynamic) {
        if (m_dynamic) {
            m_dynamic->release();
            m_dynamic = nullptr;
        }
        m_dynamic = dynamic;
    }

    PxRigidDynamic* get() { return m_dynamic; }

   private:
    PxRigidDynamic* m_dynamic;
};

class PxStatic_ptr_wrapper {
   public:
    PxStatic_ptr_wrapper(PxRigidStatic* s = nullptr) : m_static(s) {}
    ~PxStatic_ptr_wrapper() {
        m_static->release();
        m_static = nullptr;
    }
    void set(PxRigidStatic* s) {
        if (m_static) {
            m_static->release();
            m_static = nullptr;
        }
        m_static = s;
    }

    PxRigidStatic* get() { return m_static; }

   private:
    PxRigidStatic* m_static;
};

class PxMaterial_ptr_wrapper {
   public:
    PxMaterial_ptr_wrapper(PxMaterial* material = nullptr) : m_material(material) {}
    ~PxMaterial_ptr_wrapper() {
        m_material->release();
        m_material = nullptr;
    }
    void set(PxMaterial* material) {
        if (m_material) {
            m_material->release();
            m_material = nullptr;
        }
        m_material = material;
    }

    PxMaterial* get() { return m_material; }

   private:
    PxMaterial* m_material;
};

};  // namespace knot
