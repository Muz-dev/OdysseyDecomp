#include "Library/Clipping/ClippingRequestKeeper.h"

#include "Library/Clipping/ClippingRequestInfo.h"

namespace al {

ClippingRequestTable::ClippingRequestTable(s32 capacity) {
    mCapacity = capacity;
    mSize = 0;
    mInfo = nullptr;
    mInfo = new ClippingRequestInfo[mCapacity];

}


ClippingRequestKeeper::ClippingRequestKeeper(s32 capacity) {
    mRequestTable = new ClippingRequestTable(capacity);
}



void ClippingRequestKeeper::request(LiveActor* actor, ClippingRequestType clippingRequestType) {
    mRequestTable->mSize++;
    mRequestTable->mInfo[mRequestTable->mSize - 1] = {actor, clippingRequestType};
}

void ClippingRequestKeeper::executeRequest() {

}




} // namespace al