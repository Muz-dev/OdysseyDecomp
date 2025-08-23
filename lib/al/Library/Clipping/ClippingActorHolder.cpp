#include "Library/Clipping/ClippingActorHolder.h"

#include "ClippingRequestKeeper.h"
#include "Library/Base/StringUtil.h"
#include "Library/Clipping/ClippingActorInfo.h"
#include "Library/Clipping/ClippingActorInfoList.h"
#include "Library/Clipping/ClippingGroupHolder.h"
#include "Library/LiveActor/ActorClippingFunction.h"
#include "Library/LiveActor/ActorFlagFunction.h"
#include "Library/LiveActor/LiveActor.h"
#include "Library/LiveActor/LiveActorFlag.h"
#include "Library/Thread/AsyncFunctorThread.h"
#include "thread/seadDelegateThread.h"
#include "thread/seadEvent.h"

namespace al {
ClippingActorHolder::ClippingActorHolder(s32 clippingActorInfoSize) : mClippingActorInfoSize{clippingActorInfoSize}{
    mClippingActorInfoList = new ClippingActorInfoList(clippingActorInfoSize);
    _10 = new ClippingActorInfoList(mClippingActorInfoSize);
    _18 = new ClippingActorInfoList(mClippingActorInfoSize);
    _20 = new ClippingActorInfoList(mClippingActorInfoSize);
    mClippingRequestKeepers = new ClippingRequestKeeper*[3];
    mClippingEvents = new sead::Event*[3];
    mDelegateThreads = new sead::DelegateThread*[3];

    for (s32 i = 0; i < 3; i++) {
        mClippingRequestKeepers[i] = new ClippingRequestKeeper((mClippingActorInfoSize + 2) / 3);
        mClippingEvents[i]= new sead::Event(true);
        mClippingEvents[i]->setSignal();

        mDelegateThreads[i] = new sead::DelegateThread((new StringTmp<32>("クリッピング非同期実行[%d]", i))->cstr(),
                                                       new sead::Delegate2<ClippingActorHolder, sead::Thread*, s64>(this,&ClippingActorHolder::updateAsync),
                                                       nullptr,
                                                       sead::Thread::cDefaultPriority+1,
                                                       sead::MessageQueue::BlockType::Blocking,
                                                       0x7fffffff,
                                                       0x2000, // stack szie
                                                       1); // core id

        sead::CoreIdMask mask = sead::CoreIdMask(i);
        mDelegateThreads[i]->setAffinity(mask);
        mDelegateThreads[i]->start();
    }
}

void ClippingActorHolder::updateAsync(sead::Thread* _thread, s64 clippingJudge) {
    sead::CoreId currentCoreId = getCurrentCoreId();
    s32 coreId=currentCoreId;
    s32 clippingActorInfoCount = mClippingActorInfoList->getClippingActorInfoCount();
    s32 iVar1 = (clippingActorInfoCount + 2) / 3;

    s32 i = iVar1 * coreId;

    iVar1 = i + iVar1;

    if (iVar1 <= clippingActorInfoCount)
        clippingActorInfoCount = iVar1;

    for (;i < clippingActorInfoCount;i++) {
        mClippingActorInfoList->getClippingActorInfos()[i]->updateClipping
        (mClippingRequestKeepers[coreId], (ClippingJudge *)clippingJudge);
    }
    mClippingEvents[coreId]->setSignal();
}

ClippingActorHolder::~ClippingActorHolder() {
    for (s32 i = 0; i < 3; i++) {
        mDelegateThreads[i]->quitAndWaitDoneSingleThread(false);
        if (mClippingEvents[i]) {
            delete mClippingEvents[i];
            mClippingEvents[i] = nullptr;
        }
        if (mDelegateThreads[i]) {
            delete mDelegateThreads[i];
            mDelegateThreads[i] = nullptr;
        }
    }
}

void ClippingActorHolder::update(const ClippingJudge* clippingJudge) {
    mClippingEvents[0]->resetSignal();
    mDelegateThreads[0]->sendMessage((sead::MessageQueue::Element)clippingJudge, (sead::MessageQueue::BlockType)1);

    mClippingEvents[1]->resetSignal();
    mDelegateThreads[1]->sendMessage((sead::MessageQueue::Element)clippingJudge, (sead::MessageQueue::BlockType)1);

    mClippingEvents[2]->resetSignal();
    mDelegateThreads[2]->sendMessage((sead::MessageQueue::Element)clippingJudge, (sead::MessageQueue::BlockType)1);

    mClippingEvents[0]->wait();
    mClippingEvents[1]->wait();
    mClippingEvents[2]->wait();

    mClippingRequestKeepers[0]->executeRequest();
    mClippingRequestKeepers[1]->executeRequest();
    mClippingRequestKeepers[2]->executeRequest();
}

ClippingActorInfo* ClippingActorHolder::registerActor(LiveActor* liveActor) {
    liveActor->getFlags()->isClippingInvalid = false;
    ClippingActorInfo* actorInfo = new ClippingActorInfo(liveActor);
    _18->add(actorInfo);
    mActorCount++;
    return actorInfo;
}

ClippingActorInfo* ClippingActorHolder::initGroupClipping(LiveActor* liveActor, const ActorInitInfo& actorInitInfo) {
    ClippingActorInfo* actorInfo;
    if (!mClippingActorInfoList->isInList(liveActor)) {
        if (!_18->isInList(liveActor)) {
            if (!_10->isInList(liveActor))
                actorInfo = nullptr;
            else {
                actorInfo = _10->remove(liveActor);
                _20->add(actorInfo);
                if (isClipped(liveActor))
                    liveActor->endClipped();
            }
        }
        else
            actorInfo = _18->find(liveActor, nullptr);
    }
    else {
        actorInfo = mClippingActorInfoList->remove(liveActor);
        _20->add(actorInfo);
    }
    actorInfo->setGroupClippingId(actorInitInfo);
    return actorInfo;
}

void ClippingActorHolder::endInit(ClippingGroupHolder* clippingGroupHolder) {
    for (s32 i = _18->getClippingActorInfoCount() - 1; i >= 0; i--)
        clippingGroupHolder->createAndCount(_18->getClippingActorInfos()[i]);

    for (s32 i = _20->getClippingActorInfoCount() - 1; i >= 0; i--)
        clippingGroupHolder->createAndCount(_20->getClippingActorInfos()[i]);

    clippingGroupHolder->allocBuffer();

    for (s32 i = _18->getClippingActorInfoCount() - 1; i >= 0; i--)
        clippingGroupHolder->registerInfo(_18->getClippingActorInfos()[i]);

    for (s32 i = _20->getClippingActorInfoCount() - 1; i >= 0; i--)
        clippingGroupHolder->registerInfo(_20->getClippingActorInfos()[i]);
}

void ClippingActorHolder::validateClipping(LiveActor* liveActor) {
    liveActor->getFlags()->isClippingInvalid = false;
    ClippingActorInfo* actorInfo = _10->remove(liveActor);
    if (isDead(liveActor)) {
        _18->add(actorInfo);
        actorInfo->updateFarClipLevel();
    } else {
        if (actorInfo->isGroupClipping()) {
            _20->add(actorInfo);
            actorInfo->updateFarClipLevel();
        } else {
            mClippingActorInfoList->add(actorInfo);
            actorInfo->updateFarClipLevel();
        }
    }
}

void ClippingActorHolder::invalidateClipping(LiveActor* liveActor) {
    liveActor->getFlags()->isClippingInvalid = true;

    if (isDead(liveActor)) {
        ClippingActorInfo* actorInfo = _18->remove(liveActor);
        _10->add(actorInfo);
    } else {
        if (!mClippingActorInfoList->isInList(liveActor)) {
            ClippingActorInfo* actorInfo = _20->remove(liveActor);
            _10->add(actorInfo);
        } else {
            ClippingActorInfo* actorInfo = mClippingActorInfoList->remove(liveActor);
            _10->add(actorInfo);
        }
    }
    if (isClipped(liveActor)) {
        liveActor->endClipped();
    }
}

void ClippingActorHolder::addToClippingTarget(LiveActor* liveActor) {
    if (!isInvalidClipping(liveActor) && !mClippingActorInfoList->isInList(liveActor) &&
        !_20->isInList(liveActor)) {
        ClippingActorInfo* actorInfo = _18->remove(liveActor);
        if (actorInfo->isGroupClipping()) {
            _20->add(actorInfo);
            actorInfo->updateFarClipLevel();
        } else {
            mClippingActorInfoList->add(actorInfo);
            actorInfo->updateFarClipLevel();
        }
    }
}

void ClippingActorHolder::removeFromClippingTarget(LiveActor* liveActor) {
    if (!isInvalidClipping(liveActor) && !_18->isInList(liveActor)) {
        if (!_20->isInList(liveActor)) {
            ClippingActorInfo* actorInfo = mClippingActorInfoList->remove(liveActor);
            _18->add(actorInfo);
        } else {
            ClippingActorInfo* actorInfo = _20->remove(liveActor);
            _18->add(actorInfo);
        }
    }
}

void ClippingActorHolder::onGroupClipping(LiveActor* liveActor) {
    if (mClippingActorInfoList->isInList(liveActor)) {
        ClippingActorInfo* actorInfo = mClippingActorInfoList->remove(liveActor);
        _20->add(actorInfo);
        actorInfo->updateFarClipLevel();
    }
}

void ClippingActorHolder::offGroupClipping(LiveActor* liveActor) {
    if (_20->isInList(liveActor)) {
        ClippingActorInfo* actorInfo = _20->remove(liveActor);
        mClippingActorInfoList->add(actorInfo);
        actorInfo->updateFarClipLevel();
    }
}

f32 ClippingActorHolder::getClippingRadius(const LiveActor* liveActor) {
    return find(liveActor)->getClippingRadius();
}

ClippingActorInfo* ClippingActorHolder::find(const LiveActor* liveActor) const {
    ClippingActorInfo* clippingActorInfo = mClippingActorInfoList->tryFind(liveActor);

    if (!clippingActorInfo)
        clippingActorInfo = _18->tryFind(liveActor);
    if (!clippingActorInfo)
        clippingActorInfo = _20->tryFind(liveActor);
    if (!clippingActorInfo)
        return _10->find(liveActor, nullptr);

    return clippingActorInfo;
}

const sead::Vector3f& ClippingActorHolder::getClippingCenterPos(const LiveActor* liveActor) {
    return *find(liveActor)->getClippingCenterPos();
}

void ClippingActorHolder::setTypeToSphere(LiveActor* liveActor, f32 radius, const sead::Vector3f* pos) {
    find(liveActor)->setTypeToSphere(radius, pos);
}

void ClippingActorHolder::setTypeToObb(LiveActor* liveActor, const sead::BoundBox3f& clippingObb) {
    find(liveActor)->setTypeToObb(clippingObb, nullptr);
}

const sead::BoundBox3f& ClippingActorHolder::getClippingObb(const LiveActor* liveActor) {
    return find(liveActor)->getClippingObb();
}

void ClippingActorHolder::setNearClipDistance(LiveActor* liveActor, f32 distance) {
    find(liveActor)->setNearClipDistance(distance);
}

void ClippingActorHolder::setFarClipLevel20M(LiveActor* liveActor) {
    find(liveActor)->setFarClipLevel20M();
}

void ClippingActorHolder::setFarClipLevelMax(LiveActor* liveActor) {
    find(liveActor)->setFarClipLevelMax();
}

void ClippingActorHolder::updateFarClipLevel() {
    s32 clippingActorCount = mClippingActorInfoList->getClippingActorInfoCount();

    if (clippingActorCount > 0) {
        mClippingActorInfoList->getClippingActorInfos()[0]->updateFarClipLevel();

        for (s32 i = 1; i != clippingActorCount; ++i)
            mClippingActorInfoList->getClippingActorInfos()[i]->updateFarClipLevel();
    }

    clippingActorCount = _20->getClippingActorInfoCount();

    if (0 < clippingActorCount) {
        _20->getClippingActorInfos()[0]->updateFarClipLevel();

        for (s32 i = 1; i != clippingActorCount; i++)
            this->_20->getClippingActorInfos()[i]->updateFarClipLevel();
   }
}



} // namespace al