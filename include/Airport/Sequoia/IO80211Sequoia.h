// IO80211Sequoia.h — иерархия классов IO80211Family 15.7.4 (24G517)
// Восстановлено из KDK-символов. Методы + наследование + return types (где известны).
#ifndef IO80211_SEQUOIA_H
#define IO80211_SEQUOIA_H

#include <IOKit/network/IOEthernetController.h>
#include <net/if.h>
#include <net/if_var.h>
#include <net/ethernet.h>
#include <sys/mbuf.h>
#include <Airport/apple80211_var.h>
#include <Airport/apple80211_ioctl.h>
#include <Airport/apple_private_spi.h>
#include <Airport/IOSkywalkEthernetInterface.h>

class CCLogStream;
class CCPipe;
class CreatePostOffice;
class FamilyLqmLogId;
class FlowIdMetadata;
class GetProvider;
class IO80211APIUserClient;
class IO80211AssociationJoinSnapshot;
class IO80211Buffer;
class IO80211CagedBuffer;
class IO80211CountryCodeOp;
class IO80211FeatureCode;
class IO80211FlowQueue;
class IO80211FlowQueueHash;
class IO80211FlowQueueLegacy;
class IO80211InterfaceAVCAdvisory;
class IO80211InterfacePostMessage;
class IO80211NetworkPacket;
class IO80211Peer;
class IO80211PeerManager;
class IO80211TimerSource;
class IO80211VirtualInterface;
class IOReporter;
class IOSkywalkPacketBufferPool;
class IOSkywalkPacketQueue;
class OSSymbol;
class PacketSkywalkScratch;
class RegistrationInfo;
class TxCompletionEnqueueStats;
class TxPacketRequest;
class TxSubmissionDequeueStats;
class UpdateULLADuration;

// Типы из закрытых SDK-хедеров Sequoia (нет в открытых дампах) — только fwd decl
typedef int SkywalkInterfaceRole;
struct userPrintCtx;
struct apple80211_arp_keepalive_data;
struct apple80211_awdl_low_latency_statistics_reduced;
struct apple80211_channel_band;
struct apple80211_channel_width;
struct apple80211_data_path_interface_stats;
struct apple80211_data_path_peer_stats;
struct apple80211_infra_peer_address_data;
struct apple80211_infra_scan_start_event_data;
struct apple80211_ior_legend_query;
struct apple80211_ipv4_params;
struct apple80211_ipv6_params;
struct apple80211_latency;
struct apple80211_latency_all_ac;
struct apple80211_le_scan_params;
struct apple80211_magic_pattern_data;
struct apple80211_nan_low_latency_peer_statistics;
struct apple80211_p2p_thread_coex_event;
struct apple80211_packet_filter_data;
struct apple80211_platform_config;
struct apple80211_protocol_offload_data;
struct apple80211_system_state;
struct apple80211_tcp_udp_keepalive_data;
struct apple80211_wme_ac;
struct apple80211_stat_report;
struct apple80211_set_mac_address;
struct apple80211_cca_report;
struct apple80211_frame_counters;
struct apple80211_chip_counters_tx;
struct apple80211_chip_counters_rx;
struct apple80211_chip_error_counters_tx;
struct apple80211_interface_availability;
struct apple80211_infra_specific_stats;
struct apple80211_ManagementInformationBasedot11_counters;

