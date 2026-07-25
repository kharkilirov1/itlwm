// IO80211Sequoia.h — v2: раскладка vtable восстановлена из IO80211Family 15.7.4 (24G517)
// Порядок виртуалов = ground truth из vtable-бинарника. НЕ править порядок!
// Цепочки: IO80211Controller : IOEthernetController
//          IO80211InfraInterface/IO80211VirtualInterface : IO80211SkywalkInterface
//            : IOSkywalkEthernetInterface : IOSkywalkNetworkInterface : IOSkywalkInterface : IOService
#ifndef IO80211_SEQUOIA_SKYWALK_H
#define IO80211_SEQUOIA_SKYWALK_H

#include <IOKit/IOService.h>
#include <IOKit/network/IOEthernetController.h>
#include <net/if.h>
#include <net/if_var.h>
#include <net/ethernet.h>
#include <sys/mbuf.h>
#include <Airport/apple80211_var.h>
#include <Airport/apple80211_ioctl.h>
#include <Airport/apple_private_spi.h>

// Forward declarations (закрытые типы Sequoia — используются только по указателю)
class IO80211NetworkPacket;
class IO80211Peer;
class IO80211PeerManager;
class IO80211ScanManager;
class IO80211Controller;
class IOSkywalkLogicalLink;
class IOSkywalkPacketBufferPool;
class IOSkywalkPacketQueue;
class IOSkywalkTSOOptions;
class PacketSkywalkScratch;
class IO80211FlowQueueHash;
class TxCompletionEnqueueStats;
class TxSubmissionDequeueStats;
typedef int apple80211_wme_ac;
struct nicproxy_info_s;
struct nicproxy_limits_info_s;
struct if_link_status;
typedef int SkywalkInterfaceRole;
struct userPrintCtx;
struct apple80211_stat_report;
struct apple80211_cca_report;
struct apple80211_frame_counters;
struct apple80211_chip_counters_tx;
struct apple80211_chip_counters_rx;
struct apple80211_chip_error_counters_tx;
struct apple80211_interface_availability;
struct apple80211_infra_specific_stats;
struct apple80211_ManagementInformationBasedot11_counters;


class IOSkywalkInterface : public IOService {
    OSDeclareAbstractStructors(IOSkywalkInterface)

public:
    virtual IOReturn enable(UInt);
    virtual IOReturn disable(UInt);
    virtual IOReturn clientConnectWithTask(task_t task, IOService * forClient, IOOptionBits options);
    virtual void clientDisconnect(IOService * forClient, IOOptionBits options);
    virtual bool isTerminating(void);
    OSMetaClassDeclareReservedUnused(IOSkywalkInterface,  0);
    OSMetaClassDeclareReservedUnused(IOSkywalkInterface,  1);
    OSMetaClassDeclareReservedUnused(IOSkywalkInterface,  2);
    OSMetaClassDeclareReservedUnused(IOSkywalkInterface,  3);
    OSMetaClassDeclareReservedUnused(IOSkywalkInterface,  4);
    OSMetaClassDeclareReservedUnused(IOSkywalkInterface,  5);
    OSMetaClassDeclareReservedUnused(IOSkywalkInterface,  6);
    OSMetaClassDeclareReservedUnused(IOSkywalkInterface,  7);
    OSMetaClassDeclareReservedUnused(IOSkywalkInterface,  8);
    OSMetaClassDeclareReservedUnused(IOSkywalkInterface,  9);
    OSMetaClassDeclareReservedUnused(IOSkywalkInterface, 10);
};


