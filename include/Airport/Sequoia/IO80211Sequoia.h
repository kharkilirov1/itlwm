// IO80211Sequoia.h — классы IO80211Family 15.7.4 (24G517), v2
// Раскладка vtable = ground truth из бинарника. Цепочки:
//   IO80211Controller : IOEthernetController (appended slots 394-462)
//   IO80211InfraInterface / IO80211VirtualInterface : IO80211SkywalkInterface
#ifndef IO80211_SEQUOIA_H
#define IO80211_SEQUOIA_H

#include <Airport/Sequoia/IO80211SequoiaSkywalk.h>

class IO80211APIUserClient;
class IO80211AssociationJoinSnapshot;
class IO80211CountryCodeOp;
class IO80211FeatureCode;
class IO80211FlowQueue;
class IO80211FlowQueueLegacy;
class IO80211FlowQueueHash;
class IO80211InterfaceAVCAdvisory;
class IO80211IORecursiveLock;
class IO80211PostOffice;
class IO80211TimerSource;
class IO80211WorkQueue;
class IOCommandGate;
class IOReporter;
class FlowIdMetadata;
class CCPipe;
class CCLogStream;
class CCFaultReporter;
struct apple80211_platform_config;

// ---------------------------------------------------------------------------
// IO80211VirtualInterface — этап 2 (AWDL). Своих виртуалов нет.
// ---------------------------------------------------------------------------
class IO80211VirtualInterface : public IO80211SkywalkInterface {
    OSDeclareAbstractStructors(IO80211VirtualInterface)

protected:
    uint8_t _io80211VirtualInterfaceOpaque[0x8];
};

static_assert(sizeof(IO80211VirtualInterface) == 0x120,
              "IO80211VirtualInterface Sequoia ABI size mismatch");

