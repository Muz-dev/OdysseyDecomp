#pragma once

#include <basis/seadTypes.h>
#include <math/seadMatrix.h>

#include "Library/LiveActor/LiveActor.h"

namespace al {
class HitSensor;
struct ActorInitInfo;

class DynamicCollisionActor : LiveActor {
public:
    DynamicCollisionActor(const ActorInitInfo& initInfo, s32 idx, const sead::Matrix34f* mtx, HitSensor* hitSensor, const void* ptr);
    void create(s32 idx);
    void begin();
    void vertex(const sead::Vector3f& pos);
    void endData();
    void updateCollisionParts();
    void attribute(u16 idx);
    void makePrism();
    void end();
private:
    void* mData;
    void* buffer[4];
    s32 _130;
    sead::Vector3f _134;
    sead::Vector3f _140;
    sead::Vector3f _14c;
    sead::Vector3f _158;
    sead::Vector3f _164;
    u16 _170;
    s16 buffer3;
    s32 _174;
};

} // namespace al