//
//  AirportSequoia.cpp
//  Нативный Intel Wi-Fi драйвер для macOS Sequoia (15.x) — этап 1
//  Порт AirportItlwmV2 (Sonoma 14.4) на классы IO80211Family 15.7.4
//

#include "AirportSequoia.hpp"
#include <sys/_netstat.h>
#include <crypto/sha1.h>
#include <net80211/ieee80211_priv.h>

#include "IOPCIEDeviceWrapper.hpp"

OSDefineMetaClassAndStructors(AirportSequoia, IO80211Controller);
OSDefineMetaClassAndStructors(AirportSequoiaInfra, IO80211InfraInterface);

IOWorkLoop *_fWorkloop;
IOCommandGate *_fCommandGate;

#define DRVLOG(fmt, ...)  IOLog("[AirportSequoia v" AIRPORT_SEQUOIA_VERSION "] " fmt "\n", ##__VA_ARGS__)
#define DRVERR(fmt, ...)  IOLog("[AirportSequoia ERROR] " fmt "\n", ##__VA_ARGS__)

const char* hexdump(uint8_t *buf, size_t len) {
    ssize_t str_len = len * 3 + 1;
    char *str = (char*)IOMalloc(str_len);
    if (!str)
        return nullptr;
    for (size_t i = 0; i < len; i++)
        snprintf(str + 3 * i, (len - i) * 3, "%02x ", buf[i]);
    str[MAX(str_len - 2, 0)] = 0;
    return str;
}

// ---------------------------------------------------------------------------
// AirportSequoiaInfra
// ---------------------------------------------------------------------------

bool AirportSequoiaInfra::init(IOService *provider)
{
    if (!IO80211InfraInterface::init()) {
        DRVERR("IO80211InfraInterface init failed\n");
        return false;
    }
    instance = OSDynamicCast(AirportSequoia, provider);
    if (!instance)
        return false;
    this->fHalService = instance->fHalService;
    return true;
}

void AirportSequoiaInfra::free()
{
    DRVLOG("infra free");
    IO80211InfraInterface::free();
}

// ---------------------------------------------------------------------------
// AirportSequoia
// ---------------------------------------------------------------------------

void AirportSequoia::releaseAll()
{
    if (fHalService) {
        fHalService->release();
        fHalService = NULL;
    }
    if (_fWorkloop) {
        if (_fCommandGate) {
            _fWorkloop->removeEventSource(_fCommandGate);
            _fCommandGate->release();
            _fCommandGate = NULL;
        }
        if (scanSource) {
            scanSource->cancelTimeout();
            scanSource->disable();
            _fWorkloop->removeEventSource(scanSource);
            scanSource->release();
            scanSource = NULL;
        }
        if (fWatchdogWorkLoop && watchdogTimer) {
            watchdogTimer->cancelTimeout();
            fWatchdogWorkLoop->removeEventSource(watchdogTimer);
            watchdogTimer->release();
            watchdogTimer = NULL;
            fWatchdogWorkLoop->release();
            fWatchdogWorkLoop = NULL;
        }
        _fWorkloop->release();
        _fWorkloop = NULL;
    }
    unregistPM();
}

void AirportSequoia::
eventHandler(struct ieee80211com *ic, int msgCode, void *data)
{
    AirportSequoia *that = OSDynamicCast(AirportSequoia, ic->ic_ac.ac_if.controller);
    IO80211InfraInterface *interface = that->fNetIf;
    if (!interface)
        return;
    switch (msgCode) {
        case IEEE80211_EVT_COUNTRY_CODE_UPDATE:
            interface->postMessage(APPLE80211_M_COUNTRY_CODE_CHANGED, NULL, 0, false);
            break;
        case IEEE80211_EVT_STA_ASSOC_DONE:
            interface->postMessage(APPLE80211_M_ASSOC_DONE, NULL, 0, false);
            break;
        case IEEE80211_EVT_STA_DEAUTH:
            interface->postMessage(APPLE80211_M_DEAUTH_RECEIVED, NULL, 0, false);
            break;
        default:
            break;
    }
}

