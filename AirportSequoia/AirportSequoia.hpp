//
//  AirportSequoia.hpp
//  Нативный Intel Wi-Fi драйвер для macOS Sequoia (15.x)
//  Архитектура: IO80211Controller + IO80211InfraInterface
//  Раскладка базовых классов восстановлена из IO80211Family 15.7.4 (24G517)
//

#ifndef AIRPORT_SEQUOIA_HPP
#define AIRPORT_SEQUOIA_HPP

#include <IOKit/IOService.h>
#include <IOKit/pci/IOPCIDevice.h>
#include <IOKit/IOLib.h>
#include <IOKit/IOLocks.h>
#include <IOKit/IOWorkLoop.h>
#include <IOKit/IOCommandGate.h>
#include <IOKit/IOTimerEventSource.h>
#include <IOKit/network/IOEthernetController.h>
#include <IOKit/network/IOGatedOutputQueue.h>
#include <libkern/c++/OSString.h>

#include <net80211/ieee80211_var.h>

#include <Airport/Sequoia/IO80211Sequoia.h>
#include <Airport/Sequoia/IO80211SequoiaInfraProtocol.h>
#include <Airport/IO80211WorkQueue.h>
#include <HAL/ItlHalService.hpp>

#include "AirportSequoiaEthernetInterface.hpp"

#define AIRPORT_SEQUOIA_VERSION "0.2.0"

enum
{
    kPowerStateOff = 0,
    kPowerStateOn,
    kPowerStateCount
};

#define kWatchDogTimerPeriod 1000

class AirportSequoia;

// ---------------------------------------------------------------------------
// Интерфейсный объект: живёт в стеке Skywalk, отдаёт системе 80211 API.
// Sequoia routes STA get/set commands through IO80211InfraProtocol slots.
// ---------------------------------------------------------------------------
class AirportSequoiaInfra : public IO80211InfraProtocol {
    OSDeclareDefaultStructors(AirportSequoiaInfra)

public:
    bool init(IOService *provider);
    void free() override;
    IOReturn setMacAddress(ether_addr &) override;

#define IO80211_SEQUOIA_DECLARE_INFRA_OVERRIDE(slot, method, parameter_type) \
    struct method##Tag {};                                                   \
    IOReturn method(parameter_type *) override;
    IO80211_SEQUOIA_INFRA_PROTOCOL_METHODS(
        IO80211_SEQUOIA_DECLARE_INFRA_OVERRIDE)
#undef IO80211_SEQUOIA_DECLARE_INFRA_OVERRIDE

private:
    template <typename Tag, typename Parameter>
    IOReturn dispatchProtocol(Tag tag, Parameter *data)
    {
        return handleProtocol(tag, data);
    }

    IOReturn dispatchProtocol(setWCL_SCAN_REQTag, apple80211ScanRequest *);
    IOReturn dispatchProtocol(setWCL_ASSOCIATETag,
                              apple80211AssocCandidates *);
    static IOReturn handleWclScanGated(OSObject *, void *, void *, void *,
                                       void *);
    static IOReturn handleWclAssociateGated(OSObject *, void *, void *, void *,
                                            void *);

    template <typename Tag, typename Parameter>
    IOReturn handleProtocol(Tag, Parameter *)
    {
        return kIOReturnUnsupported;
    }