class IOSkywalkNetworkInterface : public IOSkywalkInterface {
    OSDeclareAbstractStructors(IOSkywalkNetworkInterface)

public:
    struct RegistrationInfo {
        uint8_t pad[304];
    } __attribute__((packed));
    struct ExpansionData
    {
        RegistrationInfo *fRegistrationInfo;
        ifnet_t fBSDInterface;
    };
    ExpansionData *mExpansionData;

public:
    virtual IOReturn registerNetworkInterfaceWithLogicalLink(IOSkywalkNetworkInterface::RegistrationInfo const*,IOSkywalkLogicalLink *,IOSkywalkPacketBufferPool *,IOSkywalkPacketBufferPool *,UInt);
    virtual IOReturn deregisterLogicalLink(void);
    virtual SInt32 initBSDInterfaceParameters(ifnet_init_eparams *,sockaddr_dl **);
    virtual bool prepareBSDInterface(ifnet_t,UInt);
    virtual void finalizeBSDInterface(ifnet_t,UInt);
    virtual ifnet_t getBSDInterface(void);
    virtual void setBSDName(char const*);
    virtual const char * getBSDName(void);
    virtual IOReturn processBSDCommand(ifnet_t,UInt,void *);
    virtual IOReturn processInterfaceCommand(ifdrv *);
    virtual IOReturn interfaceAdvisoryEnable(bool);
    virtual SInt32 setInterfaceEnable(bool);
    virtual SInt32 setRunningState(bool);
    virtual IOReturn handleChosenMedia(UInt);
    virtual void * getSupportedMediaArray(UInt *,UInt *);
    virtual void * getPacketTapInfo(UInt *,UInt *);
    virtual UInt getUnsentDataByteCount(UInt *,UInt *,UInt);
    virtual UInt32 getSupportedWakeFlags(UInt *);
    virtual void enableNetworkWake(UInt);
    virtual void calculateRingSizeForQueue(IOSkywalkPacketQueue const*,UInt *);
    virtual UInt getMaxTransferUnit(void);
    virtual void setMaxTransferUnit(UInt);
    virtual UInt getMinPacketSize(void);
    virtual UInt getHardwareAssists(void);
    virtual void setHardwareAssists(UInt,UInt);
    virtual void * getInterfaceFamily(void);
    virtual void * getInterfaceSubFamily(void);
    virtual UInt getInitialMedia(void);
    virtual UInt getFeatureFlags(void);
    virtual UInt getTxDataOffset(void);
    virtual UInt captureInterfaceState(UInt);
    virtual void restoreInterfaceState(UInt);
    virtual void setMTU(UInt);
    virtual bool bpfTap(UInt,UInt);
    virtual const char * getBSDNamePrefix(void);
    virtual UInt getBSDUnitNumber(void);
    virtual const char * classNameOverride(void);
    virtual void deferBSDAttach(bool);
    virtual void reportDetailedLinkStatus(if_link_status const*);
    virtual UInt getTSOOptions(IOSkywalkTSOOptions *);
    OSMetaClassDeclareReservedUnused(IOSkywalkNetworkInterface,  0);
    OSMetaClassDeclareReservedUnused(IOSkywalkNetworkInterface,  1);
    OSMetaClassDeclareReservedUnused(IOSkywalkNetworkInterface,  2);
    OSMetaClassDeclareReservedUnused(IOSkywalkNetworkInterface,  3);
    OSMetaClassDeclareReservedUnused(IOSkywalkNetworkInterface,  4);
    OSMetaClassDeclareReservedUnused(IOSkywalkNetworkInterface,  5);
    OSMetaClassDeclareReservedUnused(IOSkywalkNetworkInterface,  6);
    OSMetaClassDeclareReservedUnused(IOSkywalkNetworkInterface,  7);
    OSMetaClassDeclareReservedUnused(IOSkywalkNetworkInterface,  8);
    OSMetaClassDeclareReservedUnused(IOSkywalkNetworkInterface,  9);
public:
    // non-virtual helpers (из бинарника)
    bool initRegistrationInfo(IOSkywalkNetworkInterface::RegistrationInfo*, unsigned int, unsigned long);
    void reportLinkStatus(unsigned int, unsigned int);
};


class IOSkywalkEthernetInterface : public IOSkywalkNetworkInterface {
    OSDeclareAbstractStructors(IOSkywalkEthernetInterface)

public:
    struct RegistrationInfo {
        uint8_t pad[304];
    } __attribute__((packed));
    struct ExpansionData2
    {
        RegistrationInfo *fRegistrationInfo;
        ifnet_t fBSDInterface;
    };
    ExpansionData2 *mExpansionData2;

public:
    virtual void getHardwareAddress(ether_addr *);
    virtual void setHardwareAddress(ether_addr *);
    virtual void setLinkLayerAddress(ether_addr *);
    virtual bool configureMulticastFilter(UInt,ether_addr const*,UInt);
    virtual bool setMulticastAddresses(ether_addr const*,UInt);
    virtual void setAllMulticastModeEnable(bool);
    virtual IOReturn setPromiscuousModeEnable(bool, UInt);
    virtual void reportNicProxyLimits(nicproxy_limits_info_s);
    virtual void hwConfigNicProxyData(nicproxy_info_s *);
    OSMetaClassDeclareReservedUnused(IOSkywalkEthernetInterface,  0);
    OSMetaClassDeclareReservedUnused(IOSkywalkEthernetInterface,  1);
    OSMetaClassDeclareReservedUnused(IOSkywalkEthernetInterface,  2);
    OSMetaClassDeclareReservedUnused(IOSkywalkEthernetInterface,  3);
    OSMetaClassDeclareReservedUnused(IOSkywalkEthernetInterface,  4);
    OSMetaClassDeclareReservedUnused(IOSkywalkEthernetInterface,  5);
    OSMetaClassDeclareReservedUnused(IOSkywalkEthernetInterface,  6);
    OSMetaClassDeclareReservedUnused(IOSkywalkEthernetInterface,  7);
    OSMetaClassDeclareReservedUnused(IOSkywalkEthernetInterface,  8);
    OSMetaClassDeclareReservedUnused(IOSkywalkEthernetInterface,  9);
    OSMetaClassDeclareReservedUnused(IOSkywalkEthernetInterface, 10);
public:
    bool initRegistrationInfo(IOSkywalkEthernetInterface::RegistrationInfo*, unsigned int, unsigned long);
};