void AirportSequoia::watchdogAction(IOTimerEventSource *timer)
{
    struct _ifnet *ifp = &fHalService->get80211Controller()->ic_ac.ac_if;
    (*ifp->if_watchdog)(ifp);
    watchdogTimer->setTimeoutMS(kWatchDogTimerPeriod);
}

void AirportSequoia::fakeScanDone(OSObject *owner, IOTimerEventSource *sender)
{
    UInt32 msg = 0;
    AirportSequoia *that = (AirportSequoia *)owner;
    that->fNetIf->postMessage(APPLE80211_M_SCAN_DONE, &msg, 4, false);
}

bool AirportSequoia::init(OSDictionary *properties)
{
    DRVLOG("%s\n", __PRETTY_FUNCTION__);
    bool ret = IO80211Controller::init(properties);
    power_state = 0;
    return ret;
}

IOService *AirportSequoia::probe(IOService *provider, SInt32 *score)
{
    DRVLOG("%s\n", __PRETTY_FUNCTION__);
    IOPCIEDeviceWrapper *wrapper = OSDynamicCast(IOPCIEDeviceWrapper, provider);
    if (!wrapper) {
        DRVERR("%s Not a IOPCIEDeviceWrapper instance\n", __FUNCTION__);
        return NULL;
    }
    pciNub = wrapper->pciNub;
    fHalService = wrapper->fHalService;
    if (!pciNub || !fHalService) {
        DRVERR("%s Not a valid IOPCIEDeviceWrapper instance\n", __FUNCTION__);
        return NULL;
    }
    return IO80211Controller::probe(provider, score);
}

