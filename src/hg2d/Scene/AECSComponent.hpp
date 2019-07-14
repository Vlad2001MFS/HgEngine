#pragma once
#include "hd/IO/hdSerializerRW.hpp"

namespace hg2d {

struct AECSComponent {
    AECSComponent();
    virtual ~AECSComponent();

    virtual void onSaveLoad(hd::SerializerRW &stream) = 0;
};

}
