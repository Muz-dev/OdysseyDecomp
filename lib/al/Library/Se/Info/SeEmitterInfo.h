#pragma once

namespace al {
class SeShapeInfo3DPoint;

struct SeEmitterInfo {
    SeEmitterInfo();
    const char* name;
    s64 unk1;
    s64 unk2;
    s64 unk3;
    SeShapeInfo3DPoint* shapeInfo;

    static sead::PtrArray<al::SeEmitterInfo>::CompareCallback compareInfo;
};
} // namespace al