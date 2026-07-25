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
#include <IOKit/IOWorkLoop.h>
#include <IOKit/IOCommandGate.h>
#include <IOKit/IOTimerEventSource.h>
#include <IOKit/network/IOEthernetController.h>
#include <IOKit/network/IOGatedOutputQueue.h>
#include <libkern/c++/OSString.h>

#include <net80211/ieee80211_var.h>

#include <Airport/Sequoia/IO80211Sequoia.h>
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
// Этап 1: минимум — init + доступ к HAL. get/set — этап 1b.
// ---------------------------------------------------------------------------
class AirportSequoiaInfra : public IO80211InfraInterface {
    OSDeclareDefaultStructors(AirportSequoiaInfra)

public:
    bool init(IOService *provider);
    void free() override;

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

    // IO80211Controller: slot 394 — pure в бинарнике, обязаны реализовать
    SInt32 apple80211_ioctl(IO80211SkywalkInterface *, unsigned long, void *, bool, bool) override;
    bool createWorkQueue() override;
    IOWorkLoop * getWorkQueue() override;

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
    void releaseAll();
    void watchdogAction(IOTimerEventSource *timer);
    void setPowerStateOff(void);
    void setPowerStateOn(void);
    void unregistPM();
    bool initPCIPowerManagment(IOPCIDevice *provider);

    static IOReturn setLinkStateGated(OSObject *target, void *arg0, void *arg1, void *arg2, void *arg3);
    static IOReturn tsleepHandler(OSObject *owner, void *arg0 = 0, void *arg1 = 0, void *arg2 = 0, void *arg3 = 0);
    static void eventHandler(struct ieee80211com *, int, void *);
    static void fakeScanDone(OSObject *owner, IOTimerEventSource *sender);

public:
    IOPCIDevice *pciNub;
    ItlHalService *fHalService;
    AirportSequoiaInfra *fNetIf;
    AirportSequoiaEthernetInterface *bsdInterface;
    IONetworkStats *fpNetStats;
    IOTimerEventSource *watchdogTimer;
    IOWorkLoop *fWatchdogWorkLoop;
    IOTimerEventSource *scanSource;

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
};

#endif /* AIRPORT_SEQUOIA_HPP */
