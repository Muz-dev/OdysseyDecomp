#pragma once

#include "basis/seadTypes.h"

namespace al {
class LiveActor;

enum class ClippingRequestType : s32 {
    val1,
    val2
};

struct ClippingRequestInfo {
    LiveActor* mLiveActor;
    ClippingRequestType mRequestType = ClippingRequestType::val1;
};

} // namespace al