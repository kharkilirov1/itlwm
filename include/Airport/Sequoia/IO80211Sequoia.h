// IO80211Sequoia.h — классы IO80211Family 15.7.4 (24G517), v2
// Раскладка vtable = ground truth из бинарника. Цепочки:
//   IO80211Controller : IOEthernetController (appended slots 394-397)
//   IO80211InfraInterface / IO80211VirtualInterface : IO80211SkywalkInterface
//     (своих виртуалов НЕ добавляют — vtable кончается на 397)
#ifndef IO80211_SEQUOIA_H
#define IO80211_SEQUOIA_H

#include <Airport/Sequoia/IO80211SequoiaSkywalk.h>

class IO80211APIUserClient;
class IO80211AssociationJoinSnapshot;
class IO80211CountryCodeOp;
class IO80211FeatureCode;
class IO80211FlowQueue;
class IO80211FlowQueueHash;
class IO80211TimerSource;
class IOReporter;
class CCPipe;
class CCLogStream;

// ---------------------------------------------------------------------------
// IO80211VirtualInterface — этап 2 (AWDL). Своих виртуалов нет.
// ---------------------------------------------------------------------------
class IO80211VirtualInterface : public IO80211SkywalkInterface {
    OSDeclareAbstractStructors(IO80211VirtualInterface)
};

// ---------------------------------------------------------------------------
// IO80211InfraInterface — основной STA-интерфейс. Своих виртуалов нет;
// все его методы — переопределения слотов базовой цепочки.
// ---------------------------------------------------------------------------
class IO80211InfraInterface : public IO80211SkywalkInterface {
    OSDeclareAbstractStructors(IO80211InfraInterface)

public:
    // non-virtual (из бинарника 15.7.4)
    IOReturn registerInfraEthernetInterface(IOSkywalkEthernetInterface::RegistrationInfo *,
                                            IOSkywalkPacketQueue * *,unsigned int,
                                            IOSkywalkPacketBufferPool *,IOSkywalkPacketBufferPool *);
};

// ---------------------------------------------------------------------------
// IO80211Controller : IOEthernetController
// Собственные виртуалы — только слоты 394-397 (после IOEthernetController).
// Слот 394 в бинарнике ___cxa_pure_virtual: единственный чистый виртуал —
// центральная точка входа ioctl, которую ОБЯЗАН реализовать драйвер.
// По сигнатуре Sonoma (IO80211ControllerV2.h) это apple80211_ioctl.
// ---------------------------------------------------------------------------
class IO80211Controller : public IOEthernetController {
    OSDeclareAbstractStructors(IO80211Controller)

public:
    // slot 394 — pure в бинарнике
    virtual SInt32 apple80211_ioctl(IO80211SkywalkInterface *,unsigned long,void *,bool,bool);
    // slot 395
    virtual bool createWorkQueue(void);
    // slot 396
    virtual IOReturn debugStateInit(void); // RT?
    // slot 397
    virtual IOWorkLoop * getWorkQueue(void); // RT? (Sonoma: IO80211WorkQueue*, класс удалён из Sequoia)

public:
    // non-virtual (из бинарника 15.7.4)
    bool attachInterface(OSObject *,IOService *); // RT? (Sonoma: bool)
    bool detachInterface(OSObject *,bool); // RT?
    IO80211InfraInterface * getInfraInterface(void); // RT?
    bool attachVirtualInterface(IO80211VirtualInterface * *,ether_addr *,unsigned int,bool); // RT?
    IO80211VirtualInterface * createVirtualInterface(ether_addr *,unsigned int); // RT?
    SInt32 enableFeature(IO80211FeatureCode,void *); // RT?
};

#endif /* IO80211_SEQUOIA_H */