bool AirportSequoia::start(IOService *provider)
{
    DRVLOG("%s\n", __PRETTY_FUNCTION__);
    int boot_value = 0;

    UInt8 builtIn = 0;
    setProperty("built-in", OSData::withBytes(&builtIn, sizeof(builtIn)));
    setProperty("DriverKitDriver", kOSBooleanFalse);
    if (!IO80211Controller::start(provider)) {
        return false;
    }
    pciNub->setBusMasterEnable(true);
    pciNub->setIOEnable(true);
    pciNub->setMemoryEnable(true);
    pciNub->configWrite8(0x41, 0);
    if (pciNub->requestPowerDomainState(kIOPMPowerOn,
                                        (IOPowerConnection *) getParentEntry(gIOPowerPlane), IOPMLowestState) != IOPMNoErr) {
        IO80211Controller::stop(provider);
        return false;
    }
    if (initPCIPowerManagment(pciNub) == false) {
        IO80211Controller::stop(pciNub);
        return false;
    }
    if (_fWorkloop == NULL) {
        DRVERR("No _fWorkloop!!\n");
        IO80211Controller::stop(pciNub);
        releaseAll();
        return false;
    }
    _fCommandGate = IOCommandGate::commandGate(this, (IOCommandGate::Action)AirportSequoia::tsleepHandler);
    if (_fCommandGate == 0) {
        DRVERR("No command gate!!\n");
        IO80211Controller::stop(pciNub);
        releaseAll();
        return false;
    }
    _fWorkloop->addEventSource(_fCommandGate);
    const IONetworkMedium *primaryMedium;
    if (!createMediumTables(&primaryMedium) ||
        !setCurrentMedium(primaryMedium) || !setSelectedMedium(primaryMedium)) {
        DRVERR("setup medium fail\n");
        releaseAll();
        return false;
    }
    fHalService->initWithController(this, _fWorkloop, _fCommandGate);
    fHalService->get80211Controller()->ic_event_handler = eventHandler;

    if (PE_parse_boot_argn("-novht", &boot_value, sizeof(boot_value)))
        fHalService->get80211Controller()->ic_userflags |= IEEE80211_F_NOVHT;
    if (PE_parse_boot_argn("-noht40", &boot_value, sizeof(boot_value)))
        fHalService->get80211Controller()->ic_userflags |= IEEE80211_F_NOHT40;

    // === Загрузка firmware Intel ===
    if (!fHalService->attach(pciNub)) {
        DRVERR("HAL attach fail\n");
        IO80211Controller::stop(pciNub);
        releaseAll();
        return false;
    }
    DRVLOG("firmware loaded: %s\n", fHalService->getDriverInfo()->getFirmwareName());

    fWatchdogWorkLoop = IOWorkLoop::workLoop();
    if (fWatchdogWorkLoop == NULL) {
        DRVERR("init watchdog workloop fail\n");
        fHalService->detach(pciNub);
        IO80211Controller::stop(pciNub);
        releaseAll();
        return false;
    }
    watchdogTimer = IOTimerEventSource::timerEventSource(this, OSMemberFunctionCast(IOTimerEventSource::Action, this, &AirportSequoia::watchdogAction));
    if (!watchdogTimer) {
        DRVERR("init watchdog fail\n");
        fHalService->detach(pciNub);
        IO80211Controller::stop(pciNub);
        releaseAll();
        return false;
    }
    fWatchdogWorkLoop->addEventSource(watchdogTimer);
    scanSource = IOTimerEventSource::timerEventSource(this, &fakeScanDone);
    _fWorkloop->addEventSource(scanSource);
    scanSource->enable();

    // === Skywalk-интерфейс ===
    fNetIf = new AirportSequoiaInfra;
    if (!fNetIf->init(this)) {
        DRVERR("infra interface init fail\n");
        IO80211Controller::stop(provider);
        releaseAll();
        return false;
    }
    fNetIf->setInterfaceRole(1);
    fNetIf->setInterfaceId(1);

    if (!fNetIf->attach(this)) {
        DRVERR("attach to service fail\n");
        IO80211Controller::stop(provider);
        releaseAll();
        return false;
    }
    if (!attachInterface(fNetIf, this)) {
        DRVERR("attachInterface fail\n");
        IO80211Controller::stop(provider);
        releaseAll();
        return false;
    }
    if (!IONetworkController::attachInterface((IONetworkInterface **)&bsdInterface, true)) {
        DRVERR("attach to IONetworkController interface fail\n");
        IO80211Controller::stop(provider);
        releaseAll();
        return false;
    }
    // TODO(этап 1b): RegistrationInfo/deferBSDAttach-хак из V2 —
    // оффсеты mExpansionData в Sequoia не проверены, пока обходимся без него.
    fNetIf->start(this);

    setLinkStatus(kIONetworkLinkValid);
    if (TAILQ_EMPTY(&fHalService->get80211Controller()->ic_ess))
        fHalService->get80211Controller()->ic_flags |= IEEE80211_F_AUTO_JOIN;

    // этап 1: адаптер включаем сразу (в Sonoma — через enable())
    enableAdapter((IONetworkInterface *)bsdInterface);

    registerService();
    return true;
}

void AirportSequoia::stop(IOService *provider)
{
    DRVLOG("%s\n", __PRETTY_FUNCTION__);
    struct _ifnet *ifp = &fHalService->get80211Controller()->ic_ac.ac_if;
    IO80211Controller::stop(provider);
    disableAdapter(bsdInterface);
    setLinkStatus(kIONetworkLinkValid);
    fHalService->detach(pciNub);
    ether_ifdetach(ifp);
    detachInterface(fNetIf, true);
    OSSafeReleaseNULL(fNetIf);
    releaseAll();
}

void AirportSequoia::free()
{
    DRVLOG("%s\n", __PRETTY_FUNCTION__);
    if (fHalService != NULL) {
        fHalService->release();
        fHalService = NULL;
    }
    IO80211Controller::free();
}

// --- IO80211Controller appended slots --------------------------------------

SInt32 AirportSequoia::
apple80211_ioctl(IO80211SkywalkInterface *interface, unsigned long cmd, void *data, bool b1, bool b2)
{
    // TODO(этап 1b): диспетчеризация ioctl по новой схеме Sequoia
    DRVLOG("%s cmd: %lu\n", __FUNCTION__, cmd);
    return kIOReturnUnsupported;
}

