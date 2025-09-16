#include "Boss/Stacker/StackerStateNormalAttack.h"

#include "Boss/Stacker/Stacker.h"
#include "Library/LiveActor/ActorPoseUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Nerve/NerveUtil.h"

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
    NERVES_MAKE_STRUCT(StackerStateNormalAttack, Move, FirstWait, Bounce, AttackSuccess,
        Press, Swoon, SwoonReaction, SwoonStart, Attack, AttackEnd, AttackSign, SwoonEnd)
} // namespace

StackerStateNormalAttack::StackerStateNormalAttack(Stacker* stacker) : ActorStateBase("通常移動攻撃", stacker) {}

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