class IO80211SkywalkInterface : public IOSkywalkEthernetInterface {
public:
    virtual IOReturn getMetaClass(void); // RT?
    virtual IOReturn initIvars(void); // RT?
    virtual IOReturn postCAEventForIOCTLLegacyUsage(int,unsigned long long); // RT?
    virtual bool init(void);
    virtual bool init(IOService *,ether_addr *);
    virtual bool start(IOService *);
    virtual IOReturn getInterfaceRoleCounter(SkywalkInterfaceRole); // RT?
    virtual IOReturn getInterfaceRole(void); // RT?
    virtual IOReturn getInterfaceRoleStr(void); // RT?
    virtual IOReturn logDebug(unsigned long long,char const *,...); // RT?
    virtual IOReturn getPeerManager(void); // RT?
    virtual IOReturn getInterfaceId(void); // RT?
    virtual const char * getBSDName(void);
    virtual IOReturn logDebug(char const *,...); // RT?
    virtual IOReturn setInterfaceRole(unsigned int); // RT?
    virtual void free(void);
    virtual SInt32 initBSDInterfaceParameters(ifnet_init_eparams *,sockaddr_dl * *);
    virtual bool prepareBSDInterface(__ifnet *,unsigned int);
    virtual IOReturn createIOReporters(void); // RT?
    virtual IOReturn validateDispatchQueue(void); // RT?
    virtual ifnet_t getIfnet(void);
    virtual IOReturn getRingMD(IO80211APIUserClient *,unsigned long long); // RT?
    virtual bool isIOUCPipeOpened(void);
    virtual void postMessageIOUC(char const *,unsigned int,void *,unsigned long);
    virtual IOReturn newUserClient(task *,void *,unsigned int,OSDictionary *,IOUserClient * *);
    virtual int getEventPipeSize(void);
    virtual UInt64 createEventPipe(IO80211APIUserClient *);
    virtual void destroyEventPipe(IO80211APIUserClient *);
    virtual IOReturn setUserBufferInfo(IOMemoryDescriptor *,unsigned long long); // RT?
    virtual IOReturn processBSDCommandGated(unsigned int,void *); // RT?
    virtual IOReturn performGatedCommandIOUC(unsigned int,void *); // RT?
    virtual IOReturn processBSDCommand(__ifnet *,unsigned int,void *);
    virtual IOReturn performGatedCommandIOCTL(unsigned int,void *); // RT?
    virtual IOReturn storeProcessNameAndIoctlInformation(unsigned long); // RT?
    virtual IOReturn isCommandAllowed(void); // RT?
    virtual bool createPeerManager(void);
    virtual IOReturn createPeer(unsigned char const *,IO80211PeerManager *); // RT?
    virtual IOReturn storeIoctlInArray(OSArray *,OSNumber *); // RT?
    virtual IOReturn dumpUnentitledProcesses(void); // RT?
    virtual IOReturn logDebugHex(void const *,unsigned long,char const *,...); // RT?
    virtual bool shouldLog(unsigned long long);
    virtual void handleDebugCmd(apple80211_debug_command *);
    virtual void printDataPath(userPrintCtx *);
    virtual UInt32 getDataQueueDepth(void);
    virtual const char * stringFromReturn(int);
    virtual IOReturn getControllerWorkQueue(void); // RT?
    virtual IOReturn getWorkQueue(void); // RT?
    virtual IOReturn getPeerMonitor(IO80211Peer *); // RT?
    virtual IOReturn getLinkQualityMonitor(IO80211Peer *); // RT?
    virtual IOReturn getLinkRecovery(IO80211Peer *); // RT?
    virtual IOReturn getLqmCrashTracer(IO80211Peer *); // RT?
    virtual void getHardwareAddress(ether_addr *);
    virtual IOReturn getSelfMacAddr(void); // RT?
    virtual IOReturn setInitMacAddress(ether_addr &); // RT?
    virtual void setHardwareAddress(ether_addr *);
    virtual IOReturn setSET_MAC_ADDRESS(apple80211_set_mac_address *);
    virtual IOReturn updateTimeSyncMacAddress(ether_addr &); // RT?
    virtual IOMediumType getMediumType(void);
    virtual IOReturn handleSIOCSIFADDR(void);
    virtual bool isDebounceOnGoing(void);
    virtual bool setLinkState(IO80211LinkState,unsigned int,bool,unsigned int);
    virtual SInt32 setRunningState(bool);
    virtual void postMessage(unsigned int,void *,unsigned long,bool);
    virtual IOReturn postMessageInternal(unsigned int,void *,unsigned long,bool); // RT?
    virtual IOReturn setPidLock(bool); // RT?
    virtual IOReturn pidLocked(void); // RT?
    virtual IOReturn setPowerState(unsigned long,IOService *);
    virtual IOReturn getPowerState(void); // RT?
    virtual void stop(IOService *);
    virtual unsigned long initialPowerStateForDomainState(unsigned long);
    virtual unsigned long maxCapabilityForDomainState(unsigned long);
    virtual IOReturn getP2PSkywalkPeerMgr(void); // RT?
    virtual IOReturn postMessageSync(unsigned int,void *,unsigned long); // RT?
    virtual IOReturn reportDataPathEventsGated(void *,void *,void *,void *,void *); // RT?
    virtual IOReturn reportDataPathEvents(unsigned int,void *,unsigned long,bool);
    virtual void * getSupportedMediaArray(unsigned int *,unsigned int *); // RT?
    virtual IOReturn handleChosenMedia(unsigned int);
    virtual IOReturn setPromiscuousModeEnable(bool,unsigned int);
    virtual IOReturn recordInputPacket(int,int); // RT?
    virtual IOReturn getInterfaceMonitor(void); // RT?
    virtual IOReturn recordOutputPacket(apple80211_wme_ac,int,int);
    virtual IOReturn recordOutputPackets(TxSubmissionDequeueStats *,TxSubmissionDequeueStats *);
    virtual mbuf_flags_t inputPacket(IO80211NetworkPacket *,packet_info_tag *,ether_header *,bool *,bool);
    virtual void logTxPacket(IO80211NetworkPacket *,PacketSkywalkScratch *,apple80211_wme_ac,bool);
    virtual void logTxCompletionPacket(IO80211NetworkPacket *,PacketSkywalkScratch *,unsigned char *,apple80211_wme_ac,int,unsigned int,bool,bool);
    virtual IOReturn recordCompletionPackets(TxCompletionEnqueueStats *,TxCompletionEnqueueStats *);
    virtual IOReturn forwardInfraRelayPackets(IO80211NetworkPacket *,ether_header *);
    virtual void logSkywalkTxReqPacket(IO80211NetworkPacket *,PacketSkywalkScratch *,unsigned char *,apple80211_wme_ac,bool);
    virtual bool findOrCreateFlowQueue(IO80211FlowQueueHash);
    virtual UInt64 findOrCreateFlowQueueWithCache(IO80211FlowQueueHash,bool *);
    virtual UInt64 findExistingFlowQueue(IO80211FlowQueueHash);
    virtual void flushPacketQueues(void);
    virtual void cachePeer(ether_addr *,unsigned int *);
    virtual void removePacketQueue(IO80211FlowQueueHash const *);
    virtual SInt64 pendingPackets(unsigned char);
    virtual SInt64 packetSpace(unsigned char);
    virtual IOReturn setInterfaceId(unsigned int); // RT?
    virtual IOReturn getCompanionInterfaceId(void); // RT?
    virtual IOReturn setCompanionInterfaceId(unsigned int); // RT?
    virtual bool isChipInterfaceReady(void);
    virtual void setScanningState(unsigned int,bool,apple80211_scan_data *,int);
    virtual IOReturn configureReport(IOReportChannelList *,unsigned int,void *,void *);
    virtual IOReturn updateReport(IOReportChannelList *,unsigned int,void *,void *);
    virtual void setDataPathState(bool);
    virtual IOReturn getController(void); // RT?
    virtual void updateLinkParameters(apple80211_interface_availability *);
    virtual void updateInterfaceCoexRiskPct(unsigned long long);
    virtual void setLQM(unsigned long long);
    virtual void updateLinkStatus(void);
    virtual void updateLinkStatusGated(void);
    virtual void setInterfaceExtendedCCA(apple80211_channel,apple80211_cca_report *);
    virtual void setInterfaceCCA(apple80211_channel,int);
    virtual void removePacketQueue(IO80211FlowQueueHash *);
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
    virtual UInt64 createLinkQualityMonitor(IO80211Peer *,IOService *);
    virtual void releaseLinkQualityMonitor(IO80211Peer *);
    virtual IOReturn getPropertyTable(void); // RT?
    virtual IOReturn getWorkerMatchingDict(OSString *); // RT?
    virtual bool isInterfaceEnabled(void);
    virtual IOReturn getPacketPool(OSString *); // RT?
    virtual IOReturn getLogger(void); // RT?
    virtual IOReturn debugHandler(apple80211_debug_command *);
    virtual IOReturn getDeviceType(void); // RT?
    virtual IOReturn setDeviceType(unsigned int); // RT?
    virtual IOReturn setParentInterface(IO80211VirtualInterface *); // RT?
    virtual IOReturn getParentInterface(void); // RT?
    virtual IOReturn setLowLatencyEnabled(bool); // RT?
    virtual IOReturn isLowLatencyEnabled(void); // RT?
    virtual IOReturn pidLockPid(void); // RT?
    virtual IOReturn getInterfaceSnapshots(void); // RT?
    virtual IOReturn setInterfaceSnapshots(IO80211AssociationJoinSnapshot *); // RT?
    virtual IO80211LinkState linkState(void);
    virtual void setInterfaceNF(apple80211_channel,long long);
    virtual void setInterfaceOFDMDesense(apple80211_channel,long long);
    virtual void setNotificationProperty(OSSymbol const *,OSObject const *);
    virtual IOReturn getLQMSummary(apple80211_lqm_summary *); // RT?
    virtual IOReturn getScanManager(void); // RT?
    virtual void statsDump(void);
    virtual void powerOnNotification(void);
    virtual void powerOffNotification(void);
    virtual UInt64 getTxQueueDepth(void);
    virtual UInt64 getRxQueueCapacity(void);
    virtual void updateRxCounter(unsigned long long);
    virtual IOReturn getMultiCastQueue(void); // RT?
    virtual int getAssocState(void);
    virtual void notifyQueueState(apple80211_wme_ac,unsigned short);
    virtual int getTxHeadroom(void);
    virtual IOReturn getRxCompQueue(void); // RT?
    virtual IOReturn getTxCompQueue(void); // RT?
    virtual IOReturn getTxSubQueue(apple80211_wme_ac); // RT?
    virtual IOReturn getTxPacketPool(void); // RT?
    virtual IOReturn getRxPacketPool(void); // RT?
    virtual void enableDatapath(void);
    virtual void disableDatapath(void);
    virtual int getNumTxQueues(void);
    virtual IOReturn enable(unsigned int);
    virtual IOReturn disable(unsigned int);
    virtual IOReturn classNameOverride(void); // RT?
    virtual int errnoFromReturn(int);
    virtual SInt32 setInterfaceEnable(bool);
    virtual UInt32 getFeatureFlags(void);
    virtual bool isCommandProhibited(int);
    virtual IOReturn routeIoctlToWcl(unsigned int,unsigned int,void *,unsigned long); // RT?
    virtual IOReturn attachPeer(ether_addr *); // RT?
    virtual IOReturn detachPeer(ether_addr *); // RT?
    virtual IOReturn setDebugTrafficReport(bool); // RT?
    virtual IOReturn getMacAddressAgent(void); // RT?
    virtual IOReturn getDataPathInterfaceStats(apple80211_data_path_interface_stats *); // RT?
    virtual IOReturn getDataPathPeerStats(apple80211_data_path_peer_stats *); // RT?
    virtual IOReturn getLastQueuePacketTime(ether_addr *); // RT?
    virtual IOReturn getLastRxUnicastLinkActivityTime(ether_addr *); // RT?
    virtual IOReturn updateInterfaceDataStats(apple80211_data_path_interface_stats *); // RT?
    virtual IOReturn updatePeerDataStats(apple80211_data_path_peer_stats *); // RT?
    virtual IOReturn logTxLatency(unsigned char *,unsigned int,unsigned long long); // RT?
    virtual IOReturn findPeer(ether_addr &); // RT?
    virtual IOReturn logRxLatency(unsigned int,unsigned long long); // RT?
    virtual IOReturn getNClearTxRxLatency(apple80211_latency_all_ac *,apple80211_latency_all_ac *); // RT?
    virtual IOReturn getLastTxTimeStamp(unsigned long long &); // RT?
    virtual IOReturn getLastRxTimeStamp(unsigned long long &); // RT?
};

