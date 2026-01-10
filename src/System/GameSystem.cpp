#include "System/GameSystem.h"

#include "Application.h"
#include "GameConfigData.h"
#include "Library/Effect/EffectSystem.h"
#include "Library/Memory/HeapUtil.h"
#include "Library/Nerve/NerveSetupUtil.h"
#include "Library/Network/AccountHolder.h"
#include "Library/Network/HtmlViewer.h"
#include "Library/Network/NetworkSystem.h"
#include "Library/System/GameSystemInfo.h"
#include "Library/Layout/LayoutSystem.h"
#include "Library/Audio/AudioInfoList.h"

#include "Library/Audio/AudioLoadGroup.h"
#include "Library/Audio/System/AudioKeeperFunction.h"
#include "Library/Audio/System/AudioSystemInfo.h"
#include "Library/Collision/CollisionCodeFunction.h"
#include "Library/LiveActor/ActorSensorUtil.h"
#include "Library/Message/MessageSystem.h"
#include "Library/Resource/ResourceFunction.h"
#include "Library/Se/Info/SeAudioInfo.h"
#include "Library/Se/Info/SeEmitterInfo.h"
#include "Library/Se/Info/SeShapeInfo3DPoint.h"
#include "ProjectNfpDirector.h"
#include "heap/seadExpHeap.h"
#include "nn/audio.h"
#include "nn/friends.h"

namespace {
NERVE_IMPL(GameSystem, Play);

// TODO: Remove maybe_unused once this class is implemented and the nerves are used
[[maybe_unused]] NERVES_MAKE_STRUCT(GameSystem, Play);
}  // namespace


GameSystem::GameSystem() : NerveExecutor("ゲームシステム") {}

void GameSystem::init() {
    mSystemInfo = new al::GameSystemInfo;
    mSystemInfo->drawSystemInfo = Application::instance()->getDrawSystemInfo();
    mGameConfigData = new GameConfigData;
    initNerve(&NrvGameSystem.Play);
    mAccountHolder = Application::instance()->getAccountHolder();
    nn::friends::Initialize();
    mNetworkSystem = new al::NetworkSystem(mAccountHolder->getUserHandle(), true);
    mNetworkSystem->requestSystemInitialize();
    mSystemInfo->networkSystem = mNetworkSystem;
    mHtmlViewer = new al::HtmlViewer;
    mSystemInfo->htmlViewer = mHtmlViewer;
    mNfpDirector = new ProjectNfpDirector;
    mSystemInfo->nfpDirector = mNfpDirector;
    mNfpDirector->initialize();

    sead::ExpHeap* heap = sead::ExpHeap::create(0x4600000, "", nullptr, 8,  sead::Heap::HeapDirection::cHeapDirection_Forward, false);
    al::addNamedHeap(heap, "EffectSystemHeap");

    al::EffectSystem* effectSystem = al::EffectSystem::createSystem(mSystemInfo->drawSystemInfo->drawContext, heap);
    mSystemInfo->effectSystem = effectSystem;

    mSystemInfo->layoutSystem = new al::LayoutSystem;
    mSystemInfo->messageSystem = new al::MessageSystem;

    al::AudioSystemInitInfo audioSystemInitInfo {
        .seBgmName = nullptr,
        .unk1 = true,
        .unk2 = true,
        .isBgmOnSameMixIndex = false,
        .masterVolume = 1.0f,
        .dockedVolume = 1.0f,
        .unk4 = 0,
        .useAudioMaximizer = false,
        .changeInputBgmChannelVolume = false,
        .cacheSizePerSound = 0,
        .undockedVolume = 1.0f,
        .systemHeapSize = -1,
        .monoVolume = 1.0f,
        .stereoVolume = 1.0f,
        .materialCodeList = nullptr,
        .materialCodePrefixList = nullptr,
    };

    al::CollisionCodeList* materialCode = alCollisionCodeFunction::tyrCreateCollisionCodeList("MaterialCode");
    al::CollisionCodeList* materialCodePrefix = alCollisionCodeFunction::tyrCreateCollisionCodeList("MaterialCodePrefix");

    mSystemInfo->effectSystem->setMaterialCodeList(materialCode);
    mSystemInfo->effectSystem->setMaterialCodePrefix(materialCodePrefix);

    audioSystemInitInfo = {
        .dockedVolume = 0.401f,
        .undockedVolume = 1.0f,
        .unk2 = true,
        .isBgmOnSameMixIndex = true,
        .changeInputBgmChannelVolume = true,
        .seDataName = "SeData",
        .seBgmName = "BgmData",
        .monoVolume = 0.3f,
        .stereoVolume = 1.061f,
        .cacheSizePerSound = 0x40000,
        .materialCodeList = materialCode,
        .materialCodePrefixList = materialCodePrefix,
    };

    sead::Heap* audioHeap = al::tryFindNamedHeap("AudioHeap");
    u64 audioHeapSize = 0;
    if (audioHeap) {
        audioHeapSize = audioHeap->getSize();
    }
    mAudioSystem = new al::AudioSystem;
    mAudioSystem->init(audioSystemInitInfo);

    al::SeadAudioPlayer* audioPlayerForSe = alAudioSystemFunction::getSeadAudioPlayerForSe(mAudioSystem);
    al::SeadAudioPlayer* audioPlayerForBgm = alAudioSystemFunction::getSeadAudioPlayerForBgm(mAudioSystem);

    al::setAudioPlayerToResourceSystem(audioPlayerForSe, audioPlayerForBgm);
    mAudioInfoList = new al::AudioInfoListWithParts<al::AudioResourceLoadGroupInfo>;

    al::AudioResourceLoadGroupInfo* audioResourceLoadGroupInfo = new al::AudioResourceLoadGroupInfo;
    audioResourceLoadGroupInfo->name = "システム常駐";
    al::AudioLoadGroupList* audioLoadGroupList = new al::AudioLoadGroupList;
    audioLoadGroupList->unk2 = nullptr;
    audioLoadGroupList->unk1 = new sead::PtrArray<al::AudioResourceLoadInfo>(0, nullptr);
    audioLoadGroupList->unk1->allocBuffer(1, nullptr, 8);
    audioLoadGroupList->resourceLoadInfos = nullptr;
    audioResourceLoadGroupInfo->unk1 = audioLoadGroupList;

    audioLoadGroupList = new al::AudioLoadGroupList;
    audioLoadGroupList->unk2 = nullptr;
    audioLoadGroupList->unk1 = new sead::PtrArray<al::AudioResourceLoadInfo>(0, nullptr);
    audioLoadGroupList->unk1->allocBuffer(1, nullptr, 8);
    audioResourceLoadGroupInfo->unk1->resourceLoadInfos = nullptr;
    audioResourceLoadGroupInfo->unk2 = audioLoadGroupList;

    al::AudioResourceLoadInfo* audioResourceLoadInfo = new al::AudioResourceLoadInfo;
    audioResourceLoadInfo->name = "SeResourceStdSystem";
    audioResourceLoadInfo->unk1 = false;



}