bool AirportSequoia::createWorkQueue()
{
    DRVLOG("%s %d\n", __FUNCTION__, _fWorkloop != 0);
    return _fWorkloop != 0;
}

IOWorkLoop *AirportSequoia::getWorkQueue()
{
    return _fWorkloop;
}

// --- Сеть -------------------------------------------------------------------

IONetworkInterface *AirportSequoia::createInterface()
{
    AirportSequoiaEthernetInterface *netif = new AirportSequoiaEthernetInterface;
    if (!netif)
        return NULL;
    if (!netif->initWithSkywalkInterfaceAndProvider(this, fNetIf)) {
        netif->release();
        return NULL;
    }
    return netif;
}

bool AirportSequoia::configureInterface(IONetworkInterface *netif)
{
    IONetworkData *nd;
    struct _ifnet *ifp = &fHalService->get80211Controller()->ic_ac.ac_if;

    if (IO80211Controller::configureInterface(netif) == false) {
        DRVERR("super failed\n");
        return false;
    }

    nd = netif->getParameter(kIONetworkStatsKey);
    if (!nd || !(fpNetStats = (IONetworkStats *)nd->getBuffer())) {
        DRVERR("network statistics buffer unavailable?\n");
        return false;
    }
    ifp->netStat = fpNetStats;
    ether_ifattach(ifp, OSDynamicCast(IOEthernetInterface, netif));
    fpNetStats->collisions = 0;
#ifdef __PRIVATE_SPI__
    netif->configureOutputPullModel(fHalService->getDriverInfo()->getTxQueueSize(), 0, 0, IOEthernetInterface::kOutputPacketSchedulingModelNormal, 0);
#endif
    return true;
}

bool AirportSequoia::createMediumTables(const IONetworkMedium **primary)
{
    IONetworkMedium *medium;

    OSDictionary *mediumDict = OSDictionary::withCapacity(2);
    if (mediumDict == NULL) {
        DRVERR("Cannot allocate OSDictionary\n");
        return false;
    }

    medium = IONetworkMedium::medium(kIOMediumIEEE80211, 54000000);
    IONetworkMedium::addMedium(mediumDict, medium);
    medium->release();
    if (primary) {
        *primary = medium;
    }
    medium = IONetworkMedium::medium(kIOMediumIEEE80211None, 0);
    IONetworkMedium::addMedium(mediumDict, medium);
    medium->release();

    bool result = publishMediumDictionary(mediumDict);
    if (!result) {
        DRVERR("Cannot publish medium dictionary!\n");
    }

    mediumDict->release();
    return result;
}

IOReturn AirportSequoia::selectMedium(const IONetworkMedium *medium)
{
    setSelectedMedium(medium);
    return kIOReturnSuccess;
}

bool AirportSequoia::
setLinkStatus(UInt32 status, const IONetworkMedium *activeMedium, UInt64 speed, OSData *data)
{
    struct _ifnet *ifq = &fHalService->get80211Controller()->ic_ac.ac_if;
    if (status == currentStatus) {
        return true;
    }
    bool ret = IO80211Controller::setLinkStatus(status, activeMedium, speed, data);
    currentStatus = status;
    if (fNetIf) {
        if (status & kIONetworkLinkActive) {
#ifdef __PRIVATE_SPI__
            bsdInterface->startOutputThread();
#endif
            getCommandGate()->runAction(setLinkStateGated, (void *)kIO80211NetworkLinkUp, (void *)0);
        } else if (!(status & kIONetworkLinkNoNetworkChange)) {
#ifdef __PRIVATE_SPI__
            bsdInterface->stopOutputThread();
            bsdInterface->flushOutputQueue();
#endif
            ifq_flush(&ifq->if_snd);
            mq_purge(&fHalService->get80211Controller()->ic_mgtq);
            getCommandGate()->runAction(setLinkStateGated, (void *)kIO80211NetworkLinkDown, (void *)(uint64_t)fHalService->get80211Controller()->ic_deauth_reason);
        }
    }
    return ret;
}