class IO80211InfraInterface : public IO80211SkywalkInterface {
public:
    virtual IOReturn getMetaClass(void); // RT?
    virtual bool start(IOService *);
    virtual IOReturn triggerLinkStatusUpdate(IO80211TimerSource *); // RT?
    virtual IOReturn reportDataTransferRatesTimer(IO80211TimerSource *); // RT?
    virtual IOReturn publishOffloadCapability(void); // RT?
    virtual void stop(IOService *);
    virtual bool init(void);
    virtual void free(void);
    virtual IOReturn createLQMData(void); // RT?
    virtual SInt32 initBSDInterfaceParameters(ifnet_init_eparams *,sockaddr_dl * *);
    virtual bool prepareBSDInterface(__ifnet *,unsigned int);
    virtual IOReturn updateStaticProperties(void); // RT?
    virtual IOReturn processBSDCommand(__ifnet *,unsigned int,void *);
    virtual IOReturn updateMediumStatus(ifmediareq *); // RT?
    virtual bool isDebounceOnGoing(void);
    virtual bool setLinkState(IO80211LinkState,unsigned int,bool,unsigned int);
    virtual IOReturn setLinkStateInternal(IO80211LinkState,unsigned int,bool,unsigned int);
    virtual IOReturn handleKeyDone(bool,bool); // RT?
    virtual IOReturn updateLinkSpeed(void); // RT?
    virtual IOReturn cancelDebounceTimer(void); // RT?
    virtual IO80211LinkState linkState(void);
    virtual void postMessage(unsigned int,void *,unsigned long,bool);
    virtual IOReturn getInfraPeer(void); // RT?
    virtual IOReturn bssidChange(void *,unsigned long); // RT?
    virtual IOReturn UpdateULLADuration(unsigned long long *); // RT?
    virtual IOReturn finishSIBCoexTimer(void); // RT?
    virtual IOReturn getAVCAdvisory(void); // RT?
    virtual IOReturn updateChanOutageTime(apple80211_infra_scan_start_event_data *); // RT?
    virtual IOReturn loadHwChannels(void); // RT?
    virtual IOReturn loadChannelInfo(void); // RT?
    virtual IOReturn resetInterface(void *,unsigned long); // RT?
    virtual IOReturn updateSSIDProperty(void); // RT?
    virtual IOReturn updateCountryCodeProperty(bool); // RT?
    virtual IOReturn routeToP2PInterface(unsigned int,void *,unsigned long); // RT?
    virtual IOReturn updateBSSIDProperty(ether_addr &,apple80211_channel &,bool); // RT?
    virtual IOReturn updateChannelProperty(apple80211_channel &); // RT?
    virtual IOReturn updateLocaleProperty(void); // RT?
    virtual void setWoWEnabled(bool);
    virtual void setScanningState(unsigned int,bool,apple80211_scan_data *,int);
    virtual IOReturn configureReport(IOReportChannelList *,unsigned int,void *,void *);
    virtual IOReturn updateReport(IOReportChannelList *,unsigned int,void *,void *);
    virtual IOReturn handleLeakyApStatsResetTimer(IO80211TimerSource *); // RT?
    virtual IOReturn getLeakyApStats(apple80211_leaky_ap_stats const * *); // RT?
    virtual IOReturn resetLeakyApStats(void); // RT?
    virtual IOReturn setLeakyApSsidMetrics(apple80211_leaky_ap_ssid_metrics *); // RT?
    virtual IOReturn setLeakyAPStats(apple80211_leaky_ap_event *); // RT?
    virtual IOReturn setLeakyAPStatsMode(unsigned int); // RT?
    virtual IOReturn recordOutputPackets(TxSubmissionDequeueStats *,TxSubmissionDequeueStats *);
    virtual void logTxPacket(IO80211NetworkPacket *,PacketSkywalkScratch *,apple80211_wme_ac,bool);
    virtual IOReturn getAwdlMaxBandWidth(void); // RT?
    virtual IOReturn get5GLowHighBandSwitchCounter(void); // RT?
    virtual IOReturn get5GLowHighBandSwitchSuccessPerc(void); // RT?
    virtual IOReturn getULLAClassicDuration(void); // RT?
    virtual IOReturn getCoPTxRTSFailCount(void); // RT?
    virtual IOReturn getULLALiteDuration(void); // RT?
    virtual IOReturn resetCoPTxRTSFailCount(void); // RT?
    virtual IOReturn resetSIBTurnOnMetrics(void); // RT?
    virtual IOReturn getCoPSIBCoexTurnOnCount(void); // RT?
    virtual IOReturn getCoPSIBCoexTurnOnDuration(void); // RT?
    virtual IOReturn setGatewayAddress(apple80211_infra_peer_address_data); // RT?
    virtual IOReturn setUnicastInfraPeerAddress(apple80211_infra_peer_address_data); // RT?
    virtual IOReturn setInfraPeersLoggingEnabled(bool); // RT?
    virtual IOReturn recordCompletionPackets(TxCompletionEnqueueStats *,TxCompletionEnqueueStats *);
    virtual void logTxCompletionPacket(IO80211NetworkPacket *,PacketSkywalkScratch *,unsigned char *,apple80211_wme_ac,int,unsigned int,bool,bool);
    virtual mbuf_flags_t inputPacket(IO80211NetworkPacket *,packet_info_tag *,ether_header *,bool *,bool);
    virtual void updateLinkParameters(apple80211_interface_availability *);
    virtual IOReturn updateLinkParametersStatic(void *,void *); // RT?
    virtual IOReturn updateLinkParametersGated(apple80211_interface_availability *); // RT?
    virtual void updateInterfaceCoexRiskPct(unsigned long long);
    virtual IOReturn reportDataTransferRates(void); // RT?
    virtual IOReturn setBTCoexWLANLostAntennaTime(unsigned long long,unsigned long long,bool,apple80211_btCoex_report *); // RT?
    virtual IOReturn reportDataTransferRatesStatic(void *); // RT?
    virtual IOReturn getLQMData(void); // RT?
    virtual void setLQM(unsigned long long);
    virtual IOReturn setLQMStatic(void *,void *); // RT?
    virtual IOReturn setLQMGated(unsigned long long); // RT?
    virtual void updateLinkStatus(void);
    virtual IOReturn updateLinkStatusStatic(void *); // RT?
    virtual void updateLinkStatusGated(void);
    virtual void setInterfaceExtendedCCA(apple80211_channel,apple80211_cca_report *);
    virtual void setInterfaceCCA(apple80211_channel,int);
    virtual void setInterfaceNF(apple80211_channel,long long);
    virtual void setInterfaceOFDMDesense(apple80211_channel,long long);
    virtual IOReturn setAMPDUstat(apple80211_stat_report *,apple80211_ampdu_stat_report *); // RT?
    virtual void setDataPathState(bool);
    virtual void setDebugFlags(unsigned long long,unsigned int);
    virtual void setPeerManagerLogFlag(unsigned int,unsigned int,unsigned int);
    virtual SInt64 debugFlags(void);
    virtual void setInterfaceChipCounters(apple80211_stat_report *,apple80211_chip_counters_tx *,apple80211_chip_error_counters_tx *,apple80211_chip_counters_rx *);
    virtual void setInterfaceMIBdot11(apple80211_stat_report *,apple80211_ManagementInformationBasedot11_counters *);
    virtual void setFrameStats(apple80211_stat_report *,apple80211_frame_counters *);
    virtual void setInfraSpecificFrameStats(apple80211_stat_report *,apple80211_infra_specific_stats *);
    virtual SInt64 getWmeTxCounters(unsigned long long *);
    virtual IOReturn resetTxPathHealthCheck(void); // RT?
    virtual UInt64 createLinkQualityMonitor(IO80211Peer *,IOService *);
    virtual IOReturn getInfraLinkProperties(IO80211Peer *,unsigned int &,unsigned int &,apple80211_channel &,apple80211_channel_width &,apple80211_phymode &,apple80211_channel_band &); // RT?
    virtual void releaseLinkQualityMonitor(IO80211Peer *);
    virtual IOReturn notifyAWDLStateChange(bool); // RT?
    virtual int bpfOutputPacket(__mbuf *,void *);
    virtual IOReturn bpfOutput(unsigned int,__mbuf *); // RT?
    virtual bool bpfTap(unsigned int,unsigned int);
    virtual IOReturn bpfTapInternal(unsigned int,unsigned int); // RT?
    virtual IOReturn getWMMBWReset(void); // RT?
    virtual IOReturn setWMMBWReset(bool); // RT?
    virtual bool wowEnabled(void);
    virtual IOReturn getScanManager(void); // RT?
    virtual IOReturn getMonitorMode(void); // RT?
    virtual SInt64 pendingPackets(unsigned char);
    virtual SInt64 packetSpace(unsigned char);
    virtual IOReturn getLQMSummary(apple80211_lqm_summary *); // RT?
    virtual IOReturn getTrafficMonitor(void); // RT?
    virtual UInt getHardwareAssists(void);
    virtual int getAssocState(void);
    virtual IOReturn setCurrentApAddress(ether_addr *); // RT?
    virtual IOReturn getCurrentApAddress(void); // RT?
    virtual IOReturn setLQMConfig(apple80211_lqm_config_t *); // RT?
    virtual SInt32 setInterfaceEnable(bool);
    virtual IOReturn onDispatchQueue(void); // RT?
    virtual void setWCL_ADVISORTY_INFO(apple80211_wcl_advisory_info *);
    virtual IOReturn getWCL_TX_RX_LATENCY(apple80211_wcl_tx_rx_latency *); // RT?
    virtual IOReturn updateTxRxLatency(void); // RT?
    virtual IOReturn setIPv4Params(apple80211_ipv4_params *); // RT?
    virtual IOReturn getIPv4Params(void); // RT?
    virtual IOReturn setIPv6Params(apple80211_ipv6_params *); // RT?
    virtual IOReturn getIPv6Params(void); // RT?
    virtual IOReturn getPacketFilterData(apple80211_packet_filter_data &); // RT?
    virtual IOReturn getTcpUdpKeepAliveData(apple80211_tcp_udp_keepalive_data &); // RT?
    virtual IOReturn bonjourOffloadNumActivePorts(unsigned int &); // RT?
    virtual IOReturn getTcpKeepAliveConfigData(void); // RT?
    virtual IOReturn getBonjourOffloadConfigData(unsigned short &,unsigned int &); // RT?
    virtual IOReturn getArpKeepAliveData(apple80211_arp_keepalive_data &,bool); // RT?
    virtual IOReturn getProtocolOffloadData(apple80211_protocol_offload_data &); // RT?
    virtual IOReturn getMagicPatternData(apple80211_magic_pattern_data &); // RT?
    virtual IOReturn getSTATS(apple80211_stats_data *);
    virtual IOReturn getBtCoexState(void); // RT?
    virtual IOReturn getBT_COEX_FLAGS(apple80211_state_data *);
    virtual IOReturn setBT_COEX_FLAGS(apple80211_state_data *);
    virtual IOReturn setPowerState(unsigned long,IOService *);
    virtual IOReturn setWCLSystemState(apple80211_system_state &); // RT?
    virtual void hwConfigNicProxyData(nicproxy_info_s *);
    virtual IOReturn setLeScanParams(apple80211_le_scan_params &); // RT?
    virtual IOReturn getHwChannels(apple80211_sup_channel_data &); // RT?
    virtual IOReturn registerInfraEthernetInterface(IOSkywalkEthernetInterface::RegistrationInfo *,IOSkywalkPacketQueue * *,unsigned int,IOSkywalkPacketBufferPool *,IOSkywalkPacketBufferPool *); // RT?
    virtual IOReturn getLOCALE(apple80211_locale_data *);
};

