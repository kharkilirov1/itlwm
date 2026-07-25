//
//  AirportSequoia.cpp
//  Нативный Intel Wi-Fi драйвер для macOS Sequoia (15.x) — этап 1 (скелет)
//

#include "AirportSequoia.hpp"
#include <IOKit/IOLib.h>
#include <IOKit/IOWorkLoop.h>

OSDefineMetaClassAndStructors(AirportSequoia, IO80211Controller)
OSDefineMetaClassAndStructors(AirportSequoiaInfra, IO80211InfraInterface)

#define DRVLOG(fmt, ...)  IOLog("[AirportSequoia v" AIRPORT_SEQUOIA_VERSION "] " fmt "\n", ##__VA_ARGS__)
#define DRVERR(fmt, ...)  IOLog("[AirportSequoia ERROR] " fmt "\n", ##__VA_ARGS__)

// ---------------------------------------------------------------------------
// AirportSequoiaInfra
// ---------------------------------------------------------------------------

void AirportSequoiaInfra::free() {
    DRVLOG("infra interface free");
    IO80211InfraInterface::free();
}

// ---------------------------------------------------------------------------
// AirportSequoia
// ---------------------------------------------------------------------------

IOService * AirportSequoia::probe(IOService *provider, SInt32 *score) {
    DRVLOG("probe");
    return IO80211Controller::probe(provider, score);
}

bool AirportSequoia::start(IOService *provider) {
    DRVLOG("start");
    if (!IO80211Controller::start(provider)) {
        DRVERR("IO80211Controller::start failed");
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
    IO80211Controller::stop(provider);
}

void AirportSequoia::free() {
    DRVLOG("free");
    IO80211Controller::free();
}