IOReturn AirportSequoia::
setLinkStateGated(OSObject *target, void *arg0, void *arg1, void *arg2, void *arg3)
{
    AirportSequoia *that = OSDynamicCast(AirportSequoia, target);
    // Sequoia: setLinkState(IO80211LinkState, unsigned int, bool, unsigned int)
    IOReturn ret = that->fNetIf->setLinkState((IO80211LinkState)(uint64_t)arg0, (unsigned int)(uint64_t)arg1, false, 0);
    that->fNetIf->setRunningState((IO80211LinkState)(uint64_t)arg0 == kIO80211NetworkLinkUp);
    that->fNetIf->postMessage(APPLE80211_M_LINK_CHANGED, NULL, 0, false);
    that->fNetIf->postMessage(APPLE80211_M_BSSID_CHANGED, NULL, 0, false);
    that->fNetIf->postMessage(APPLE80211_M_SSID_CHANGED, NULL, 0, false);
    if ((IO80211LinkState)(uint64_t)arg0 == kIO80211NetworkLinkUp) {
        that->fNetIf->reportLinkStatus(3, 0x80);
    } else {
        that->fNetIf->reportLinkStatus(1, 0);
    }
    that->bsdInterface->setLinkState((IO80211LinkState)(uint64_t)arg0);
    return ret;
}

#ifdef __PRIVATE_SPI__
IOReturn AirportSequoia::outputStart(IONetworkInterface *interface, IOOptionBits options)
{
    struct _ifnet *ifp = &fHalService->get80211Controller()->ic_ac.ac_if;
    mbuf_t m = NULL;
    if (ifq_is_oactive(&ifp->if_snd))
        return kIOReturnNoResources;
    while (kIOReturnSuccess == interface->dequeueOutputPackets(1, &m)) {
        if (outputPacket(m, NULL) != kIOReturnOutputSuccess ||
            ifq_is_oactive(&ifp->if_snd))
            return kIOReturnNoResources;
    }
    return kIOReturnSuccess;
}

IOReturn AirportSequoia::networkInterfaceNotification(IONetworkInterface *interface, uint32_t type, void *argument)
{
    DRVLOG("%s\n", __FUNCTION__);
    return kIOReturnSuccess;
}
#endif

UInt32 AirportSequoia::outputPacket(mbuf_t m, void *param)
{
    IOReturn ret = kIOReturnOutputSuccess;
    struct _ifnet *ifp = &fHalService->get80211Controller()->ic_ac.ac_if;

    if (fHalService->get80211Controller()->ic_state != IEEE80211_S_RUN || ifp->if_snd.queue == NULL) {
        if (m && mbuf_type(m) != MBUF_TYPE_FREE)
            freePacket(m);
        return kIOReturnOutputDropped;
    }
    if (m == NULL) {
        DRVERR("%s m==NULL!!\n", __FUNCTION__);
        ifp->netStat->outputErrors++;
        ret = kIOReturnOutputDropped;
    }
    if (!(mbuf_flags(m) & MBUF_PKTHDR)) {
        DRVERR("%s pkthdr is NULL!!\n", __FUNCTION__);
        ifp->netStat->outputErrors++;
        freePacket(m);
        ret = kIOReturnOutputDropped;
    }
    if (mbuf_type(m) == MBUF_TYPE_FREE) {
        DRVERR("%s mbuf is FREE!!\n", __FUNCTION__);
        ifp->netStat->outputErrors++;
        ret = kIOReturnOutputDropped;
    }
    size_t len = mbuf_len(m);
    ether_header_t *eh = (ether_header_t *)mbuf_data(m);
    if (len >= sizeof(ether_header_t) && eh->ether_type == htons(ETHERTYPE_PAE)) { // EAPOL
        const char* dump = hexdump((uint8_t*)mbuf_data(m), len);
        DRVLOG("output EAPOL packet, len: %zu, data: %s\n", len, dump ? dump : "no mem");
        if (dump)
            IOFree((void*)dump, 3 * len + 1);
    }
    if (!ifp->if_snd.queue->lockEnqueue(m)) {
        freePacket(m);
        ret = kIOReturnOutputDropped;
    }
    (*ifp->if_start)(ifp);
    return ret;
}

