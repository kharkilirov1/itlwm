//
//  IOPCIEDeviceWrapper.cpp
//

#include "IOPCIEDeviceWrapper.hpp"

#include "ItlIwm.hpp"
#include "ItlIwx.hpp"
#include "ItlIwn.hpp"
#include <Airport/IO80211WorkQueue.h>
#include <libkern/OSAtomic.h>

#define super IOService
OSDefineMetaClassAndStructors(IOPCIEDeviceWrapper, IOService);

#define PCI_MSI_FLAGS           2
#define PCI_CAP_ID_MSI          0x05
#define PCI_MSIX_FLAGS          2
#define PCI_CAP_ID_MSIX         0x11
#define PCI_MSIX_FLAGS_ENABLE   0x8000
#define PCI_MSI_FLAGS_ENABLE    0x0001

static IOPMPowerState powerStateArray[2] =
{
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, kIOPMDeviceUsable, kIOPMPowerOn, kIOPMPowerOn, 0, 0, 0, 0, 0, 0, 0, 0}
};

// The imported OpenBSD compatibility layer still routes timeout and input
// actions through a process-wide work-loop/command-gate pair.  Until those
// APIs carry an explicit controller context, admit exactly one wrapper so a
// second Intel device cannot overwrite another controller's live pointers.
static IOPCIEDeviceWrapper *gActiveWrapper;
extern IO80211WorkQueue *_fWorkloop;

static void pciMsiSetEnable(IOPCIDevice *device, UInt8 msiCap, int enable)
{
    UInt16 control;

    control = device->configRead16(msiCap + PCI_MSI_FLAGS);
    control &= ~PCI_MSI_FLAGS_ENABLE;
    if (enable)
        control |= PCI_MSI_FLAGS_ENABLE;
    device->configWrite16(msiCap + PCI_MSI_FLAGS, control);
}

static void pciMsiXClearAndSet(IOPCIDevice *device, UInt8 msixCap, UInt16 clear, UInt16 set)
{
    UInt16 ctrl;

    ctrl = device->configRead16(msixCap + PCI_MSIX_FLAGS);
    ctrl &= ~clear;
    ctrl |= set;
    device->configWrite16(msixCap + PCI_MSIX_FLAGS, ctrl);
}

bool IOPCIEDeviceWrapper::init(OSDictionary *properties)
{
    if (!super::init(properties))
        return false;
    halFamily = kHalFamilyNone;
    fWorkloop = NULL;
    pciNub = NULL;
    return true;
}

void IOPCIEDeviceWrapper::releaseOwnedResources()
{
    const bool ownsCompatibilityContext = (gActiveWrapper == this);
    if (ownsCompatibilityContext && _fWorkloop == fWorkloop)
        _fWorkloop = NULL;
    if (fWorkloop) {
        fWorkloop->release();
        fWorkloop = NULL;
    }
    if (ownsCompatibilityContext)
        OSCompareAndSwapPtr(this, NULL, &gActiveWrapper);
    halFamily = kHalFamilyNone;
    pciNub = NULL;
}

void IOPCIEDeviceWrapper::free()
{
    releaseOwnedResources();
    super::free();
}

IOWorkLoop *IOPCIEDeviceWrapper::getWorkLoop() const
{
    return fWorkloop;
}

IOService *IOPCIEDeviceWrapper::
probe(IOService *provider, SInt32 *score)
{
    XYLog("%s\n", __PRETTY_FUNCTION__);
    if (!super::probe(provider, score))
        return NULL;

    UInt8 msiCap = 0;
    UInt8 msixCap = 0;
    IOPCIDevice *device = OSDynamicCast(IOPCIDevice, provider);
    if (!device)
        return NULL;

    HalFamily candidateFamily = kHalFamilyNone;
    if (ItlIwx::iwx_match(device)) {
        candidateFamily = kHalFamilyIwx;
    } else if (ItlIwm::iwm_match(device)) {
        candidateFamily = kHalFamilyIwm;
    } else if (ItlIwn::iwn_match(device)) {
        candidateFamily = kHalFamilyIwn;
    }
    if (candidateFamily == kHalFamilyNone)
        return NULL;

    XYLog("%s Found Intel device\n", __FUNCTION__);
    device->findPCICapability(PCI_CAP_ID_MSIX, &msixCap);
    if (msixCap)
        pciMsiXClearAndSet(device, msixCap, PCI_MSIX_FLAGS_ENABLE, 0);
    device->findPCICapability(PCI_CAP_ID_MSI, &msiCap);
    if (msiCap)
        pciMsiSetEnable(device, msiCap, 1);
    if (!msiCap && !msixCap) {
        XYLog("%s No MSI cap\n", __FUNCTION__);
        return NULL;
    }

    halFamily = candidateFamily;
    pciNub = device;
    return this;
}

ItlHalService *IOPCIEDeviceWrapper::createHalService() const
{
    ItlHalService *service = NULL;
    switch (halFamily) {
        case kHalFamilyIwx:
            service = new ItlIwx;
            break;
        case kHalFamilyIwm:
            service = new ItlIwm;
            break;
        case kHalFamilyIwn:
            service = new ItlIwn;
            break;
        case kHalFamilyNone:
        default:
            return NULL;
    }

    // A HAL softc cannot be reused safely after a partial or completed attach.
    // Each controller lifecycle therefore receives a freshly initialized object.
    if (!service || !service->init()) {
        OSSafeReleaseNULL(service);
        return NULL;
    }
    return service;
}

bool IOPCIEDeviceWrapper::
start(IOService *provider)
{
    XYLog("%s\n", __PRETTY_FUNCTION__);
    if (!super::start(provider))
        return false;
    if (!OSCompareAndSwapPtr(NULL, this, &gActiveWrapper)) {
        XYLog("%s only one active Intel PCI wrapper is supported\n",
              __FUNCTION__);
        super::stop(provider);
        return false;
    }

    fWorkloop = IO80211WorkQueue::workQueue();
    if (!fWorkloop) {
        releaseOwnedResources();
        super::stop(provider);
        return false;
    }
    _fWorkloop = fWorkloop;

    UInt8 builtIn = 0;
    OSData *builtInData = OSData::withBytes(&builtIn, sizeof(builtIn));
    if (!builtInData || !setProperty("built-in", builtInData)) {
        OSSafeReleaseNULL(builtInData);
        releaseOwnedResources();
        super::stop(provider);
        return false;
    }
    builtInData->release();
    PMinit();
    registerPowerDriver(this, powerStateArray, 2);
    provider->joinPMtree(this);
    registerService();
    return true;
}

void IOPCIEDeviceWrapper::
stop(IOService *provider)
{
    XYLog("%s\n", __PRETTY_FUNCTION__);
    PMstop();
    super::stop(provider);
    releaseOwnedResources();
}

IOReturn IOPCIEDeviceWrapper::
setPowerState(unsigned long powerStateOrdinal, IOService *whatDevice)
{
    return IOPMAckImplied;
}
