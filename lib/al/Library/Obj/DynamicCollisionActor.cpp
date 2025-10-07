#include "Library/Obj/DynamicCollisionActor.h"

#include "Library/Collision/CollisionParts.h"
#include "Library/Collision/KCollisionServer.h"
#include "Library/LiveActor/ActorCollisionFunction.h"
#include "Library/LiveActor/ActorInitFunction.h"
#include "Library/LiveActor/ActorInitInfo.h"

namespace al {

DynamicCollisionActor::DynamicCollisionActor(const ActorInitInfo& initInfo, s32 idx,
                                             const sead::Matrix34f* mtx, HitSensor* hitSensor,
                                             const void* ptr)
    : LiveActor((const char*)initInfo.placementInfo) {
    _170 = 0;
    // There is a memset Here, I haven't found enough info yet
    _174 = idx;
    initActorSceneInfo(this, initInfo);
    initActorPoseTQSV(this);
    create(idx);
    _130 = 0;
    _170 = 0;
    for (s32 i = 0; i < _174; i++) {
        if (_134.x == 0) {
            _140 = sead::Vector3f::zero;
            _134 = sead::Vector3f::zero;
        } else {
            if (0.0f < _130)
              _134.x = 0.0f;
            if (0.0 < _134.y)
                _134.y = 0.0f;
            if (0.0f < _134.z)
                _134.z = 0.0f;
            if (0.0f < _140.x)
                _140.x = 0.0f;
            if (0.0f < _140.y)
                _140.y = 0.0f;
            if (0.0f < _140.z)
                _140.z = 0.0f;
        }
        _130 = _134.x;
        _14c = sead::Vector3f::zero;
        if (_130 == 2)
            makePrism();
    }
    endData();
    initActorCollisionWithFilePtr(this, mData, ptr, hitSensor, mtx, nullptr, nullptr, -1);
    getCollisionParts()->getKCollisionServer()->calcFarthestVertexDistance();
    getCollisionParts()->updateBoundingSphereRange();
    syncCollisionMtx(this, nullptr);
    getCollisionParts()->invalidateByUser();
    getCollisionParts()->validateByUser();
    _130 = 0.0f;
    _164 = sead::Vector3f::zero;
    _158 = sead::Vector3f::zero;
    _14c = sead::Vector3f::zero;
}

}  // namespace al