const OSString *AirportSequoia::newVendorString() const
{
    return OSString::withCString("Apple");
}

const OSString *AirportSequoia::newModelString() const
{
    return OSString::withCString(fHalService->getDriverInfo()->getFirmwareName());
}

IOReturn AirportSequoia::getHardwareAddress(IOEthernetAddress *addrP)
{
    if (IEEE80211_ADDR_EQ(etheranyaddr, fHalService->get80211Controller()->ic_myaddr))
        return kIOReturnError;
    else {
        IEEE80211_ADDR_COPY(addrP, fHalService->get80211Controller()->ic_myaddr);
        return kIOReturnSuccess;
    }
}

IOReturn AirportSequoia::setHardwareAddress(const void *addrP, UInt32 addrBytes)
{
    if (!fNetIf || !addrP)
        return kIOReturnError;
    if_setlladdr(&fHalService->get80211Controller()->ic_ac.ac_if, (const UInt8 *)addrP);
    if (fHalService->get80211Controller()->ic_state > IEEE80211_S_INIT) {
        fHalService->disable(bsdInterface);
        fHalService->enable(bsdInterface);
    }
    return kIOReturnSuccess;
}

UInt32 AirportSequoia::getFeatures() const
{
    return fHalService->getDriverInfo()->supportedFeatures();
}

IOReturn AirportSequoia::setPromiscuousMode(IOEnetPromiscuousMode mode)
{
    return kIOReturnSuccess;
}

IOReturn AirportSequoia::setMulticastMode(IOEnetMulticastMode mode)
{
    return kIOReturnSuccess;
}

IOReturn AirportSequoia::setMulticastList(IOEthernetAddress *addr, UInt32 len)
{
    return fHalService->getDriverController()->setMulticastList(addr, len);
}

IOReturn AirportSequoia::getPacketFilters(const OSSymbol *group, UInt32 *filters) const
{
    IOReturn rtn = kIOReturnSuccess;
    if (group == gIOEthernetWakeOnLANFilterGroup && magicPacketSupported)
        *filters = kIOEthernetWakeOnMagicPacket;
    else if (group == gIONetworkFilterGroup)
        *filters = kIOPacketFilterMulticast | kIOPacketFilterPromiscuous;
    else
        rtn = IOEthernetController::getPacketFilters(group, filters);
    return rtn;
}

IOReturn AirportSequoia::enableAdapter(IONetworkInterface *netif)
{
    fHalService->enable(netif);
    watchdogTimer->setTimeoutMS(kWatchDogTimerPeriod);
    watchdogTimer->enable();
    return kIOReturnSuccess;
}

void AirportSequoia::disableAdapter(IONetworkInterface *netif)
{
    watchdogTimer->cancelTimeout();
    watchdogTimer->disable();
    fHalService->disable(netif);
}

IOReturn AirportSequoia::
tsleepHandler(OSObject *owner, void *arg0, void *arg1, void *arg2, void *arg3)
{
    AirportSequoia *dev = OSDynamicCast(AirportSequoia, owner);
    if (dev == 0)
        return kIOReturnError;

    if (arg1 == 0) {
        if (_fCommandGate->commandSleep(arg0, THREAD_INTERRUPTIBLE) == THREAD_AWAKENED)
            return kIOReturnSuccess;
        else
            return kIOReturnTimeout;
    } else {
        AbsoluteTime deadline;
        clock_interval_to_deadline((*(int*)arg1), kNanosecondScale, reinterpret_cast<uint64_t*>(&deadline));
        if (_fCommandGate->commandSleep(arg0, deadline, THREAD_INTERRUPTIBLE) == THREAD_AWAKENED)
            return kIOReturnSuccess;
        else
            return kIOReturnTimeout;
    }
}

// --- Power management --------------------------------------------------------

