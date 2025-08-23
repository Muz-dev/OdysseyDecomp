#pragma once

#include <basis/seadTypes.h>
#include <math/seadVector.h>
#include <math/seadBoundBox.h>

namespace sead {
class DelegateThread;
class Thread;
class Event;
}

namespace al {
struct ActorInitInfo;
class ClippingActorInfo;
class ClippingGroupHolder;
class ClippingJudge;
class LiveActor;
class ClippingActorInfoList;
class ClippingRequestKeeper;

class ClippingActorHolder {
public:
    ClippingActorHolder(s32);

    void updateAsync(sead::Thread*, s64);
    ~ClippingActorHolder();
    void update(const ClippingJudge*);
    ClippingActorInfo* registerActor(LiveActor*);
    ClippingActorInfo* initGroupClipping(LiveActor*, const ActorInitInfo&);
    void endInit(ClippingGroupHolder*);
    void validateClipping(LiveActor*);
    void invalidateClipping(LiveActor*);
    void addToClippingTarget(LiveActor*);
    void removeFromClippingTarget(LiveActor*);
    void onGroupClipping(LiveActor*);
    void offGroupClipping(LiveActor*);
    f32 getClippingRadius(const LiveActor*);
    ClippingActorInfo* find(const LiveActor*) const;
    const sead::Vector3f& getClippingCenterPos(const LiveActor*);
    void setTypeToSphere(LiveActor*, f32, const sead::Vector3f*);
    void setTypeToObb(LiveActor* liveActor, const sead::BoundBox3f&);
    const sead::BoundBox3f& getClippingObb(const LiveActor*);
    void setNearClipDistance(LiveActor*, f32);
    void setFarClipLevel20M(LiveActor*);
    void setFarClipLevelMax(LiveActor*);
    void updateFarClipLevel();

private:
    s32 mClippingActorInfoSize = 0;
    s32 mActorCount = 0;
    ClippingActorInfoList* mClippingActorInfoList = nullptr;
    ClippingActorInfoList* _10 = nullptr;
    ClippingActorInfoList* _18 = nullptr;
    ClippingActorInfoList* _20 = nullptr;
    ClippingRequestKeeper** mClippingRequestKeepers = nullptr;
    sead::Event** mClippingEvents = nullptr;
    sead::DelegateThread** mDelegateThreads = nullptr;
};

}  // namespace al
