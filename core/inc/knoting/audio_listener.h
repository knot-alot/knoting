#pragma once
#include <knoting/asset.h>
#include <knoting/types.h>
#include <fmod.hpp>

namespace knot::components {
class AudioListener : public Asset {
   public:
    AudioListener();
    ~AudioListener();

   private:
    float m_hearing_range;
};

}  // namespace knot::components