class IO80211VirtualInterface : public IO80211SkywalkInterface {
public:
    virtual IOReturn getMetaClass(void); // RT?
    virtual IOReturn _RESERVEDIO80211VirtualInterface0(void); // RT?
    virtual IOReturn _RESERVEDIO80211VirtualInterface1(void); // RT?
    virtual IOReturn _RESERVEDIO80211VirtualInterface2(void); // RT?
    virtual IOReturn _RESERVEDIO80211VirtualInterface3(void); // RT?
    virtual IOReturn _RESERVEDIO80211VirtualInterface4(void); // RT?
    virtual IOReturn _RESERVEDIO80211VirtualInterface5(void); // RT?
    virtual IOReturn _RESERVEDIO80211VirtualInterface6(void); // RT?
    virtual IOReturn _RESERVEDIO80211VirtualInterface7(void); // RT?
    virtual IOReturn _RESERVEDIO80211VirtualInterface8(void); // RT?
    virtual IOReturn _RESERVEDIO80211VirtualInterface9(void); // RT?
    virtual IOReturn _RESERVEDIO80211VirtualInterface10(void); // RT?
    virtual IOReturn _RESERVEDIO80211VirtualInterface11(void); // RT?
    virtual IOReturn _RESERVEDIO80211VirtualInterface12(void); // RT?
    virtual IOReturn _RESERVEDIO80211VirtualInterface13(void); // RT?
    virtual IOReturn _RESERVEDIO80211VirtualInterface14(void); // RT?
    virtual IOReturn _RESERVEDIO80211VirtualInterface15(void); // RT?
    virtual bool attach(IOService *);
    virtual void detach(IOService *);
    virtual IOReturn peerToPeerAttachToBpf(void); // RT?
    virtual IOReturn bpfAttachEN10MB(unsigned int); // RT?
    virtual bool attachToBpf(void);
    virtual IOReturn isPeerToPeerInterface(void); // RT?
    virtual IOReturn freeBpfResources(void); // RT?
    virtual IOReturn allocBpfResources(void); // RT?
    virtual bool init(IO80211Controller *,ether_addr *,unsigned int,char const *);
    virtual IOReturn logDebug(char const *,...); // RT?
    virtual bool start(IOService *);
    virtual IOReturn enable(unsigned int);
    virtual bool createPeerManager(void);
    virtual IOReturn setInfraChannel(apple80211_channel *); // RT?
    virtual IOReturn handleChannelSwitchAnnouncement(apple80211_channel_switch_announcement *); // RT?
    virtual IOReturn setJoiningState(unsigned int,joinStatus,bool); // RT?
    virtual IOReturn isAwdlAssistedDiscoveryEnabled(void); // RT?
    virtual IOReturn notifyHostapState(apple80211_hostap_state *); // RT?
    virtual const char * stringFromReturn(int);
    virtual UInt32 configureAQMOutput(void);
    virtual void free(void);
    virtual bool terminate(unsigned int);
    virtual void stop(IOService *);
    virtual IOReturn configureBpfOutputQueues(bool); // RT?
    virtual int bpfOutputPacket(__mbuf *,void *);
    virtual bool bpfTap(unsigned int,unsigned int);
    virtual IOReturn bpfTapInput(__mbuf *,unsigned int,void *,unsigned long); // RT?
    virtual IOReturn getMgmtFramePool(void); // RT?
    virtual IOReturn bpfTapInput(IO80211Buffer *,unsigned int,void *,unsigned long); // RT?
    virtual IOReturn sendToBpfTap(IO80211Buffer *,unsigned int,void *,unsigned long); // RT?
    virtual IOReturn peerToPeerConfigureIfnet(void); // RT?
    virtual bool configureIfnet(void);
    virtual IOReturn dequeueTxPackets(TxPacketRequest *); // RT?
    virtual IOReturn storeProcessNameAndIoctlInformation(unsigned long); // RT?
    virtual IOReturn storeIoctlInArray(OSArray *,OSNumber *); // RT?
    virtual IOReturn logDebug(unsigned long long,char const *,...); // RT?
    virtual IOReturn dumpUnentitledProcesses(void); // RT?
    virtual IOReturn ioctl_internal_gated(void *,void *,void *,void *,void *); // RT?
    virtual IOReturn ioctl_internal(void *); // RT?
    virtual bool prepareBSDInterface(__ifnet *,unsigned int);
    virtual IOReturn processBSDCommand(__ifnet *,unsigned int,void *);
    virtual UInt32 getFeatureFlags(void);
    virtual SInt32 setInterfaceEnable(bool);
    virtual bool setMulticastAddresses(ether_addr const *,unsigned int);
    virtual void handleIoctl(__ifnet *,unsigned long,void *);
    virtual IOReturn handleIoctlGated(void *,void *,void *,void *,void *); // RT?
    virtual IOReturn terminating(void); // RT?
    virtual void setScanningState(unsigned int,bool,apple80211_scan_data *,int);
    virtual IOReturn setInfraTxState(bool); // RT?
    virtual void updateLinkParameters(apple80211_interface_availability *);
    virtual void updateInterfaceCoexRiskPct(unsigned long long);
    virtual IOReturn postAwdlStatistics(apple80211_awdl_statistics *); // RT?
    virtual IOReturn IO80211InterfacePostMessage(unsigned int,void *,unsigned long); // RT?
    virtual IOReturn postSyncStateChanged(void); // RT?
    virtual SInt64 getWmeTxCounters(unsigned long long *);
    virtual void logSkywalkTxReqPacket(IO80211NetworkPacket *,PacketSkywalkScratch *,unsigned char *,apple80211_wme_ac,bool);
    virtual void flushPacketQueues(void);
    virtual void removePacketQueue(IO80211FlowQueueHash const *);
    virtual IOReturn getPacketQueueDatabase(void); // RT?
    virtual SInt64 pendingPackets(unsigned char);
    virtual IOReturn queueSize(unsigned char); // RT?
    virtual SInt64 packetSpace(unsigned char);
    virtual UInt64 findExistingFlowQueue(IO80211FlowQueueHash);
    virtual void cachePeer(ether_addr *,unsigned int *);
    virtual IOReturn isPeerManagerDataDisabled(void); // RT?
    virtual bool findOrCreateFlowQueue(IO80211FlowQueueHash);
    virtual UInt64 findOrCreateFlowQueueWithCache(IO80211FlowQueueHash,bool *);
    virtual IOReturn dropTxPacket(__mbuf *); // RT?
    virtual IOReturn reportTransmitCompletionStatus(__mbuf *,int,unsigned int,unsigned int,unsigned int); // RT?
    virtual bool isOutputFlowControlled(void);
    virtual void setOutputFlowControlled(void);
    virtual void clearOutputFlowControlled(void);
    virtual IOReturn forwardPacket(IO80211NetworkPacket *); // RT?
    virtual IOReturn resetUserClientReference(void); // RT?
    virtual IOReturn resetUserClientReferenceGated(OSObject *,void *,void *,void *,void *); // RT?
    virtual void postMessage(unsigned int,void *,unsigned long,bool);
    virtual void setDebugFlags(unsigned long long,unsigned int);
    virtual SInt64 debugFlags(void);
    virtual void setWoWEnabled(bool);
    virtual IO80211LinkState linkState(void);
    virtual bool setLinkState(IO80211LinkState,unsigned int);
    virtual void setInterfaceExtendedCCA(apple80211_channel,apple80211_cca_report *,apple80211_awdl_sync_channel_sequence *);
    virtual void setInterfaceCCA(apple80211_channel,int,apple80211_awdl_sync_channel_sequence *);
    virtual void setInterfaceNF(apple80211_channel,long long);
    virtual void setInterfaceChipCounters(apple80211_stat_report *,apple80211_chip_counters_tx *,apple80211_chip_error_counters_tx *,apple80211_chip_counters_rx *);
    virtual void setInterfaceMIBdot11(apple80211_stat_report *,apple80211_ManagementInformationBasedot11_counters *);
    virtual void setFrameStats(apple80211_stat_report *,apple80211_frame_counters *);
    virtual void setInfraSpecificFrameStats(apple80211_stat_report *,apple80211_infra_specific_stats *);
    virtual IOReturn setAMPDUstat(apple80211_stat_report *,apple80211_ampdu_stat_report *); // RT?
    virtual IOReturn pushPacket(__mbuf *); // RT?
    virtual mbuf_flags_t inputPacket(IO80211NetworkPacket *,packet_info_tag *,ether_header *,bool *,bool);
    virtual void logTxPacket(IO80211NetworkPacket *,PacketSkywalkScratch *,apple80211_wme_ac,bool);
    virtual void logTxCompletionPacket(IO80211NetworkPacket *,PacketSkywalkScratch *,unsigned char *,apple80211_wme_ac,int,unsigned int,bool,bool);
    virtual IOReturn getOutputQueueForDLT(unsigned int); // RT?
    virtual void handleDebugCmd(apple80211_debug_command *);
    virtual void printDataPath(userPrintCtx *);
    virtual bool shouldLog(unsigned long long);
    virtual IOReturn configureReport(IOReportChannelList *,unsigned int,void *,void *);
    virtual IOReturn getCompanionLowLatencySkywalkInterface(void); // RT?
    virtual IOReturn updateReport(IOReportChannelList *,unsigned int,void *,void *);
    virtual IOReturn postAwdlHppStatsEvent(apple80211_awdl_low_latency_statistics_reduced *); // RT?
    virtual IOReturn postLowlatencyStatistics(apple80211_nan_low_latency_peer_statistics *); // RT?
    virtual IOReturn postP2PAirplayStatistics(apple80211_p2p_airplay_statistics *); // RT?
    virtual IOReturn postHostapChannelChanged(apple80211_hostap_state *); // RT?
    virtual IOReturn postP2pThreadCoexEvent(apple80211_p2p_thread_coex_event *); // RT?
    virtual IOReturn postHostAPBandUpdate(apple80211_channel_band); // RT?
    virtual IOReturn startAsyncEventUserClientForTask(task *,kIO80211InterfaceType); // RT?
    virtual int getEventPipeSize(void);
    virtual UInt64 createEventPipe(IO80211APIUserClient *);
    virtual IOReturn p2pDaemonExited(void); // RT?
    virtual IOReturn p2pDaemonExitedStatic(void *); // RT?
    virtual IOReturn p2pDaemonExitedGated(void); // RT?
    virtual IOReturn newUserClient(task *,void *,unsigned int,OSDictionary *,IOUserClient * *);
    virtual IOReturn addPeerToCache(unsigned char *); // RT?
    virtual IOReturn deletePeerFromCache(unsigned char *); // RT?
    virtual IOReturn reset(void); // RT?
    virtual IOReturn setDisplayState(bool); // RT?
    virtual IOReturn getP2PSkywalkPeerMgr(void); // RT?
    virtual IOReturn wmmSupported(void); // RT?
    virtual bool wowEnabled(void);
    virtual IOReturn getRoamingFlags(void); // RT?
    virtual IOReturn setRoamingFlags(unsigned int); // RT?
    virtual IOReturn setCompanionLowLatencySkywalkInterface(IO80211SkywalkInterface *); // RT?
    virtual IOReturn dupAndTransmitMcastPacket(IO80211NetworkPacket *,unsigned char *,unsigned char *); // RT?
    virtual IOReturn modifyUnicastPacket(unsigned char *,unsigned int,unsigned char,unsigned int *,unsigned char *); // RT?
    virtual IOReturn handleMulticastTransmit(IO80211NetworkPacket *,unsigned char *,unsigned int,unsigned char); // RT?
    virtual IOReturn freeMulticastPacket(IO80211NetworkPacket *); // RT?
    virtual IOReturn triggerAWDLMulticastTx(unsigned int,unsigned char); // RT?
    virtual IOReturn p2pMulticastTx(IO80211NetworkPacket * *,unsigned int); // RT?
    virtual IOReturn setInterfaceSuspended(void); // RT?
    virtual IOReturn clearInterfaceSuspended(void); // RT?
    virtual IOReturn isInterfaceSuspended(void); // RT?
    virtual IOReturn setPeerManagerDataDisabled(bool); // RT?
    virtual IOReturn getPeerManagerDataDisabled(void); // RT?
    virtual IOReturn stashMulticastPacketPair(IO80211NetworkPacket *,IO80211NetworkPacket *); // RT?
    virtual IOReturn sendPendingPacketsToStack(IO80211NetworkPacket * *,unsigned int); // RT?
    virtual IOReturn freePendingPackets(IO80211NetworkPacket * *,unsigned int); // RT?
    virtual IOReturn setAwdlCurrentChannelSequenceIndex(unsigned int); // RT?
    virtual IOReturn getPeerSlotDataStats(ether_addr *,bool,peerSlotDataStats *); // RT?
    virtual IOReturn getPeerDataStats(ether_addr *,bool,packetCounters_t *); // RT?
    virtual IOReturn clearPeerDataStats(ether_addr *,bool); // RT?
    virtual IOReturn findPeer(ether_addr &); // RT?
};

