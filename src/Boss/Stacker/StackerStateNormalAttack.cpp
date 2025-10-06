#include "Boss/Stacker/StackerStateNormalAttack.h"

#include "Boss/Stacker/Stacker.h"
#include "Library/Collision/CollisionPartsKeeperUtil.h"
#include "Library/LiveActor/ActorActionFunction.h"
#include "Library/LiveActor/ActorMovementFunction.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Math/MathUtil.h"
#include "Library/Matrix/MatrixUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"
#include "Library/Player/PlayerUtil.h"
#include "Project/HitSensor/HitSensor.h"
#include "Util/PlayerUtil.h"
#include "Util/SensorMsgFunction.h"

namespace {
NERVE_IMPL(StackerStateNormalAttack, Move)
NERVE_IMPL(StackerStateNormalAttack, FirstWait)
NERVE_IMPL(StackerStateNormalAttack, Bounce)
NERVE_IMPL(StackerStateNormalAttack, AttackSuccess)
NERVE_IMPL(StackerStateNormalAttack, Press)
NERVE_IMPL(StackerStateNormalAttack, Swoon)
NERVE_IMPL(StackerStateNormalAttack, SwoonReaction)
NERVE_IMPL(StackerStateNormalAttack, SwoonStart)
NERVE_IMPL(StackerStateNormalAttack, Attack)
NERVE_IMPL(StackerStateNormalAttack, AttackEnd)
NERVE_IMPL(StackerStateNormalAttack, AttackSign)
NERVE_IMPL(StackerStateNormalAttack, SwoonEnd)
NERVES_MAKE_STRUCT(StackerStateNormalAttack, Move, FirstWait, Bounce, Press, AttackSuccess, Swoon,
                   SwoonReaction, SwoonStart, Attack, AttackEnd, AttackSign, SwoonEnd)
}  // namespace

StackerStateNormalAttack::StackerStateNormalAttack(Stacker* stacker)
    : ActorStateBase("通常移動攻撃", stacker) {}

void StackerStateNormalAttack::appear() {
    if (!(_50 & 0xFF)) {
        if (_50 <= 0x100)
            al::setNerve(this, &NrvStackerStateNormalAttack.Move);
        else {
            al::setNerve(this, &NrvStackerStateNormalAttack.Bounce);
            _50 = 0;
        }
    } else {
        al::setNerve(this, &NrvStackerStateNormalAttack.FirstWait);
        _50 = 0;
    }
    _3c = 3;
    _40 = 0;
    _4c = 1.0f;
    ActorStateBase::appear();
}

void StackerStateNormalAttack::kill() {
    al::setScale(this->mActor, {1.0f, 1.0f, 1.0f});
    ActorStateBase::kill();
}

void StackerStateNormalAttack::control() {
    sead::Vector3f vec;

    al::makeMtxSRT(&mMtx, mStacker);
    vec.y = 1.0f;
    vec.x = (mStacker->get_154() - 3) * 0.2 + 1.0f;
    vec.z = vec.x;
    al::preScaleMtx(&mMtx, vec);
    if (_48 > 0.0f)
        _48 -= 0.02;
    if (_38 > 0)
        _38--;

    vec.set(_4c, _4c, _4c);
    al::setScale(mActor, vec);
    _4c = al::lerpValue(_4c, 1.0f, 0.06);
}

void StackerStateNormalAttack::attackSensor(al::HitSensor* self, al::HitSensor* other, bool) {
    if (!al::isNerve(this, &NrvStackerStateNormalAttack.SwoonStart) &&
        !al::isNerve(this, &NrvStackerStateNormalAttack.Swoon) &&
        !al::isNerve(this, &NrvStackerStateNormalAttack.SwoonEnd) &&
        !al::isNerve(this, &NrvStackerStateNormalAttack.Bounce) &&
        !al::isNerve(this, &NrvStackerStateNormalAttack.Press)) {
        al::HitSensor* sensor = al::getHitSensor(mStacker, "Attack");
        bool result = rs::sendMsgStackerRollingAttack(other, sensor);
        if (!result && al::isSensorEnemyAttack(self)) {
            if (al::sendMsgEnemyAttack(other, self)) {
                if (al::isNerve(this, &NrvStackerStateNormalAttack.Move))
                    al::setNerve(this, &NrvStackerStateNormalAttack.AttackSuccess);
                return;
            }
            rs::sendMsgPushToPlayer(other, self);
        }
        al::sendMsgPush(other, self);
        return;
    }
    rs::sendMsgPushToPlayer(other, self);
}

bool StackerStateNormalAttack::receiveMsg(const al::SensorMsg* sensorMsg, al::HitSensor* self,
                                          al::HitSensor* other, bool param4) {}

bool StackerStateNormalAttack::isAttack() const {
    if (!al::isNerve(this, &NrvStackerStateNormalAttack.AttackSign) && !al::isNerve(this, &NrvStackerStateNormalAttack.Attack)) {
        return al::isNerve(this, &NrvStackerStateNormalAttack.AttackEnd);
    }
    return true;
}

bool StackerStateNormalAttack::isSwoon() const {
    if (!al::isNerve(this, &NrvStackerStateNormalAttack.Swoon) && !al::isNerve(this, &NrvStackerStateNormalAttack.SwoonStart) && !al::isNerve(this, &NrvStackerStateNormalAttack.SwoonEnd))
        return al::isNerve(this, &NrvStackerStateNormalAttack.SwoonReaction);
    return true;
}

void StackerStateNormalAttack::exeFirstWait() {
    al::LiveActor* actor = mActor;
    if (al::isFirstStep(actor) && al::tryStartActionIfNotPlaying(actor, "Walk") && _88 < 0.8f) {
        al::setActionFrameRate(actor, 0.9230769);
    }
    sead::Vector3f playerPos = rs::getPlayerPos(actor);
    al::turnToTarget(actor, playerPos, _88);
    al::addVelocityToGravity(actor, 5.0f);
    al::scaleVelocity(actor, 0.9);
    if (!al::isGreaterEqualStep(this, 0x46)) {
        return;
    }
    al::setNerve(this, &NrvStackerStateNormalAttack.Move);
}

