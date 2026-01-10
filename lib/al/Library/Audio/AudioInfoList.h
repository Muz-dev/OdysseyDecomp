#pragma once
#include "Library/Yaml/ByamlIter.h"
#include "container/seadPtrArray.h"

namespace al {

struct AudioResourceLoadInfo;

template <typename T>
class AudioInfoListWithParts {
public:
    AudioInfoListWithParts();

    sead::PtrArray<T>* getPlayInfoList() const { return playInfoList; }
    sead::PtrArray<T>* getSortedInfoList() const { return sortedPlayInfoList; }
private:
    sead::PtrArray<T>* playInfoList;
    bool unk1;
    sead::PtrArray<T>* sortedPlayInfoList;
};

al::AudioInfoListWithParts<al::AudioResourceLoadInfo>* createAudioInfoList(const ByamlIter& bymlIter, s32 idx) {

}

} // namespace al