    IOReturn handleProtocol(getCHANNELTag, apple80211_channel_data *);
    IOReturn handleProtocol(getPOWERSAVETag, apple80211_powersave_data *);
    IOReturn handleProtocol(getTXPOWERTag, apple80211_txpower_data *);
    IOReturn handleProtocol(getRATETag, apple80211_rate_data *);
    IOReturn handleProtocol(getOP_MODETag, apple80211_opmode_data *);
    IOReturn handleProtocol(getRSSITag, apple80211_rssi_data *);
    IOReturn handleProtocol(getSUPPORTED_CHANNELSTag,
                            apple80211_sup_channel_data *);
    IOReturn handleProtocol(getMCSTag, apple80211_mcs_data *);
    IOReturn handleProtocol(getMCS_VHTTag, apple80211_mcs_vht_data *);
    IOReturn handleProtocol(getNSSTag, apple80211_nss_data *);
    IOReturn handleProtocol(setCIPHER_KEYTag, apple80211_key *);
    IOReturn handleProtocol(setWCL_SCAN_REQTag, apple80211ScanRequest *);
    IOReturn handleProtocol(setWCL_ASSOCIATETag,
                            apple80211AssocCandidates *);
    IOReturn handleProtocol(setWCL_LINK_UP_DONETag, void *);
    IOReturn associateSSID(const UInt8 *, UInt32, const ether_addr &,
                           UInt32, UInt32, const UInt8 *, UInt32, UInt16);
    IOReturn removeKey(const apple80211_key *);
    IOReturn installPTK(const UInt8 *, size_t, const UInt8 *);
    IOReturn installGTK(const UInt8 *, size_t, UInt16, const UInt8 *);

public:
    AirportSequoia *instance;
    ItlHalService *fHalService;
};

// ---------------------------------------------------------------------------
// Контроллер драйвера: PCI -> HAL (firmware) -> интерфейсы.
// ---------------------------------------------------------------------------
class AirportSequoia : public IO80211Controller {
    OSDeclareDefaultStructors(AirportSequoia)

public:
    bool init(OSDictionary *properties) override;
    void free() override;
    IOService * probe(IOService *provider, SInt32 *score) override;
    bool start(IOService *provider) override;
    void stop(IOService *provider) override;

    // IO80211Controller 15.7.4 pure slots.
    bool isCommandProhibited(int command) override;
    bool createWorkQueue() override;
    IO80211WorkQueue * getWorkQueue() const override;
    SInt32 handleCardSpecific(IO80211SkywalkInterface *, unsigned long,
                             void *, bool) override;
    IOReturn getDRIVER_VERSION(IO80211SkywalkInterface *,
                               apple80211_version_data *) override;
    IOReturn getHARDWARE_VERSION(IO80211SkywalkInterface *,
                                 apple80211_version_data *) override;
    IOReturn getCARD_CAPABILITIES(IO80211SkywalkInterface *,
                                  apple80211_capability_data *) override;
    IOReturn getPOWER(IO80211SkywalkInterface *,
                      apple80211_power_data *) override;
    IOReturn setPOWER(IO80211SkywalkInterface *,
                      apple80211_power_data *) override;
    IOReturn getCOUNTRY_CODE(IO80211SkywalkInterface *,
                             apple80211_country_code_data *) override;
    IOReturn setCOUNTRY_CODE(IO80211SkywalkInterface *,
                             apple80211_country_code_data *) override;
    IOReturn setGET_DEBUG_INFO(IO80211SkywalkInterface *,
                               apple80211_debug_command *) override;
    CCLogStream * getLogger() const override;
    CCFaultReporter * getFaultReporterFromDriver() override;

    // IOEthernetController: единственный pure из SDK
    IOReturn getHardwareAddress(IOEthernetAddress *addrP) override;

    // IONetworkController / IOService
    IONetworkInterface * createInterface() override;
    bool configureInterface(IONetworkInterface *netif) override;
    UInt32 outputPacket(mbuf_t, void *param) override;
    UInt32 getFeatures() const override;
    const OSString * newVendorString() const override;
    const OSString * newModelString() const override;
    IOReturn selectMedium(const IONetworkMedium *medium) override;
    IOReturn setPromiscuousMode(IOEnetPromiscuousMode mode) override;
    IOReturn setMulticastMode(IOEnetMulticastMode mode) override;
    IOReturn setMulticastList(IOEthernetAddress *addr, UInt32 len) override;
    IOReturn setMulticastList(ether_addr const *addr, UInt count) override;
    IOReturn getPacketFilters(const OSSymbol *group, UInt32 *filters) const override;
    IOReturn setHardwareAddress(const void *addr, UInt32 addrBytes) override;
    bool setLinkStatus(UInt32 status, const IONetworkMedium *activeMedium = 0,
                       UInt64 speed = 0, OSData *data = 0) override;
#ifdef __PRIVATE_SPI__
    IOReturn outputStart(IONetworkInterface *interface, IOOptionBits options) override;
    IOReturn networkInterfaceNotification(IONetworkInterface *interface,
                                          uint32_t type, void *argument) override;
#endif
    // Power management
    IOReturn registerWithPolicyMaker(IOService *policyMaker) override;
    IOReturn setPowerState(unsigned long powerStateOrdinal, IOService *policyMaker) override;
    IOReturn setWakeOnMagicPacket(bool active) override;

public:
    // helpers (не виртуалы)
    IOReturn enableAdapter(IONetworkInterface *netif);
    void disableAdapter(IONetworkInterface *netif);
    bool createMediumTables(const IONetworkMedium **primary);
    void quiesceAsyncSources();
    void releaseAll();
    void watchdogAction(IOTimerEventSource *timer);
    void setPowerStateOff(void);
    void setPowerStateOn(void);
    void acknowledgePowerTransition();
    void unregistPM();
    bool initPCIPowerManagment(IOPCIDevice *provider);
    IOCommandGate *retainMainCommandGate();
    bool isStopping();
    bool claimWclScanToken();
    bool isWclScanPending();
    void clearWclScanToken();
    void clearEventHandlerAfterDrain();

