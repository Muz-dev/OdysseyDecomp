#pragma once

namespace al {
class CollisionCodeList;
class AudioEffectDataBase;
class AudioSoundArchiveInfo;
class BgmDataBase;
class SeDataBase;

struct AudioSystemInfo {
    AudioSystemInfo();

    void* _0;
    AudioEffectDataBase* audioEffectDataBase;
    AudioSoundArchiveInfo* audioSoundArchiveInfo;
    SeDataBase* seDataBase;
    BgmDataBase* bgmDataBase;
};

struct AudioSystemInitInfo {
    char* seDataName;
    char* seBgmName;
    bool unk1;
    bool unk2;
    bool isBgmOnSameMixIndex;
    bool unk3;
    f32 masterVolume;
    f32 dockedVolume;
    f32 undockedVolume;
    s32 systemHeapSize;
    s32 unk4;
    bool useAudioMaximizer;
    bool changeInputBgmChannelVolume;
    bool unk5;
    bool unk6;
    f32 monoVolume;
    f32 stereoVolume;
    bool unk7[4];
    CollisionCodeList* materialCodeList;
    CollisionCodeList* materialCodePrefixList;
    s32 cacheSizePerSound;
};

}  // namespace al