// ---------------------------------------------------------------------------
// IO80211InfraInterface — основной STA-интерфейс. Своих виртуалов нет;
// все его методы — переопределения слотов базовой цепочки.
// ---------------------------------------------------------------------------
class IO80211InfraInterface : public IO80211SkywalkInterface {
    OSDeclareAbstractStructors(IO80211InfraInterface)

public:
    // Overrides present in the 15.7.4 IO80211InfraInterface vtable. Declaring
    // them here is required so a third-party derived vtable references the
    // IO80211InfraInterface implementations rather than older base versions.
    virtual bool init(void) APPLE_KEXT_OVERRIDE;
    virtual void free(void) APPLE_KEXT_OVERRIDE;
    virtual IOReturn configureReport(IOReportChannelList *, UInt, void *, void *) APPLE_KEXT_OVERRIDE;
    virtual IOReturn updateReport(IOReportChannelList *, UInt, void *, void *) APPLE_KEXT_OVERRIDE;
    virtual bool start(IOService *) APPLE_KEXT_OVERRIDE;
    virtual void stop(IOService *) APPLE_KEXT_OVERRIDE;
    virtual IOReturn setPowerState(unsigned long, IOService *) APPLE_KEXT_OVERRIDE;
    virtual SInt32 initBSDInterfaceParameters(ifnet_init_eparams *, sockaddr_dl **) APPLE_KEXT_OVERRIDE;
    virtual bool prepareBSDInterface(ifnet_t, UInt) APPLE_KEXT_OVERRIDE;
    virtual IOReturn processBSDCommand(ifnet_t, UInt, void *) APPLE_KEXT_OVERRIDE;
    virtual SInt32 setInterfaceEnable(bool) APPLE_KEXT_OVERRIDE;
    virtual UInt getHardwareAssists(void) APPLE_KEXT_OVERRIDE;
    virtual bool bpfTap(UInt, UInt) APPLE_KEXT_OVERRIDE;
    virtual void hwConfigNicProxyData(nicproxy_info_s *) APPLE_KEXT_OVERRIDE;
    virtual void postMessage(UInt, void *, unsigned long, bool) APPLE_KEXT_OVERRIDE;
    virtual IOReturn recordOutputPackets(TxSubmissionDequeueStats *,
                                         TxSubmissionDequeueStats *) APPLE_KEXT_OVERRIDE;
    virtual void logTxPacket(IO80211NetworkPacket *, PacketSkywalkScratch *,
                             apple80211_wme_ac, bool) APPLE_KEXT_OVERRIDE;
    virtual void logTxCompletionPacket(IO80211NetworkPacket *, PacketSkywalkScratch *,
                                       unsigned char *, apple80211_wme_ac, int,
                                       UInt, bool, bool) APPLE_KEXT_OVERRIDE;
    virtual IOReturn recordCompletionPackets(TxCompletionEnqueueStats *,
                                             TxCompletionEnqueueStats *) APPLE_KEXT_OVERRIDE;
    virtual mbuf_flags_t inputPacket(IO80211NetworkPacket *, packet_info_tag *,
                                     ether_header *, bool *, bool) APPLE_KEXT_OVERRIDE;
    virtual SInt64 pendingPackets(unsigned char) APPLE_KEXT_OVERRIDE;
    virtual SInt64 packetSpace(unsigned char) APPLE_KEXT_OVERRIDE;
    virtual bool isDebounceOnGoing(void) APPLE_KEXT_OVERRIDE;
    virtual bool setLinkState(IO80211LinkState, UInt, bool, UInt) APPLE_KEXT_OVERRIDE;
    virtual IO80211LinkState linkState(void) APPLE_KEXT_OVERRIDE;
    virtual void setScanningState(UInt, bool, apple80211_scan_data *, int) APPLE_KEXT_OVERRIDE;
    virtual void setDataPathState(bool) APPLE_KEXT_OVERRIDE;
    virtual IOReturn getScanManager(void) APPLE_KEXT_OVERRIDE;
    virtual void updateLinkParameters(apple80211_interface_availability *) APPLE_KEXT_OVERRIDE;
    virtual void updateInterfaceCoexRiskPct(unsigned long long) APPLE_KEXT_OVERRIDE;
    virtual void setLQM(unsigned long long) APPLE_KEXT_OVERRIDE;
    virtual void updateLinkStatus(void) APPLE_KEXT_OVERRIDE;
    virtual void updateLinkStatusGated(void) APPLE_KEXT_OVERRIDE;
    virtual void setInterfaceExtendedCCA(apple80211_channel,
                                         apple80211_cca_report *) APPLE_KEXT_OVERRIDE;
    virtual void setInterfaceCCA(apple80211_channel, int) APPLE_KEXT_OVERRIDE;
    virtual void setInterfaceNF(apple80211_channel, long long) APPLE_KEXT_OVERRIDE;
    virtual void setInterfaceOFDMDesense(apple80211_channel, long long) APPLE_KEXT_OVERRIDE;
    virtual void setDebugFlags(unsigned long long, UInt) APPLE_KEXT_OVERRIDE;
    virtual SInt64 debugFlags(void) APPLE_KEXT_OVERRIDE;
    virtual void setInterfaceChipCounters(apple80211_stat_report *,
                                          apple80211_chip_counters_tx *,
                                          apple80211_chip_error_counters_tx *,
                                          apple80211_chip_counters_rx *) APPLE_KEXT_OVERRIDE;
    virtual void setInterfaceMIBdot11(
        apple80211_stat_report *,
        apple80211_ManagementInformationBasedot11_counters *) APPLE_KEXT_OVERRIDE;
    virtual void setFrameStats(apple80211_stat_report *,
                               apple80211_frame_counters *) APPLE_KEXT_OVERRIDE;
    virtual void setInfraSpecificFrameStats(
        apple80211_stat_report *,
        apple80211_infra_specific_stats *) APPLE_KEXT_OVERRIDE;
    virtual SInt64 getWmeTxCounters(unsigned long long *) APPLE_KEXT_OVERRIDE;
    virtual void setPeerManagerLogFlag(UInt, UInt, UInt) APPLE_KEXT_OVERRIDE;
    virtual void setWoWEnabled(bool) APPLE_KEXT_OVERRIDE;
    virtual bool wowEnabled(void) APPLE_KEXT_OVERRIDE;
    virtual UInt64 createLinkQualityMonitor(IO80211Peer *,
                                            IOService *) APPLE_KEXT_OVERRIDE;
    virtual void releaseLinkQualityMonitor(IO80211Peer *) APPLE_KEXT_OVERRIDE;
    virtual int getAssocState(void) APPLE_KEXT_OVERRIDE;
    virtual void * getLQMSummary(apple80211_lqm_summary *) APPLE_KEXT_OVERRIDE;