class IO80211Controller : public IOEthernetController {
public:
    virtual IOReturn getMetaClass(void); // RT?
    virtual IOReturn _RESERVEDIO80211Controller0(void); // RT?
    virtual IOReturn _RESERVEDIO80211Controller1(void); // RT?
    virtual IOReturn _RESERVEDIO80211Controller2(void); // RT?
    virtual IOReturn _RESERVEDIO80211Controller3(void); // RT?
    virtual IOReturn _RESERVEDIO80211Controller4(void); // RT?
    virtual IOReturn _RESERVEDIO80211Controller5(void); // RT?
    virtual IOReturn _RESERVEDIO80211Controller6(void); // RT?
    virtual IOReturn _RESERVEDIO80211Controller7(void); // RT?
    virtual IOReturn _RESERVEDIO80211Controller8(void); // RT?
    virtual IOReturn _RESERVEDIO80211Controller9(void); // RT?
    virtual IOReturn _RESERVEDIO80211Controller10(void); // RT?
    virtual IOReturn _RESERVEDIO80211Controller11(void); // RT?
    virtual IOReturn _RESERVEDIO80211Controller12(void); // RT?
    virtual IOReturn _RESERVEDIO80211Controller13(void); // RT?
    virtual IOReturn _RESERVEDIO80211Controller14(void); // RT?
    virtual IOReturn _RESERVEDIO80211Controller15(void); // RT?
    virtual bool start(IOService *);
    virtual IOReturn setGlobalLogger(CCLogStream *); // RT?
    virtual IOReturn createIOReporters(IOService *); // RT?
    virtual IOReturn setupControlPathLogging(void); // RT?
    virtual IOReturn findAndAttachToFaultReporter(void); // RT?
    virtual IOReturn logDebug(char const *,...); // RT?
    virtual IOReturn GetProvider(void); // RT?
    virtual IOReturn handleIOReporterLegendIsCalm(IO80211TimerSource *); // RT?
    virtual IOReturn allocFramePool(void); // RT?
    virtual IOReturn getReporterProvider(void); // RT?
    virtual IOReturn getTimerFactory(void); // RT?
    virtual bool init(OSDictionary *);
    virtual IOReturn debugStateInit(void); // RT?
    virtual void free(void);
    virtual void stop(IOService *);
    virtual IOReturn releaseIOReporters(void); // RT?
    virtual IOReturn refreshFamilyLQMLogs(FamilyLqmLogId,char *); // RT?
    virtual IOReturn getFamilyLQMLogs(apple80211_lqm_summary *); // RT?
    virtual IOReturn setInfraChannel(apple80211_channel *); // RT?
    virtual IOReturn getAwdlInterface(void); // RT?
    virtual IOReturn calculateInterfacesCoex(void); // RT?
    virtual IO80211FlowQueueLegacy requestFlowQueue(FlowIdMetadata const *);
    virtual void releaseFlowQueue(IO80211FlowQueue *);
    virtual IOReturn dispatchPacket(__mbuf *,unsigned int,char *,unsigned int); // RT?
    virtual IOReturn getDriverTextLog(void); // RT?
    virtual IOReturn isStarted(void); // RT?
    virtual IOReturn logDebug(unsigned long long,char const *,...); // RT?
    virtual IOReturn io80211isDebuggable(bool *); // RT?
    virtual IOReturn clearLogTargets(void); // RT?
    virtual IOReturn setLogTarget(IO80211VirtualInterface *,bool); // RT?
    virtual UInt32 selfDiagnosticsReport(int,char const *,unsigned int);
    virtual IOReturn getHtCapabilityLength(void); // RT?
    virtual IOReturn getHtCapability(void); // RT?
    virtual IOReturn setHtCapability(ieee80211_ht_capability_ie *); // RT?
    virtual IOReturn setCurrentChannel(apple80211_channel *); // RT?
    virtual IOReturn setPrimaryInterfaceDatapathState(bool); // RT?
    virtual IOReturn setChannelSequenceList(apple80211_awdl_sync_channel_sequence *); // RT?
    virtual IOReturn calculateInterfacesAvaiability(void); // RT?
    virtual void updateInterfaceCoexRiskPct(unsigned long long);
    virtual IOReturn printChannels(void); // RT?
    virtual IOReturn scanStarted(scanSource,apple80211_scan_data *); // RT?
    virtual IOReturn scanDone(scanSource,int); // RT?
    virtual IOReturn handleChannelSwitchAnnouncement(apple80211_channel_switch_announcement *); // RT?
    virtual IOReturn joinStarted(scanSource,joinStatus); // RT?
    virtual IOReturn joinDone(scanSource,joinStatus); // RT?
    virtual IOReturn getPid(void); // RT?
    virtual IOReturn getProcessName(char *,unsigned long); // RT?
    virtual IOReturn isAwdlAssistedDiscoveryEnabled(void); // RT?
    virtual IOReturn notifyHostapState(apple80211_hostap_state *); // RT?
    virtual IOReturn getNanInterface(void); // RT?
    virtual IOReturn nanConnectionStarted(void); // RT?
    virtual IOReturn inputInfraPacket(__mbuf *); // RT?
    virtual IOReturn getSSIDData(apple80211_ssid_data *); // RT?
    virtual IOReturn getSSIDTransitionEnabled(apple80211_ssid_transition_feature_enabled *); // RT?
    virtual IOReturn getBSSIDData(IO80211SkywalkInterface *,apple80211_bssid_data *); // RT?
    virtual IOReturn getBeaconPeriod(apple80211_beacon_period_data *); // RT?
    virtual IOReturn getDTIMInt(apple80211_dtim_int_data *); // RT?
    virtual IOReturn setPowerStats(apple80211_stat_report *,apple80211_power_debug_sub_info *); // RT?
    virtual void setFrameStats(apple80211_stat_report *,apple80211_frame_counters *,apple80211_channel *);
    virtual void setInfraSpecificFrameStats(apple80211_stat_report *,apple80211_infra_specific_stats *);
    virtual IOReturn setLeakyAPStats(apple80211_leaky_ap_event *); // RT?
    virtual IOReturn setChipCounterStats(apple80211_stat_report *,apple80211_chip_stats *,apple80211_channel *); // RT?
    virtual IOReturn setExtendedChipCounterStats(apple80211_stat_report *,void *); // RT?
    virtual IOReturn getInfraChannel(void); // RT?
    virtual IOReturn getInfraChannel(apple80211_channel_data *); // RT?
    virtual IOReturn setCountryCode(apple80211_country_code_data *); // RT?
    virtual IOReturn getCountryCode(apple80211_country_code_data *); // RT?
    virtual IOReturn setAMPDUstat(apple80211_stat_report *,apple80211_ampdu_stat_report *,apple80211_channel *); // RT?
    virtual IOReturn setBTCoexstat(apple80211_stat_report *,apple80211_btCoex_report *); // RT?
    virtual IOReturn setLTECoexstat(apple80211_stat_report *,apple80211_lteCoex_report *); // RT?
    virtual IOReturn setChanExtendedCCA(apple80211_stat_report *,apple80211_cca_report *); // RT?
    virtual IOReturn setChanCCA(apple80211_stat_report *,int); // RT?
    virtual IOReturn setInterfaceRSSI(int); // RT?
    virtual IOReturn getInterfaceMonitor(void); // RT?
    virtual void setInterfaceNF(int);
    virtual IOReturn setInterfaceSNR(int); // RT?
    virtual IOReturn updateAwdlRadioDutyCyclePct(unsigned short); // RT?
    virtual IOReturn getAwdlRadioDutyCyclePct(void); // RT?
    virtual IOReturn logTxLatency(unsigned long long,unsigned int); // RT?
    virtual IOReturn incrementTxLatency(apple80211_latency *,unsigned int); // RT?
    virtual IOReturn getTxLatencyClearOnRead(apple80211_latency_all_ac &); // RT?
    virtual IOReturn incrementRxLatency(apple80211_latency *,unsigned int); // RT?
    virtual IOReturn logRxLatency(unsigned long long,unsigned int); // RT?
    virtual IOReturn getRxLatencyClearOnRead(apple80211_latency_all_ac &); // RT?
    virtual IOReturn setChanNoiseFloor(apple80211_stat_report *,int); // RT?
    virtual IOReturn setChanNoiseFloorLTE(apple80211_stat_report *,int); // RT?
    virtual bool createWorkQueue(void);
    virtual IOReturn getWorkQueue(void); // RT?
    virtual IOReturn getIO80211CommandGate(void); // RT?
    virtual IO80211SkywalkInterface getPrimarySkywalkInterface(void);
    virtual void getHardwareAddress(IOEthernetAddress *);
    virtual IOReturn acquireMaxBuffer(void); // RT?
    virtual IOReturn releaseMaxBuffer(IO80211CagedBuffer *); // RT?
    virtual IOReturn getMaxBufferLock(void); // RT?
    virtual const char * stringFromReturn(int);
    virtual int errnoFromReturn(int);
    virtual IOReturn getASSOCIATE_RESULT(IO80211SkywalkInterface *,apple80211_assoc_result_data *); // RT?
    virtual IOReturn getASSOCIATE_EXTENDED_RESULT(IO80211SkywalkInterface *,apple80211_assoc_result_data *); // RT?
    virtual bool attachInterface(OSObject *,IOService *);
    virtual void detachInterface(OSObject *,bool);
    virtual bool attachVirtualInterface(IO80211VirtualInterface * *,ether_addr *,unsigned int,bool);
    virtual bool detachVirtualInterface(IO80211VirtualInterface *,bool);
    virtual IO80211VirtualInterface createVirtualInterface(ether_addr *,unsigned int);
    virtual void dataLinkLayerAttachComplete(void);
    virtual void requestPacketTx(void *,unsigned int);
    virtual int bpfOutputPacket(OSObject *,unsigned int,__mbuf *);
    virtual IOReturn copyIn(unsigned long long,void *,unsigned long); // RT?
    virtual IOReturn copyOut(void const *,unsigned long long,unsigned long); // RT?
    virtual IOReturn getUserspaceP2POptions(void); // RT?
    virtual IOReturn setUserspaceP2POptions(unsigned long long); // RT?
    virtual UInt32 getDataQueueDepth(OSObject *);
    virtual SInt32 enableVirtualInterface(IO80211VirtualInterface *);
    virtual SInt32 disableVirtualInterface(IO80211VirtualInterface *);
    virtual IOReturn getPeerStatsID(ether_addr *,unsigned char *); // RT?
    virtual IOReturn removePeerStatsID(unsigned char); // RT?
    virtual IOReturn lockIOReporterLegend(void); // RT?
    virtual IOReturn unlockIOReporterLegend(void); // RT?
    virtual IOReturn syncReporterLegendWork(void); // RT?
    virtual IOReturn reporterLegendHasChanges(void); // RT?
    virtual IOReturn getIOReporterLegendCalmTimerValue(unsigned int); // RT?
    virtual IOReturn getSynthesizedIOReportLegendWithGroupSubgroup(apple80211_ior_legend_query *); // RT?
    virtual IOReturn syncReporterLegendWorkInternal(void); // RT?
    virtual IOReturn removeReporterFromLegend(IOService *,IOReporter *,char const *,char const *); // RT?
    virtual IOReturn removeReporterFromLegendSync(IOService *,IOReporter *,char const *,char const *); // RT?
    virtual IOReturn verifyContentsOfLegend(OSDictionary *,char const *,char const *); // RT?
    virtual IOReturn getCommonFaultReporter(void); // RT?
    virtual IOReturn verifyNoDuplicateChannelIDsInLegend(OSArray *,OSDictionary *,char const *,char const *); // RT?
    virtual IOReturn verifyNoDuplicateChannelIDs(IOService *,IOReporter *,char const *,char const *); // RT?
    virtual IOReturn addReporterLegend(IOService *,IOReporter *,char const *,char const *); // RT?
    virtual IOReturn verifyContentsOfReportersLegend(IOReporter *,char const *,char const *); // RT?
    virtual IOReturn addReporterLegendSync(IOService *,IOReporter *,char const *,char const *); // RT?
    virtual IOReturn setIORValidation(bool,bool); // RT?
    virtual IOReturn configureReport(IOReportChannelList *,unsigned int,void *,void *);
    virtual IOReturn getNanDataInterface(void); // RT?
    virtual IOReturn updateReport(IOReportChannelList *,unsigned int,void *,void *);
    virtual IOReturn getPrimaryInterfaceScanManager(void); // RT?
    virtual IOReturn getInfraInterface(void); // RT?
    virtual IOReturn resetIO80211ReporterHistory(void); // RT?
    virtual IOReturn setDisplayState(bool); // RT?
    virtual UInt32 getFeatures(void);
    virtual const OSString newVendorString(void);
    virtual const OSString newModelString(void);
    virtual bool createWorkLoop(void);
    virtual IOWorkLoop getWorkLoop(void);
    virtual SInt32 monitorModeSetEnabled(bool,unsigned int);
    virtual UInt32 hardwareOutputQueueDepth(void);
    virtual SInt32 performCountryCodeOperation(IO80211CountryCodeOp);
    virtual SInt32 enableFeature(IO80211FeatureCode,void *);
    virtual IOReturn setSleeping(bool); // RT?
    virtual bool requiresExplicitMBufRelease(void);
    virtual IOReturn getSaplInterface(void); // RT?
    virtual IOReturn getNanMgmtInterface(void); // RT?
    virtual IOReturn setNanInterface(IO80211VirtualInterface *); // RT?
    virtual IOReturn setNanMgmtInterface(IO80211VirtualInterface *); // RT?
    virtual IOReturn setNanDataInterface(IO80211VirtualInterface *); // RT?
    virtual IOReturn getLLWInterface(void); // RT?
    virtual IOReturn setAwdlInterface(IO80211VirtualInterface *); // RT?
    virtual IOReturn setSapInterface(IO80211VirtualInterface *); // RT?
    virtual bool flowIdSupported(void);
    virtual void getLogPipes(CCPipe * *,CCPipe * *,CCPipe * *);
    virtual void enableFeatureForLoggingFlags(unsigned long long);
    virtual IOReturn requestQueueSizeAndTimeout(unsigned short *,unsigned short *);
    virtual IOReturn enablePacketTimestamping(void);
    virtual IOReturn disablePacketTimestamping(void);
    virtual UInt getPacketTSCounter(void);
    virtual IOReturn getEventCCStream(void); // RT?
    virtual IOReturn getControllerMonitor(void); // RT?
    virtual IOReturn getRangingManager(void); // RT?
    virtual bool wasDynSARInFailSafeMode(void);
    virtual IOReturn isIOCTLLogicDisabled(void); // RT?
    virtual IOReturn isIOUCPreferred(void); // RT?
    virtual IOReturn shouldPanicOnIOCTL(void); // RT?
    virtual IOReturn isEventSocketDisabled(void); // RT?
    virtual IOReturn getLqmCrashTracerFeatureFlagEnabled(void); // RT?
    virtual IOReturn setLqmCrashTracerFeatureFlagEnabled(unsigned int); // RT?
    virtual IOReturn getP2PForULLRTInfraFeatureFlagEnabled(void); // RT?
    virtual IOReturn setP2PForULLRTInfraFeatureFlagEnabled(bool); // RT?
    virtual void postMessage(IO80211SkywalkInterface *,unsigned int,void *,unsigned long,bool);
    virtual IOReturn postMessageSync(IO80211SkywalkInterface *,unsigned int,void *,unsigned long,bool); // RT?
    virtual IOReturn setPromiscuousMode(bool);
    virtual IOReturn setMulticastMode(bool);
    virtual IOReturn setMulticastList(ether_addr const *,unsigned int);
    virtual IOReturn onDispatchQueue(void); // RT?
    virtual IOReturn isLQMOSLOGEnabled(void); // RT?
    virtual IOReturn isLQMIOLOGEnabled(void); // RT?
    virtual IOReturn isLQMCCLOGEnabled(void); // RT?
    virtual IOReturn logLQMToCC(char const *,...); // RT?
    virtual IOReturn getRNGAgent(void); // RT?
    virtual void updateAdvisoryScoresIfNeed(void);
    virtual UInt64 getAVCAdvisoryInfo(IO80211InterfaceAVCAdvisory *);
    virtual IOReturn CreatePostOffice(void); // RT?
    virtual IOReturn getPostOffice(void); // RT?
    virtual IOReturn getPLATFORM_CONFIG(IO80211SkywalkInterface *,apple80211_platform_config *); // RT?
    virtual IOReturn getActionFramePoolCapacity(void); // RT?
    virtual IOReturn getMgmtFramePool(void); // RT?
    virtual IOReturn loadCardCapabilities(IO80211SkywalkInterface *); // RT?
    virtual IOReturn isCardCapabilitiesSet(apple80211_card_capability); // RT?
    virtual IOReturn getOSFeatureFlags(void); // RT?
    virtual IOReturn setOSFeatureFlags(apple80211_feature_flags *); // RT?
    virtual IOReturn updateWoWReasonToIoReg(unsigned int,char *,unsigned long,unsigned int); // RT?
    virtual IOReturn getInterfaceActiveBitmap(void); // RT?
    virtual IOReturn getGlobalLogger(void); // RT?
    virtual IOReturn allocIO80211RecursiveLock(void); // RT?
};

#endif