bool AirportSequoia::initPCIPowerManagment(IOPCIDevice *provider)
{
    UInt16 reg16;

    reg16 = provider->configRead16(kIOPCIConfigCommand);
    reg16 |= (kIOPCICommandBusMaster | kIOPCICommandMemorySpace | kIOPCICommandMemWrInvalidate);
    reg16 &= ~kIOPCICommandIOSpace;
    provider->configWrite16(kIOPCIConfigCommand, reg16);
    provider->findPCICapability(kIOPCIPowerManagementCapability, &pmPCICapPtr);
    if (pmPCICapPtr) {
        UInt16 pciPMCReg = provider->configRead32(pmPCICapPtr) >> 16;
        if (pciPMCReg & kPCIPMCPMESupportFromD3Cold)
            magicPacketSupported = true;
        provider->configWrite16((pmPCICapPtr + 4), 0x8000);
        IOSleep(10);
    }
    return true;
}

static IOPMPowerState powerStateArray[kPowerStateCount] =
{
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, kIOPMDeviceUsable, kIOPMPowerOn, kIOPMPowerOn, 0, 0, 0, 0, 0, 0, 0, 0}
};

void AirportSequoia::unregistPM()
{
    if (powerOffThreadCall) {
        thread_call_free(powerOffThreadCall);
        powerOffThreadCall = NULL;
    }
    if (powerOnThreadCall) {
        thread_call_free(powerOnThreadCall);
        powerOnThreadCall = NULL;
    }
}

IOReturn AirportSequoia::setPowerState(unsigned long powerStateOrdinal, IOService *policyMaker)
{
    IOReturn result = IOPMAckImplied;

    if (pmPowerState == powerStateOrdinal)
        return result;
    switch (powerStateOrdinal) {
        case kPowerStateOff:
            if (powerOffThreadCall) {
                retain();
                if (thread_call_enter(powerOffThreadCall))
                    release();
                result = 5000000;
            }
            break;
        case kPowerStateOn:
            if (powerOnThreadCall) {
                retain();
                if (thread_call_enter(powerOnThreadCall))
                    release();
                result = 5000000;
            }
            break;
        default:
            break;
    }
    return result;
}

IOReturn AirportSequoia::setWakeOnMagicPacket(bool active)
{
    magicPacketEnabled = active;
    return kIOReturnSuccess;
}

static void handleSetPowerStateOff(thread_call_param_t param0, thread_call_param_t param1)
{
    AirportSequoia *self = (AirportSequoia *)param0;

    if (param1 == 0) {
        self->getCommandGate()->runAction((IOCommandGate::Action)handleSetPowerStateOff, (void *)1);
    } else {
        self->setPowerStateOff();
        self->release();
    }
}

static void handleSetPowerStateOn(thread_call_param_t param0, thread_call_param_t param1)
{
    AirportSequoia *self = (AirportSequoia *)param0;

    if (param1 == 0) {
        self->getCommandGate()->runAction((IOCommandGate::Action)handleSetPowerStateOn, (void *)1);
    } else {
        self->setPowerStateOn();
        self->release();
    }
}

IOReturn AirportSequoia::registerWithPolicyMaker(IOService *policyMaker)
{
    IOReturn ret;

    pmPowerState = kPowerStateOn;
    pmPolicyMaker = policyMaker;

    powerOffThreadCall = thread_call_allocate((thread_call_func_t)handleSetPowerStateOff, (thread_call_param_t)this);
    powerOnThreadCall = thread_call_allocate((thread_call_func_t)handleSetPowerStateOn, (thread_call_param_t)this);
    ret = pmPolicyMaker->registerPowerDriver(this, powerStateArray, kPowerStateCount);
    return ret;
}

void AirportSequoia::setPowerStateOff()
{
    DRVLOG("%s\n", __FUNCTION__);
    pmPowerState = kPowerStateOff;
    disableAdapter(bsdInterface);
    pmPolicyMaker->acknowledgeSetPowerState();
}

void AirportSequoia::setPowerStateOn()
{
    DRVLOG("%s\n", __FUNCTION__);
    pmPowerState = kPowerStateOn;
    pmPolicyMaker->acknowledgeSetPowerState();
}
