//
//  AirportSequoia.cpp
//  Нативный Intel Wi-Fi драйвер для macOS Sequoia (15.x) — этап 1 (скелет)
//

#include "AirportSequoia.hpp"
#include <IOKit/IOLib.h>
#include <IOKit/IOWorkLoop.h>

#define super IO80211Controller
OSDefineMetaClassAndStructors(AirportSequoia, IO80211Controller)
OSDefineMetaClassAndStructors(AirportSequoiaInfra, IO80211InfraInterface)

#define DRVLOG(fmt, ...)  IOLog("[AirportSequoia v" AIRPORT_SEQUOIA_VERSION "] " fmt "\n", ##__VA_ARGS__)
#define DRVERR(fmt, ...)  IOLog("[AirportSequoia ERROR] " fmt "\n", ##__VA_ARGS__)

// ---------------------------------------------------------------------------
// AirportSequoiaInfra
// ---------------------------------------------------------------------------

bool AirportSequoiaInfra::init(IOService *provider) {
    if (!super::init(provider)) {
        return false;
    }
    DRVLOG("infra interface init");
    return true;
}

void AirportSequoiaInfra::free() {
    DRVLOG("infra interface free");
    super::free();
}

// ---------------------------------------------------------------------------
// AirportSequoia
// ---------------------------------------------------------------------------

IOService * AirportSequoia::probe(IOService *provider, SInt32 *score) {
    DRVLOG("probe");
    return super::probe(provider, score);
}

bool AirportSequoia::start(IOService *provider) {
    DRVLOG("start");
    if (!super::start(provider)) {
        DRVERR("super::start failed");
        return false;
    }

    fPCIDevice = OSDynamicCast(IOPCIDevice, provider);
    if (!fPCIDevice) {
        DRVERR("provider is not IOPCIDevice");
        return false;
    }
    fPCIDevice->retain();

    fWorkLoop = getWorkLoop();
    if (!fWorkLoop) {
        DRVERR("no workloop");
        return false;
    }

    // TODO(этап 1): загрузка firmware Intel через HAL itlwm (IwlService/iwx)
    // TODO(этап 1): создание и attach AirportSequoiaInfra (attachInterface)

    registerService();
    DRVLOG("started ok (skeleton)");
    return true;
}

void AirportSequoia::stop(IOService *provider) {
    DRVLOG("stop");
    if (fInfraInterface) {
        fInfraInterface->release();
        fInfraInterface = nullptr;
    }
    if (fPCIDevice) {
        fPCIDevice->release();
        fPCIDevice = nullptr;
    }
    super::stop(provider);
}

void AirportSequoia::free() {
    DRVLOG("free");
    super::free();
}
