#pragma once
#include "container/seadPtrArray.h"

namespace al {

struct AudioResourceLoadInfo {
    const char* name;
    bool unk1;

    static sead::PtrArray<AudioResourceLoadInfo>::CompareCallback compareInfo;
};

struct AudioLoadGroupList {
    sead::PtrArray<AudioResourceLoadInfo>* unk1;
    void* unk2;
    sead::PtrArray<AudioResourceLoadInfo>* resourceLoadInfos;
};

struct AudioResourceLoadGroupInfo {
    const char* name;
    AudioLoadGroupList* unk1;
    AudioLoadGroupList* unk2;
};

} // namespace al