    virtual IOReturn setLinkStateInternal(IO80211LinkState, UInt, bool, UInt); // 460
    virtual IOReturn setCurrentApAddress(ether_addr *);                        // 461
    virtual void setWCL_ADVISORTY_INFO(apple80211_wcl_advisory_info *);       // 462
    virtual IOReturn getWCL_TX_RX_LATENCY(apple80211_wcl_tx_rx_latency *);    // 463
    virtual IOReturn createLQMData(void);                                      // 464

    // non-virtual (из бинарника 15.7.4)
    IOReturn registerInfraEthernetInterface(IOSkywalkEthernetInterface::RegistrationInfo *,
                                            IOSkywalkPacketQueue * *,unsigned int,
                                            IOSkywalkPacketBufferPool *,IOSkywalkPacketBufferPool *);

protected:
    uint8_t _io80211InfraInterfaceOpaque[0x8];
};

static_assert(sizeof(IO80211InfraInterface) == 0x120,
              "IO80211InfraInterface Sequoia ABI size mismatch");

// ---------------------------------------------------------------------------
// IO80211Controller : IOEthernetController.
// Слоты 394-462 восстановлены из vtable IO80211Family 15.7.4 (24G517).
// Нумерация ниже относится к object vptr (без двух Itanium ABI header words).
// ---------------------------------------------------------------------------
class IO80211Controller : public IOEthernetController {
    OSDeclareAbstractStructors(IO80211Controller)

public:
    virtual bool isCommandProhibited(int) = 0;                              // 394
    virtual bool createWorkQueue(void);                                     // 395
    virtual void debugStateInit(void);                                      // 396
    virtual IO80211WorkQueue * getWorkQueue(void) const;                    // 397
    virtual void requestPacketTx(void *, UInt);                             // 398
    virtual IOCommandGate * getIO80211CommandGate(void) const;              // 399
    virtual IO80211SkywalkInterface * getPrimarySkywalkInterface(void);     // 400
    virtual int bpfOutputPacket(OSObject *, UInt, mbuf_t);                   // 401
    virtual SInt32 monitorModeSetEnabled(bool, UInt);                       // 402
    virtual SInt32 handleCardSpecific(IO80211SkywalkInterface *,
                                      unsigned long, void *, bool) = 0;     // 403
    virtual UInt32 hardwareOutputQueueDepth(void);                          // 404
    virtual SInt32 performCountryCodeOperation(IO80211CountryCodeOp);       // 405
    virtual void dataLinkLayerAttachComplete(void);                         // 406
    virtual SInt32 enableFeature(IO80211FeatureCode, void *);               // 407
    virtual IOReturn getDRIVER_VERSION(IO80211SkywalkInterface *,
                                       apple80211_version_data *) = 0;      // 408
    virtual IOReturn getHARDWARE_VERSION(IO80211SkywalkInterface *,
                                         apple80211_version_data *) = 0;    // 409
    virtual IOReturn getCARD_CAPABILITIES(IO80211SkywalkInterface *,
                                          apple80211_capability_data *) = 0;// 410
    virtual IOReturn getPOWER(IO80211SkywalkInterface *,
                              apple80211_power_data *) = 0;                 // 411
    virtual IOReturn setPOWER(IO80211SkywalkInterface *,
                              apple80211_power_data *) = 0;                 // 412
    virtual IOReturn getCOUNTRY_CODE(IO80211SkywalkInterface *,
                                     apple80211_country_code_data *) = 0;   // 413
    virtual IOReturn setCOUNTRY_CODE(IO80211SkywalkInterface *,
                                     apple80211_country_code_data *) = 0;   // 414
    virtual IOReturn setGET_DEBUG_INFO(IO80211SkywalkInterface *,
                                       apple80211_debug_command *) = 0;     // 415
    virtual IOReturn getPLATFORM_CONFIG(IO80211SkywalkInterface *,
                                        apple80211_platform_config *);      // 416
    virtual SInt32 enableVirtualInterface(IO80211VirtualInterface *);       // 417
    virtual SInt32 disableVirtualInterface(IO80211VirtualInterface *);      // 418
    virtual bool requiresExplicitMBufRelease(void);                         // 419
    virtual bool flowIdSupported(void);                                    // 420
    virtual IO80211FlowQueueLegacy *
        requestFlowQueue(FlowIdMetadata const *);                           // 421
    virtual void releaseFlowQueue(IO80211FlowQueue *);                      // 422
    virtual void getLogPipes(CCPipe **, CCPipe **, CCPipe **);              // 423
    virtual CCLogStream * getLogger(void) const = 0;                        // 424
    virtual void enableFeatureForLoggingFlags(unsigned long long);          // 425
    virtual IOReturn requestQueueSizeAndTimeout(unsigned short *,
                                                unsigned short *);          // 426
    virtual IOReturn enablePacketTimestamping(void);                        // 427
    virtual IOReturn disablePacketTimestamping(void);                       // 428
    virtual UInt getPacketTSCounter(void);                                  // 429
    virtual void * getDriverTextLog(void);                                  // 430
    virtual UInt32 selfDiagnosticsReport(int, char const *, UInt);           // 431
    virtual CCFaultReporter * getFaultReporterFromDriver(void) = 0;         // 432
    virtual IO80211IORecursiveLock * allocIO80211RecursiveLock(void);       // 433
    virtual UInt32 getDataQueueDepth(OSObject *);                           // 434
    virtual bool wasDynSARInFailSafeMode(void);                             // 435
    virtual void updateAdvisoryScoresIfNeed(void);                          // 436
    virtual UInt64 getAVCAdvisoryInfo(IO80211InterfaceAVCAdvisory *);       // 437
    virtual UInt32 getActionFramePoolCapacity(void);                        // 438
    virtual IO80211PostOffice * getPostOffice(void);                        // 439
    virtual IO80211PostOffice * CreatePostOffice(void);                     // 440
    virtual bool attachInterface(OSObject *, IOService *);                  // 441
    virtual void detachInterface(OSObject *, bool);                         // 442
    virtual IO80211VirtualInterface *
        createVirtualInterface(ether_addr *, UInt);                         // 443
    virtual bool attachVirtualInterface(IO80211VirtualInterface **,
                                        ether_addr *, UInt, bool);          // 444
    virtual bool detachVirtualInterface(IO80211VirtualInterface *, bool);   // 445

