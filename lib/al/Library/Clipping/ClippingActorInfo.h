#pragma once

#include <math/seadBoundBox.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al {
struct ActorInitInfo;

class ClippingRequestKeeper;
class ClippingJudge;
class LiveActor;
class PlacementId;
class ViewIdHolder;

class ClippingActorInfo {
public:
    ClippingActorInfo(LiveActor*);
    void setTypeToSphere(f32 radius, const sead::Vector3f* pos);
    void startClipped();
    void endClipped();
    void updateClipping(const ClippingJudge*);
    bool judgeClipping(const ClippingJudge*) const;
    void updateClipping(ClippingRequestKeeper*, const ClippingJudge*);
    bool isGroupClipping() const;
    bool isGroupClippingInit() const;
    void setTypeToObb(const sead::BoundBox3f&, const sead::Matrix34f*);
    void setGroupClippingId(const ActorInitInfo&);
    void setFarClipLevel20M();
    void updateFarClipLevel();
    void setFarClipLevelMax();
    bool isFarClipLevelMax() const;
    bool checkActiveViewGroupAny() const;
    void initViewGroup(const ViewIdHolder*);
    void registerViewGroupFarClipFlag(const bool*);

    LiveActor* getLiveActor() const { return mLiveActor; }

    PlacementId* getPlacementId() const { return mPlacementId; }

    const ViewIdHolder* getViewIdHolder() const { return mViewIdHolder; }

    bool isGroupClippingEnable() const { return mIsEnableGroupClipping; }

    void setGroupClippingEnable(bool enable) { mIsEnableGroupClipping = enable; }

    f32 getClippingRadius() const { return mClippingRadius; }

    const sead::Vector3f* getClippingCenterPos() const { return mClippingCenterPos; }

    const sead::BoundBox3f& getClippingObb() const { return mClippingObb; }

    void setNearClipDistance(f32 distance) { mNearClipDistance = distance; }

    // TODO: rename unnamed member variables
private:
    LiveActor* mLiveActor = nullptr;
    const sead::Vector3f* mClippingCenterPos = nullptr;
    f32 mClippingRadius = 0.0f;
    const sead::Matrix34f* mClippingMtx = nullptr;
    sead::BoundBox3f mClippingObb;
    f32 mNearClipDistance = 300.0f;
    PlacementId* mPlacementId = nullptr;
    s16 _48 = 1;
    s16 _4a = 1;
    s32 mFarClipFlagEntries = 0;
    s32 mFarClipFlagSize = 0;
    const bool** mFarClipFlags = nullptr;
    const ViewIdHolder* mViewIdHolder = nullptr;
    bool mIsEnableGroupClipping = true;
};
}  // namespace al