class IO80211SkywalkInterface : public IOSkywalkEthernetInterface {
    OSDeclareAbstractStructors(IO80211SkywalkInterface)

public:
    virtual bool createPeerManager(void);
    virtual IOReturn /*RT?*/ createPeer(unsigned char const *,IO80211PeerManager *);
    virtual void postMessage(unsigned int,void *,unsigned long,bool);
    virtual IOReturn reportDataPathEvents(unsigned int,void *,unsigned long,bool);
    virtual IOReturn recordOutputPackets(TxSubmissionDequeueStats *,TxSubmissionDequeueStats *);
    virtual IOReturn recordOutputPacket(apple80211_wme_ac,int,int);
    virtual void logTxPacket(IO80211NetworkPacket *,PacketSkywalkScratch *,apple80211_wme_ac,bool);
    virtual void logTxCompletionPacket(IO80211NetworkPacket *,PacketSkywalkScratch *,unsigned char *,apple80211_wme_ac,int,unsigned int,bool,bool);
    virtual IOReturn recordCompletionPackets(TxCompletionEnqueueStats *,TxCompletionEnqueueStats *);
    virtual mbuf_flags_t inputPacket(IO80211NetworkPacket *,packet_info_tag *,ether_header *,bool *,bool);
    virtual IOReturn forwardInfraRelayPackets(IO80211NetworkPacket *,ether_header *);
    virtual void logSkywalkTxReqPacket(IO80211NetworkPacket *,PacketSkywalkScratch *,unsigned char *,apple80211_wme_ac,bool);
    virtual SInt64 pendingPackets(unsigned char);
    virtual SInt64 packetSpace(unsigned char);
    virtual bool isChipInterfaceReady(void);
    virtual bool isDebounceOnGoing(void);
    virtual bool setLinkState(IO80211LinkState,unsigned int,bool,unsigned int);
    virtual IO80211LinkState linkState(void);
    virtual void setScanningState(unsigned int,bool,apple80211_scan_data *,int);
    virtual void setDataPathState(bool);
    virtual IOReturn /*RT?*/ getScanManager(void);
    virtual IOReturn /*RT?*/ getController(void);
    virtual void updateLinkParameters(apple80211_interface_availability *);
    virtual void updateInterfaceCoexRiskPct(unsigned long long);
    virtual void setLQM(unsigned long long);
    virtual void updateLinkStatus(void);
    virtual void updateLinkStatusGated(void);
    virtual void setInterfaceExtendedCCA(apple80211_channel,apple80211_cca_report *);
    virtual void setInterfaceCCA(apple80211_channel,int);
    virtual void setInterfaceNF(apple80211_channel,long long);
    virtual void setInterfaceOFDMDesense(apple80211_channel,long long);
    virtual void removePacketQueue(IO80211FlowQueueHash const *);
    virtual void setDebugFlags(unsigned long long,unsigned int);
    virtual SInt64 debugFlags(void);
    virtual void setInterfaceChipCounters(apple80211_stat_report *,apple80211_chip_counters_tx *,apple80211_chip_error_counters_tx *,apple80211_chip_counters_rx *);
    virtual void setInterfaceMIBdot11(apple80211_stat_report *,apple80211_ManagementInformationBasedot11_counters *);
    virtual void setFrameStats(apple80211_stat_report *,apple80211_frame_counters *);
    virtual void setInfraSpecificFrameStats(apple80211_stat_report *,apple80211_infra_specific_stats *);
    virtual SInt64 getWmeTxCounters(unsigned long long *);
    virtual void setPeerManagerLogFlag(unsigned int,unsigned int,unsigned int);
    virtual void setWoWEnabled(bool);
    virtual bool wowEnabled(void);
    virtual void printDataPath(userPrintCtx *);
    virtual UInt32 getDataQueueDepth(void);
    virtual bool findOrCreateFlowQueue(IO80211FlowQueueHash);
    virtual UInt64 findOrCreateFlowQueueWithCache(IO80211FlowQueueHash,bool *);
public:
    // non-virtual (из бинарника 15.7.4)
    IOReturn setInterfaceRole(unsigned int);
    IOReturn setInterfaceId(unsigned int);
    SkywalkInterfaceRole getInterfaceRole(void);
    IOReturn attachPeer(ether_addr *);
    void postMessageIOUC(char const *,unsigned int,void *,unsigned long);
};

#endif /* IO80211_SEQUOIA_SKYWALK_H */