    OSMetaClassDeclareReservedUnused(IO80211Controller, 0);                 // 446
    OSMetaClassDeclareReservedUnused(IO80211Controller, 1);                 // 447
    OSMetaClassDeclareReservedUnused(IO80211Controller, 2);                 // 448
    OSMetaClassDeclareReservedUnused(IO80211Controller, 3);                 // 449
    OSMetaClassDeclareReservedUnused(IO80211Controller, 4);                 // 450
    OSMetaClassDeclareReservedUnused(IO80211Controller, 5);                 // 451
    OSMetaClassDeclareReservedUnused(IO80211Controller, 6);                 // 452
    OSMetaClassDeclareReservedUnused(IO80211Controller, 7);                 // 453
    OSMetaClassDeclareReservedUnused(IO80211Controller, 8);                 // 454
    OSMetaClassDeclareReservedUnused(IO80211Controller, 9);                 // 455
    OSMetaClassDeclareReservedUnused(IO80211Controller, 10);                // 456
    OSMetaClassDeclareReservedUnused(IO80211Controller, 11);                // 457
    OSMetaClassDeclareReservedUnused(IO80211Controller, 12);                // 458
    OSMetaClassDeclareReservedUnused(IO80211Controller, 13);                // 459
    OSMetaClassDeclareReservedUnused(IO80211Controller, 14);                // 460
    OSMetaClassDeclareReservedUnused(IO80211Controller, 15);                // 461

public:
    virtual IOReturn setMulticastList(ether_addr const *, UInt);            // 462

protected:
    // IOEthernetController is 0x118 in the target SDK. 15.7.4 registers
    // IO80211Controller with OSMetaClass at 0x128.
    uint8_t _io80211ControllerOpaque[0x10];
};

static_assert(sizeof(IO80211Controller) == 0x128,
              "IO80211Controller Sequoia ABI size mismatch");

#endif /* IO80211_SEQUOIA_H */