    static IOReturn setLinkStateGated(OSObject *target, void *arg0, void *arg1, void *arg2, void *arg3);
    static IOReturn tsleepHandler(OSObject *owner, void *arg0 = 0, void *arg1 = 0, void *arg2 = 0, void *arg3 = 0);
    static IOReturn setPowerStateOffGated(OSObject *owner, void *arg0 = 0,
                                          void *arg1 = 0, void *arg2 = 0,
                                          void *arg3 = 0);
    static IOReturn setPowerStateOnGated(OSObject *owner, void *arg0 = 0,
                                         void *arg1 = 0, void *arg2 = 0,
                                         void *arg3 = 0);
    static IOReturn quiesceGated(OSObject *owner, void *arg0 = 0,
                                 void *arg1 = 0, void *arg2 = 0,
                                 void *arg3 = 0);
    static void eventHandler(struct ieee80211com *, int, void *);

public:
    IOPCIDevice *pciNub;
    ItlHalService *fHalService;
    AirportSequoiaInfra *fNetIf;
    AirportSequoiaEthernetInterface *bsdInterface;
    IONetworkStats *fpNetStats;
    IOTimerEventSource *watchdogTimer;
    IOWorkLoop *fWatchdogWorkLoop;
    // Reuses the former scan-timer pointer's eight-byte slot without changing
    // the recovered Sequoia class size.
    IOSimpleLock *lifecycleLock;

    // состояние
    uint8_t power_state;
    UInt32 currentStatus;

    // pm
    thread_call_t powerOnThreadCall;
    thread_call_t powerOffThreadCall;
    UInt32 pmPowerState;
    IOService *pmPolicyMaker;
    UInt8 pmPCICapPtr;
    bool magicPacketEnabled;
    bool magicPacketSupported;
    char geoLocationCountryCode[3];
    bool stopping;
    // Consumes the former final padding byte. Only a WCL request accepted by
    // slot 592 may consume a net80211 SCAN_DONE event.
    bool wclScanPending;
};

static_assert(sizeof(AirportSequoiaInfra) == 0x130,
              "AirportSequoiaInfra Sequoia ABI size mismatch");
static_assert(__offsetof(AirportSequoiaInfra, instance) == 0x120,
              "AirportSequoiaInfra::instance ABI offset mismatch");
static_assert(__offsetof(AirportSequoiaInfra, fHalService) == 0x128,
              "AirportSequoiaInfra::fHalService ABI offset mismatch");
static_assert(sizeof(AirportSequoia) == 0x198,
              "AirportSequoia Sequoia ABI size mismatch");
static_assert(__offsetof(AirportSequoia, pciNub) == 0x128,
              "AirportSequoia::pciNub ABI offset mismatch");
static_assert(__offsetof(AirportSequoia, lifecycleLock) == 0x160,
              "AirportSequoia::lifecycleLock ABI offset mismatch");
static_assert(__offsetof(AirportSequoia, wclScanPending) == 0x197,
              "AirportSequoia::wclScanPending ABI offset mismatch");

#endif /* AIRPORT_SEQUOIA_HPP */
