#pragma once

#include <knoting/log.h>

namespace knot {

// Abstract class, use subsystems please
template <class T>
class Component {
   public:
    virtual ~Component() { static_cast<T*>(this)->on_destroy(); }

    virtual void on_awake(){};
    virtual void on_destroy(){};
};

}  // namespace knot
