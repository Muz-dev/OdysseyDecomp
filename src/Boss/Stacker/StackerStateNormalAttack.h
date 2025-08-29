#pragma once
#include "Library/Nerve/NerveStateBase.h"

namespace al {
class SensorMsg;
}

namespace al {
class HitSensor;
} // namespace al
class Stacker;

class StackerStateNormalAttack : public al::ActorStateBase {
public:
    StackerStateNormalAttack(Stacker* stacker);
    void appear() override;
    void kill() override;
    void control() override;
    void attackSensor(al::HitSensor* self, al::HitSensor other, bool);
    void receiveMsg(const al::SensorMsg* sensorMsg, al::HitSensor* self, al::HitSensor other, bool);
    bool isAttack() const;
    bool isSwoon() const;
    void exeFirstWait();
    void exeMove();
    void exeAttackSuccess();
    void exeAttackSign();
    void exeAttack();
    void exeSwoonStart();
    void exeSwoon();
    void exeSwoonEnd();
    void exeSwoonReaction();
    void exeBounce();
    void exePress();
};