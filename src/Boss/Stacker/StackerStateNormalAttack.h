#pragma once

#include <math/seadMatrix.h>

#include "Library/Nerve/NerveStateBase.h"

namespace al {
class SensorMsg;
}

namespace al {
class HitSensor;
}  // namespace al
class Stacker;

class StackerStateNormalAttack : public al::ActorStateBase {
public:
    StackerStateNormalAttack(Stacker* stacker);
    void appear() override;
    void kill() override;
    void control() override;
    void attackSensor(al::HitSensor* self, al::HitSensor* other, bool);
    bool receiveMsg(const al::SensorMsg* sensorMsg, al::HitSensor* self, al::HitSensor* other, bool);
    bool isAttack() const;
    bool isSwoon() const;
    void exeFirstWait();
    void exeMove();
    void exeAttackSuccess();
    void exeAttackSign();
    void exeAttack();
    void exeAttackEnd();
    void exeSwoonStart();
    void exeSwoon();
    void exeSwoonEnd();
    void exeSwoonReaction();
    void exeBounce();
    void exePress();

private:
    Stacker* mStacker;
    void* buf;
    f64 _30;
    s32 _38;
    s32 _3c;
    s32 _40;
    s32 _44;
    f32 _48;
    f32 _4c;
    u16 _50;
    sead::Matrix34f mMtx;
    f32 _84;
    f32 _88;
    f32 _8c;
};
