// IO80211Sequoia.h — иерархия классов IO80211Family 15.7.4 (24G517)
// Восстановлено из KDK-символов: методы (mangled->demangled) + наследование (OSMetaClass).
// IO80211InfraInterface, IO80211VirtualInterface : IO80211SkywalkInterface : IOSkywalkEthernetInterface
// IO80211Controller : IOEthernetController
#ifndef IO80211_SEQUOIA_H
#define IO80211_SEQUOIA_H

#include <IOKit/network/IOEthernetController.h>
#include <Airport/IOSkywalkEthernetInterface.h>

class IO80211SkywalkInterface : public IOSkywalkEthernetInterface {
public:
    virtual IOReturn /*RT?*/ IO80211SkywalkInterface(OSMetaClass const *);  // TODO: return type
    virtual IOReturn /*RT?*/ getMetaClass(void);  // TODO: return type
    virtual IOReturn /*RT?*/ initIvars(void);  // TODO: return type
    virtual IOReturn /*RT?*/ postCAEventForIOCTLLegacyUsage(int,unsigned long long);  // TODO: return type
    virtual bool init(void);
    virtual bool init(IOService *,ether_addr *);
    virtual bool start(IOService *);
    virtual IOReturn /*RT?*/ getInterfaceRoleCounter(SkywalkInterfaceRole);  // TODO: return type
    virtual IOReturn /*RT?*/ getInterfaceRole(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getInterfaceRoleStr(void);  // TODO: return type
    virtual IOReturn /*RT?*/ logDebug(unsigned long long,char const *,...);  // TODO: return type
    virtual IOReturn /*RT?*/ getPeerManager(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getInterfaceId(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getBSDName(void);  // TODO: return type
    virtual IOReturn /*RT?*/ logDebug(char const *,...);  // TODO: return type
    virtual IOReturn /*RT?*/ setInterfaceRole(unsigned int);  // TODO: return type
    virtual void free(void);
    virtual SInt32 initBSDInterfaceParameters(ifnet_init_eparams *,sockaddr_dl * *);
    virtual bool prepareBSDInterface(__ifnet *,unsigned int);
    virtual IOReturn /*RT?*/ createIOReporters(void);  // TODO: return type
    virtual IOReturn /*RT?*/ validateDispatchQueue(void);  // TODO: return type
    virtual ifnet_t getIfnet(void);
    virtual IOReturn /*RT?*/ getRingMD(IO80211APIUserClient *,unsigned long long);  // TODO: return type
    virtual bool isIOUCPipeOpened(void);
    virtual void postMessageIOUC(char const *,unsigned int,void *,unsigned long);
    virtual IOReturn newUserClient(task *,void *,unsigned int,OSDictionary *,IOUserClient * *);
    virtual int getEventPipeSize(void);
    virtual UInt64 createEventPipe(IO80211APIUserClient *);
    virtual void destroyEventPipe(IO80211APIUserClient *);
    virtual IOReturn /*RT?*/ setUserBufferInfo(IOMemoryDescriptor *,unsigned long long);  // TODO: return type
    virtual IOReturn /*RT?*/ processBSDCommandGated(unsigned int,void *);  // TODO: return type
    virtual IOReturn /*RT?*/ performGatedCommandIOUC(unsigned int,void *);  // TODO: return type
    virtual IOReturn processBSDCommand(__ifnet *,unsigned int,void *);
    virtual IOReturn /*RT?*/ performGatedCommandIOCTL(unsigned int,void *);  // TODO: return type
    virtual IOReturn /*RT?*/ storeProcessNameAndIoctlInformation(unsigned long);  // TODO: return type
    virtual IOReturn /*RT?*/ isCommandAllowed(void);  // TODO: return type
    virtual bool createPeerManager(void);
    virtual IOReturn /*RT?*/ createPeer(unsigned char const *,IO80211PeerManager *);  // TODO: return type
    virtual IOReturn /*RT?*/ storeIoctlInArray(OSArray *,OSNumber *);  // TODO: return type
    virtual IOReturn /*RT?*/ dumpUnentitledProcesses(void);  // TODO: return type
    virtual IOReturn /*RT?*/ logDebugHex(void const *,unsigned long,char const *,...);  // TODO: return type
    virtual void vlogDebug(unsigned long long,char const *,__va_list_tag *);
    virtual void vlogDebugBPF(unsigned long long,char const *,__va_list_tag *);
    virtual bool shouldLog(unsigned long long);
    virtual void handleDebugCmd(apple80211_debug_command *);
    virtual void printDataPath(userPrintCtx *);
    virtual UInt32 getDataQueueDepth(void);
    virtual const char* stringFromReturn(int);
    virtual IOReturn /*RT?*/ getControllerWorkQueue(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getWorkQueue(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getPeerMonitor(IO80211Peer *);  // TODO: return type
    virtual IOReturn /*RT?*/ getLinkQualityMonitor(IO80211Peer *);  // TODO: return type
    virtual IOReturn /*RT?*/ getLinkRecovery(IO80211Peer *);  // TODO: return type
    virtual IOReturn /*RT?*/ getLqmCrashTracer(IO80211Peer *);  // TODO: return type
    virtual IOReturn getHardwareAddress(ether_addr *);
    virtual IOReturn /*RT?*/ getSelfMacAddr(void);  // TODO: return type
    virtual IOReturn /*RT?*/ setInitMacAddress(ether_addr &);  // TODO: return type
    virtual IOReturn setHardwareAddress(ether_addr *);
    virtual IOReturn setSET_MAC_ADDRESS(apple80211_set_mac_address *);
    virtual IOReturn /*RT?*/ updateTimeSyncMacAddress(ether_addr &);  // TODO: return type
    virtual IOMediumType getMediumType(void);
    virtual IOReturn handleSIOCSIFADDR(void);
    virtual bool isDebounceOnGoing(void);
    virtual bool setLinkState(IO80211LinkState,unsigned int,bool,unsigned int);
    virtual SInt32 setRunningState(bool);
    virtual void postMessage(unsigned int,void *,unsigned long,bool);
    virtual IOReturn /*RT?*/ postMessageInternal(unsigned int,void *,unsigned long,bool);  // TODO: return type
    virtual IOReturn /*RT?*/ setPidLock(bool);  // TODO: return type
    virtual IOReturn /*RT?*/ pidLocked(void);  // TODO: return type
    virtual IOReturn setPowerState(unsigned long,IOService *);
    virtual IOReturn /*RT?*/ getPowerState(void);  // TODO: return type
    virtual void stop(IOService *);
    virtual unsigned long initialPowerStateForDomainState(unsigned long);
    virtual unsigned long maxCapabilityForDomainState(unsigned long);
    virtual IOReturn /*RT?*/ getP2PSkywalkPeerMgr(void);  // TODO: return type
    virtual IOReturn /*RT?*/ postMessageSync(unsigned int,void *,unsigned long);  // TODO: return type
    virtual IOReturn /*RT?*/ reportDataPathEventsGated(void *,void *,void *,void *,void *);  // TODO: return type
    virtual IOReturn reportDataPathEvents(unsigned int,void *,unsigned long,bool);
    virtual IOReturn /*RT?*/ getSupportedMediaArray(unsigned int *,unsigned int *);  // TODO: return type
    virtual IOReturn handleChosenMedia(unsigned int);
    virtual IOReturn setPromiscuousModeEnable(bool,unsigned int);
    virtual IOReturn /*RT?*/ recordInputPacket(int,int);  // TODO: return type
    virtual IOReturn /*RT?*/ getInterfaceMonitor(void);  // TODO: return type
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
    virtual IOReturn /*RT?*/ setInterfaceId(unsigned int);  // TODO: return type
    virtual IOReturn /*RT?*/ getCompanionInterfaceId(void);  // TODO: return type
    virtual IOReturn /*RT?*/ setCompanionInterfaceId(unsigned int);  // TODO: return type
    virtual bool isChipInterfaceReady(void);
    virtual void setScanningState(unsigned int,bool,apple80211_scan_data *,int);
    virtual IOReturn configureReport(IOReportChannelList *,unsigned int,void *,void *);
    virtual IOReturn updateReport(IOReportChannelList *,unsigned int,void *,void *);
    virtual void setDataPathState(bool);
    virtual IOReturn /*RT?*/ getController(void);  // TODO: return type
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
    virtual IOReturn /*RT?*/ getPropertyTable(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getWorkerMatchingDict(OSString *);  // TODO: return type
    virtual bool isInterfaceEnabled(void);
    virtual IOReturn /*RT?*/ getPacketPool(OSString *);  // TODO: return type
    virtual IOReturn /*RT?*/ getLogger(void);  // TODO: return type
    virtual IOReturn debugHandler(apple80211_debug_command *);
    virtual IOReturn /*RT?*/ getDeviceType(void);  // TODO: return type
    virtual IOReturn /*RT?*/ setDeviceType(unsigned int);  // TODO: return type
    virtual IOReturn /*RT?*/ setParentInterface(IO80211VirtualInterface *);  // TODO: return type
    virtual IOReturn /*RT?*/ getParentInterface(void);  // TODO: return type
    virtual IOReturn /*RT?*/ setLowLatencyEnabled(bool);  // TODO: return type
    virtual IOReturn /*RT?*/ isLowLatencyEnabled(void);  // TODO: return type
    virtual IOReturn /*RT?*/ pidLockPid(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getInterfaceSnapshots(void);  // TODO: return type
    virtual IOReturn /*RT?*/ setInterfaceSnapshots(IO80211AssociationJoinSnapshot *);  // TODO: return type
    virtual IO80211LinkState linkState(void);
    virtual void setInterfaceNF(apple80211_channel,long long);
    virtual void setInterfaceOFDMDesense(apple80211_channel,long long);
    virtual void setNotificationProperty(OSSymbol const *,OSObject const *);
    virtual IOReturn /*RT?*/ getLQMSummary(apple80211_lqm_summary *);  // TODO: return type
    virtual IOReturn /*RT?*/ getScanManager(void);  // TODO: return type
    virtual void statsDump(void);
    virtual void powerOnNotification(void);
    virtual void powerOffNotification(void);
    virtual UInt64 getTxQueueDepth(void);
    virtual UInt64 getRxQueueCapacity(void);
    virtual void updateRxCounter(unsigned long long);
    virtual IOReturn /*RT?*/ getMultiCastQueue(void);  // TODO: return type
    virtual int getAssocState(void);
    virtual void notifyQueueState(apple80211_wme_ac,unsigned short);
    virtual int getTxHeadroom(void);
    virtual IOReturn /*RT?*/ getRxCompQueue(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getTxCompQueue(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getTxSubQueue(apple80211_wme_ac);  // TODO: return type
    virtual IOReturn /*RT?*/ getTxPacketPool(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getRxPacketPool(void);  // TODO: return type
    virtual void enableDatapath(void);
    virtual void disableDatapath(void);
    virtual int getNumTxQueues(void);
    virtual IOReturn enable(unsigned int);
    virtual IOReturn disable(unsigned int);
    virtual IOReturn /*RT?*/ classNameOverride(void);  // TODO: return type
    virtual int errnoFromReturn(int);
    virtual SInt32 setInterfaceEnable(bool);
    virtual UInt32 getFeatureFlags(void);
    virtual bool isCommandProhibited(int);
    virtual IOReturn /*RT?*/ routeIoctlToWcl(unsigned int,unsigned int,void *,unsigned long);  // TODO: return type
    virtual IOReturn /*RT?*/ attachPeer(ether_addr *);  // TODO: return type
    virtual IOReturn /*RT?*/ detachPeer(ether_addr *);  // TODO: return type
    virtual IOReturn /*RT?*/ setDebugTrafficReport(bool);  // TODO: return type
    virtual IOReturn /*RT?*/ getMacAddressAgent(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getDataPathInterfaceStats(apple80211_data_path_interface_stats *);  // TODO: return type
    virtual IOReturn /*RT?*/ getDataPathPeerStats(apple80211_data_path_peer_stats *);  // TODO: return type
    virtual IOReturn /*RT?*/ getLastQueuePacketTime(ether_addr *);  // TODO: return type
    virtual IOReturn /*RT?*/ getLastRxUnicastLinkActivityTime(ether_addr *);  // TODO: return type
    virtual IOReturn /*RT?*/ updateInterfaceDataStats(apple80211_data_path_interface_stats *);  // TODO: return type
    virtual IOReturn /*RT?*/ updatePeerDataStats(apple80211_data_path_peer_stats *);  // TODO: return type
    virtual IOReturn /*RT?*/ logTxLatency(unsigned char *,unsigned int,unsigned long long);  // TODO: return type
    virtual IOReturn /*RT?*/ findPeer(ether_addr &);  // TODO: return type
    virtual IOReturn /*RT?*/ logRxLatency(unsigned int,unsigned long long);  // TODO: return type
    virtual IOReturn /*RT?*/ getNClearTxRxLatency(apple80211_latency_all_ac *,apple80211_latency_all_ac *);  // TODO: return type
    virtual IOReturn /*RT?*/ getLastTxTimeStamp(unsigned long long &);  // TODO: return type
    virtual IOReturn /*RT?*/ getLastRxTimeStamp(unsigned long long &);  // TODO: return type
};

class IO80211InfraInterface : public IO80211SkywalkInterface {
public:
    virtual IOReturn /*RT?*/ IO80211InfraInterface(OSMetaClass const *);  // TODO: return type
    virtual IOReturn /*RT?*/ getMetaClass(void);  // TODO: return type
    virtual bool start(IOService *);
    virtual IOReturn /*RT?*/ triggerLinkStatusUpdate(IO80211TimerSource *);  // TODO: return type
    virtual IOReturn /*RT?*/ reportDataTransferRatesTimer(IO80211TimerSource *);  // TODO: return type
    virtual IOReturn /*RT?*/ publishOffloadCapability(void);  // TODO: return type
    virtual void stop(IOService *);
    virtual bool init(void);
    virtual void free(void);
    virtual IOReturn /*RT?*/ createLQMData(void);  // TODO: return type
    virtual SInt32 initBSDInterfaceParameters(ifnet_init_eparams *,sockaddr_dl * *);
    virtual bool prepareBSDInterface(__ifnet *,unsigned int);
    virtual IOReturn /*RT?*/ updateStaticProperties(void);  // TODO: return type
    virtual IOReturn processBSDCommand(__ifnet *,unsigned int,void *);
    virtual IOReturn /*RT?*/ updateMediumStatus(ifmediareq *);  // TODO: return type
    virtual bool isDebounceOnGoing(void);
    virtual bool setLinkState(IO80211LinkState,unsigned int,bool,unsigned int);
    virtual IOReturn setLinkStateInternal(IO80211LinkState,unsigned int,bool,unsigned int);
    virtual IOReturn /*RT?*/ handleKeyDone(bool,bool);  // TODO: return type
    virtual IOReturn /*RT?*/ updateLinkSpeed(void);  // TODO: return type
    virtual IOReturn /*RT?*/ cancelDebounceTimer(void);  // TODO: return type
    virtual IO80211LinkState linkState(void);
    virtual void postMessage(unsigned int,void *,unsigned long,bool);
    virtual IOReturn /*RT?*/ getInfraPeer(void);  // TODO: return type
    virtual IOReturn /*RT?*/ bssidChange(void *,unsigned long);  // TODO: return type
    virtual IOReturn /*RT?*/ UpdateULLADuration(unsigned long long *);  // TODO: return type
    virtual IOReturn /*RT?*/ finishSIBCoexTimer(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getAVCAdvisory(void);  // TODO: return type
    virtual IOReturn /*RT?*/ updateChanOutageTime(apple80211_infra_scan_start_event_data *);  // TODO: return type
    virtual IOReturn /*RT?*/ loadHwChannels(void);  // TODO: return type
    virtual IOReturn /*RT?*/ loadChannelInfo(void);  // TODO: return type
    virtual IOReturn /*RT?*/ resetInterface(void *,unsigned long);  // TODO: return type
    virtual IOReturn /*RT?*/ updateSSIDProperty(void);  // TODO: return type
    virtual IOReturn /*RT?*/ updateCountryCodeProperty(bool);  // TODO: return type
    virtual IOReturn /*RT?*/ routeToP2PInterface(unsigned int,void *,unsigned long);  // TODO: return type
    virtual IOReturn /*RT?*/ updateBSSIDProperty(ether_addr &,apple80211_channel &,bool);  // TODO: return type
    virtual IOReturn /*RT?*/ updateChannelProperty(apple80211_channel &);  // TODO: return type
    virtual IOReturn /*RT?*/ updateLocaleProperty(void);  // TODO: return type
    virtual void setWoWEnabled(bool);
    virtual void setScanningState(unsigned int,bool,apple80211_scan_data *,int);
    virtual IOReturn configureReport(IOReportChannelList *,unsigned int,void *,void *);
    virtual IOReturn updateReport(IOReportChannelList *,unsigned int,void *,void *);
    virtual IOReturn /*RT?*/ handleLeakyApStatsResetTimer(IO80211TimerSource *);  // TODO: return type
    virtual IOReturn /*RT?*/ getLeakyApStats(apple80211_leaky_ap_stats const * *);  // TODO: return type
    virtual IOReturn /*RT?*/ resetLeakyApStats(void);  // TODO: return type
    virtual IOReturn /*RT?*/ setLeakyApSsidMetrics(apple80211_leaky_ap_ssid_metrics *);  // TODO: return type
    virtual IOReturn /*RT?*/ setLeakyAPStats(apple80211_leaky_ap_event *);  // TODO: return type
    virtual IOReturn /*RT?*/ setLeakyAPStatsMode(unsigned int);  // TODO: return type
    virtual IOReturn recordOutputPackets(TxSubmissionDequeueStats *,TxSubmissionDequeueStats *);
    virtual void logTxPacket(IO80211NetworkPacket *,PacketSkywalkScratch *,apple80211_wme_ac,bool);
    virtual IOReturn /*RT?*/ getAwdlMaxBandWidth(void);  // TODO: return type
    virtual IOReturn /*RT?*/ get5GLowHighBandSwitchCounter(void);  // TODO: return type
    virtual IOReturn /*RT?*/ get5GLowHighBandSwitchSuccessPerc(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getULLAClassicDuration(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getCoPTxRTSFailCount(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getULLALiteDuration(void);  // TODO: return type
    virtual IOReturn /*RT?*/ resetCoPTxRTSFailCount(void);  // TODO: return type
    virtual IOReturn /*RT?*/ resetSIBTurnOnMetrics(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getCoPSIBCoexTurnOnCount(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getCoPSIBCoexTurnOnDuration(void);  // TODO: return type
    virtual IOReturn /*RT?*/ setGatewayAddress(apple80211_infra_peer_address_data);  // TODO: return type
    virtual IOReturn /*RT?*/ setUnicastInfraPeerAddress(apple80211_infra_peer_address_data);  // TODO: return type
    virtual IOReturn /*RT?*/ setInfraPeersLoggingEnabled(bool);  // TODO: return type
    virtual IOReturn recordCompletionPackets(TxCompletionEnqueueStats *,TxCompletionEnqueueStats *);
    virtual void logTxCompletionPacket(IO80211NetworkPacket *,PacketSkywalkScratch *,unsigned char *,apple80211_wme_ac,int,unsigned int,bool,bool);
    virtual mbuf_flags_t inputPacket(IO80211NetworkPacket *,packet_info_tag *,ether_header *,bool *,bool);
    virtual void updateLinkParameters(apple80211_interface_availability *);
    virtual IOReturn /*RT?*/ updateLinkParametersStatic(void *,void *);  // TODO: return type
    virtual IOReturn /*RT?*/ updateLinkParametersGated(apple80211_interface_availability *);  // TODO: return type
    virtual void updateInterfaceCoexRiskPct(unsigned long long);
    virtual IOReturn /*RT?*/ reportDataTransferRates(void);  // TODO: return type
    virtual IOReturn /*RT?*/ setBTCoexWLANLostAntennaTime(unsigned long long,unsigned long long,bool,apple80211_btCoex_report *);  // TODO: return type
    virtual IOReturn /*RT?*/ reportDataTransferRatesStatic(void *);  // TODO: return type
    virtual IOReturn /*RT?*/ getLQMData(void);  // TODO: return type
    virtual void setLQM(unsigned long long);
    virtual IOReturn /*RT?*/ setLQMStatic(void *,void *);  // TODO: return type
    virtual IOReturn /*RT?*/ setLQMGated(unsigned long long);  // TODO: return type
    virtual void updateLinkStatus(void);
    virtual IOReturn /*RT?*/ updateLinkStatusStatic(void *);  // TODO: return type
    virtual void updateLinkStatusGated(void);
    virtual void setInterfaceExtendedCCA(apple80211_channel,apple80211_cca_report *);
    virtual void setInterfaceCCA(apple80211_channel,int);
    virtual void setInterfaceNF(apple80211_channel,long long);
    virtual void setInterfaceOFDMDesense(apple80211_channel,long long);
    virtual IOReturn /*RT?*/ setAMPDUstat(apple80211_stat_report *,apple80211_ampdu_stat_report *);  // TODO: return type
    virtual void setDataPathState(bool);
    virtual void setDebugFlags(unsigned long long,unsigned int);
    virtual void setPeerManagerLogFlag(unsigned int,unsigned int,unsigned int);
    virtual SInt64 debugFlags(void);
    virtual void setInterfaceChipCounters(apple80211_stat_report *,apple80211_chip_counters_tx *,apple80211_chip_error_counters_tx *,apple80211_chip_counters_rx *);
    virtual void setInterfaceMIBdot11(apple80211_stat_report *,apple80211_ManagementInformationBasedot11_counters *);
    virtual void setFrameStats(apple80211_stat_report *,apple80211_frame_counters *);
    virtual void setInfraSpecificFrameStats(apple80211_stat_report *,apple80211_infra_specific_stats *);
    virtual SInt64 getWmeTxCounters(unsigned long long *);
    virtual IOReturn /*RT?*/ resetTxPathHealthCheck(void);  // TODO: return type
    virtual UInt64 createLinkQualityMonitor(IO80211Peer *,IOService *);
    virtual IOReturn /*RT?*/ getInfraLinkProperties(IO80211Peer *,unsigned int &,unsigned int &,apple80211_channel &,apple80211_channel_width &,apple80211_phymode &,apple80211_channel_band &);  // TODO: return type
    virtual void releaseLinkQualityMonitor(IO80211Peer *);
    virtual IOReturn /*RT?*/ notifyAWDLStateChange(bool);  // TODO: return type
    virtual int bpfOutputPacket(__mbuf *,void *);
    virtual IOReturn /*RT?*/ bpfOutput(unsigned int,__mbuf *);  // TODO: return type
    virtual bool bpfTap(unsigned int,unsigned int);
    virtual IOReturn /*RT?*/ bpfTapInternal(unsigned int,unsigned int);  // TODO: return type
    virtual IOReturn /*RT?*/ getWMMBWReset(void);  // TODO: return type
    virtual IOReturn /*RT?*/ setWMMBWReset(bool);  // TODO: return type
    virtual bool wowEnabled(void);
    virtual IOReturn /*RT?*/ getScanManager(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getMonitorMode(void);  // TODO: return type
    virtual SInt64 pendingPackets(unsigned char);
    virtual SInt64 packetSpace(unsigned char);
    virtual IOReturn /*RT?*/ getLQMSummary(apple80211_lqm_summary *);  // TODO: return type
    virtual IOReturn /*RT?*/ getTrafficMonitor(void);  // TODO: return type
    virtual UInt getHardwareAssists(void);
    virtual int getAssocState(void);
    virtual IOReturn /*RT?*/ setCurrentApAddress(ether_addr *);  // TODO: return type
    virtual IOReturn /*RT?*/ getCurrentApAddress(void);  // TODO: return type
    virtual IOReturn /*RT?*/ setLQMConfig(apple80211_lqm_config_t *);  // TODO: return type
    virtual SInt32 setInterfaceEnable(bool);
    virtual IOReturn /*RT?*/ onDispatchQueue(void);  // TODO: return type
    virtual void setWCL_ADVISORTY_INFO(apple80211_wcl_advisory_info *);
    virtual IOReturn /*RT?*/ getWCL_TX_RX_LATENCY(apple80211_wcl_tx_rx_latency *);  // TODO: return type
    virtual IOReturn /*RT?*/ updateTxRxLatency(void);  // TODO: return type
    virtual IOReturn /*RT?*/ setIPv4Params(apple80211_ipv4_params *);  // TODO: return type
    virtual IOReturn /*RT?*/ getIPv4Params(void);  // TODO: return type
    virtual IOReturn /*RT?*/ setIPv6Params(apple80211_ipv6_params *);  // TODO: return type
    virtual IOReturn /*RT?*/ getIPv6Params(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getPacketFilterData(apple80211_packet_filter_data &);  // TODO: return type
    virtual IOReturn /*RT?*/ getTcpUdpKeepAliveData(apple80211_tcp_udp_keepalive_data &);  // TODO: return type
    virtual IOReturn /*RT?*/ bonjourOffloadNumActivePorts(unsigned int &);  // TODO: return type
    virtual IOReturn /*RT?*/ getTcpKeepAliveConfigData(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getBonjourOffloadConfigData(unsigned short &,unsigned int &);  // TODO: return type
    virtual IOReturn /*RT?*/ getArpKeepAliveData(apple80211_arp_keepalive_data &,bool);  // TODO: return type
    virtual IOReturn /*RT?*/ getProtocolOffloadData(apple80211_protocol_offload_data &);  // TODO: return type
    virtual IOReturn /*RT?*/ getMagicPatternData(apple80211_magic_pattern_data &);  // TODO: return type
    virtual IOReturn getSTATS(apple80211_stats_data *);
    virtual IOReturn /*RT?*/ getBtCoexState(void);  // TODO: return type
    virtual IOReturn getBT_COEX_FLAGS(apple80211_state_data *);
    virtual IOReturn setBT_COEX_FLAGS(apple80211_state_data *);
    virtual IOReturn setPowerState(unsigned long,IOService *);
    virtual IOReturn /*RT?*/ setWCLSystemState(apple80211_system_state &);  // TODO: return type
    virtual void hwConfigNicProxyData(nicproxy_info_s *);
    virtual IOReturn /*RT?*/ setLeScanParams(apple80211_le_scan_params &);  // TODO: return type
    virtual IOReturn /*RT?*/ getHwChannels(apple80211_sup_channel_data &);  // TODO: return type
    virtual IOReturn /*RT?*/ registerInfraEthernetInterface(IOSkywalkEthernetInterface::RegistrationInfo *,IOSkywalkPacketQueue * *,unsigned int,IOSkywalkPacketBufferPool *,IOSkywalkPacketBufferPool *);  // TODO: return type
    virtual IOReturn getLOCALE(apple80211_locale_data *);
};

class IO80211VirtualInterface : public IO80211SkywalkInterface {
public:
    virtual IOReturn /*RT?*/ IO80211VirtualInterface(OSMetaClass const *);  // TODO: return type
    virtual IOReturn /*RT?*/ getMetaClass(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211VirtualInterface0(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211VirtualInterface1(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211VirtualInterface2(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211VirtualInterface3(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211VirtualInterface4(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211VirtualInterface5(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211VirtualInterface6(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211VirtualInterface7(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211VirtualInterface8(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211VirtualInterface9(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211VirtualInterface10(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211VirtualInterface11(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211VirtualInterface12(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211VirtualInterface13(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211VirtualInterface14(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211VirtualInterface15(void);  // TODO: return type
    virtual bool attach(IOService *);
    virtual void detach(IOService *);
    virtual IOReturn /*RT?*/ peerToPeerAttachToBpf(void);  // TODO: return type
    virtual IOReturn /*RT?*/ bpfAttachEN10MB(unsigned int);  // TODO: return type
    virtual bool attachToBpf(void);
    virtual IOReturn /*RT?*/ isPeerToPeerInterface(void);  // TODO: return type
    virtual IOReturn /*RT?*/ freeBpfResources(void);  // TODO: return type
    virtual IOReturn /*RT?*/ allocBpfResources(void);  // TODO: return type
    virtual bool init(IO80211Controller *,ether_addr *,unsigned int,char const *);
    virtual IOReturn /*RT?*/ logDebug(char const *,...);  // TODO: return type
    virtual bool start(IOService *);
    virtual IOReturn enable(unsigned int);
    virtual bool createPeerManager(void);
    virtual IOReturn /*RT?*/ setInfraChannel(apple80211_channel *);  // TODO: return type
    virtual IOReturn /*RT?*/ handleChannelSwitchAnnouncement(apple80211_channel_switch_announcement *);  // TODO: return type
    virtual IOReturn /*RT?*/ setJoiningState(unsigned int,joinStatus,bool);  // TODO: return type
    virtual IOReturn /*RT?*/ isAwdlAssistedDiscoveryEnabled(void);  // TODO: return type
    virtual IOReturn /*RT?*/ notifyHostapState(apple80211_hostap_state *);  // TODO: return type
    virtual const char* stringFromReturn(int);
    virtual UInt32 configureAQMOutput(void);
    virtual void free(void);
    virtual bool terminate(unsigned int);
    virtual void stop(IOService *);
    virtual IOReturn /*RT?*/ configureBpfOutputQueues(bool);  // TODO: return type
    virtual int bpfOutputPacket(__mbuf *,void *);
    virtual bool bpfTap(unsigned int,unsigned int);
    virtual IOReturn /*RT?*/ bpfTapInput(__mbuf *,unsigned int,void *,unsigned long);  // TODO: return type
    virtual IOReturn /*RT?*/ getMgmtFramePool(void);  // TODO: return type
    virtual IOReturn /*RT?*/ bpfTapInput(IO80211Buffer *,unsigned int,void *,unsigned long);  // TODO: return type
    virtual IOReturn /*RT?*/ sendToBpfTap(IO80211Buffer *,unsigned int,void *,unsigned long);  // TODO: return type
    virtual IOReturn /*RT?*/ peerToPeerConfigureIfnet(void);  // TODO: return type
    virtual bool configureIfnet(void);
    virtual IOReturn /*RT?*/ dequeueTxPackets(TxPacketRequest *);  // TODO: return type
    virtual IOReturn /*RT?*/ storeProcessNameAndIoctlInformation(unsigned long);  // TODO: return type
    virtual IOReturn /*RT?*/ storeIoctlInArray(OSArray *,OSNumber *);  // TODO: return type
    virtual IOReturn /*RT?*/ logDebug(unsigned long long,char const *,...);  // TODO: return type
    virtual IOReturn /*RT?*/ dumpUnentitledProcesses(void);  // TODO: return type
    virtual IOReturn /*RT?*/ ioctl_internal_gated(void *,void *,void *,void *,void *);  // TODO: return type
    virtual IOReturn /*RT?*/ ioctl_internal(void *);  // TODO: return type
    virtual bool prepareBSDInterface(__ifnet *,unsigned int);
    virtual IOReturn processBSDCommand(__ifnet *,unsigned int,void *);
    virtual UInt32 getFeatureFlags(void);
    virtual SInt32 setInterfaceEnable(bool);
    virtual bool setMulticastAddresses(ether_addr const *,unsigned int);
    virtual void handleIoctl(__ifnet *,unsigned long,void *);
    virtual IOReturn /*RT?*/ handleIoctlGated(void *,void *,void *,void *,void *);  // TODO: return type
    virtual IOReturn /*RT?*/ terminating(void);  // TODO: return type
    virtual void setScanningState(unsigned int,bool,apple80211_scan_data *,int);
    virtual IOReturn /*RT?*/ setInfraTxState(bool);  // TODO: return type
    virtual void updateLinkParameters(apple80211_interface_availability *);
    virtual void updateInterfaceCoexRiskPct(unsigned long long);
    virtual IOReturn /*RT?*/ postAwdlStatistics(apple80211_awdl_statistics *);  // TODO: return type
    virtual IOReturn /*RT?*/ IO80211InterfacePostMessage(unsigned int,void *,unsigned long);  // TODO: return type
    virtual IOReturn /*RT?*/ postSyncStateChanged(void);  // TODO: return type
    virtual SInt64 getWmeTxCounters(unsigned long long *);
    virtual void logSkywalkTxReqPacket(IO80211NetworkPacket *,PacketSkywalkScratch *,unsigned char *,apple80211_wme_ac,bool);
    virtual void flushPacketQueues(void);
    virtual void removePacketQueue(IO80211FlowQueueHash const *);
    virtual IOReturn /*RT?*/ getPacketQueueDatabase(void);  // TODO: return type
    virtual SInt64 pendingPackets(unsigned char);
    virtual IOReturn /*RT?*/ queueSize(unsigned char);  // TODO: return type
    virtual SInt64 packetSpace(unsigned char);
    virtual UInt64 findExistingFlowQueue(IO80211FlowQueueHash);
    virtual void cachePeer(ether_addr *,unsigned int *);
    virtual IOReturn /*RT?*/ isPeerManagerDataDisabled(void);  // TODO: return type
    virtual bool findOrCreateFlowQueue(IO80211FlowQueueHash);
    virtual UInt64 findOrCreateFlowQueueWithCache(IO80211FlowQueueHash,bool *);
    virtual IOReturn /*RT?*/ dropTxPacket(__mbuf *);  // TODO: return type
    virtual IOReturn /*RT?*/ reportTransmitCompletionStatus(__mbuf *,int,unsigned int,unsigned int,unsigned int);  // TODO: return type
    virtual bool isOutputFlowControlled(void);
    virtual void setOutputFlowControlled(void);
    virtual void clearOutputFlowControlled(void);
    virtual IOReturn /*RT?*/ forwardPacket(IO80211NetworkPacket *);  // TODO: return type
    virtual IOReturn /*RT?*/ resetUserClientReference(void);  // TODO: return type
    virtual IOReturn /*RT?*/ resetUserClientReferenceGated(OSObject *,void *,void *,void *,void *);  // TODO: return type
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
    virtual IOReturn /*RT?*/ setAMPDUstat(apple80211_stat_report *,apple80211_ampdu_stat_report *);  // TODO: return type
    virtual IOReturn /*RT?*/ pushPacket(__mbuf *);  // TODO: return type
    virtual mbuf_flags_t inputPacket(IO80211NetworkPacket *,packet_info_tag *,ether_header *,bool *,bool);
    virtual void logTxPacket(IO80211NetworkPacket *,PacketSkywalkScratch *,apple80211_wme_ac,bool);
    virtual void logTxCompletionPacket(IO80211NetworkPacket *,PacketSkywalkScratch *,unsigned char *,apple80211_wme_ac,int,unsigned int,bool,bool);
    virtual IOReturn /*RT?*/ getOutputQueueForDLT(unsigned int);  // TODO: return type
    virtual void handleDebugCmd(apple80211_debug_command *);
    virtual void printDataPath(userPrintCtx *);
    virtual void vlogDebug(unsigned long long,char const *,__va_list_tag *);
    virtual void vlogDebugBPF(unsigned long long,char const *,__va_list_tag *);
    virtual bool shouldLog(unsigned long long);
    virtual IOReturn configureReport(IOReportChannelList *,unsigned int,void *,void *);
    virtual IOReturn /*RT?*/ getCompanionLowLatencySkywalkInterface(void);  // TODO: return type
    virtual IOReturn updateReport(IOReportChannelList *,unsigned int,void *,void *);
    virtual IOReturn /*RT?*/ postAwdlHppStatsEvent(apple80211_awdl_low_latency_statistics_reduced *);  // TODO: return type
    virtual IOReturn /*RT?*/ postLowlatencyStatistics(apple80211_nan_low_latency_peer_statistics *);  // TODO: return type
    virtual IOReturn /*RT?*/ postP2PAirplayStatistics(apple80211_p2p_airplay_statistics *);  // TODO: return type
    virtual IOReturn /*RT?*/ postHostapChannelChanged(apple80211_hostap_state *);  // TODO: return type
    virtual IOReturn /*RT?*/ postP2pThreadCoexEvent(apple80211_p2p_thread_coex_event *);  // TODO: return type
    virtual IOReturn /*RT?*/ postHostAPBandUpdate(apple80211_channel_band);  // TODO: return type
    virtual IOReturn /*RT?*/ startAsyncEventUserClientForTask(task *,kIO80211InterfaceType);  // TODO: return type
    virtual int getEventPipeSize(void);
    virtual UInt64 createEventPipe(IO80211APIUserClient *);
    virtual IOReturn /*RT?*/ p2pDaemonExited(void);  // TODO: return type
    virtual IOReturn /*RT?*/ p2pDaemonExitedStatic(void *);  // TODO: return type
    virtual IOReturn /*RT?*/ p2pDaemonExitedGated(void);  // TODO: return type
    virtual IOReturn newUserClient(task *,void *,unsigned int,OSDictionary *,IOUserClient * *);
    virtual IOReturn /*RT?*/ addPeerToCache(unsigned char *);  // TODO: return type
    virtual IOReturn /*RT?*/ deletePeerFromCache(unsigned char *);  // TODO: return type
    virtual IOReturn /*RT?*/ reset(void);  // TODO: return type
    virtual IOReturn /*RT?*/ setDisplayState(bool);  // TODO: return type
    virtual IOReturn /*RT?*/ getP2PSkywalkPeerMgr(void);  // TODO: return type
    virtual IOReturn /*RT?*/ wmmSupported(void);  // TODO: return type
    virtual bool wowEnabled(void);
    virtual IOReturn /*RT?*/ getRoamingFlags(void);  // TODO: return type
    virtual IOReturn /*RT?*/ setRoamingFlags(unsigned int);  // TODO: return type
    virtual IOReturn /*RT?*/ setCompanionLowLatencySkywalkInterface(IO80211SkywalkInterface *);  // TODO: return type
    virtual IOReturn /*RT?*/ dupAndTransmitMcastPacket(IO80211NetworkPacket *,unsigned char *,unsigned char *);  // TODO: return type
    virtual IOReturn /*RT?*/ modifyUnicastPacket(unsigned char *,unsigned int,unsigned char,unsigned int *,unsigned char *);  // TODO: return type
    virtual IOReturn /*RT?*/ handleMulticastTransmit(IO80211NetworkPacket *,unsigned char *,unsigned int,unsigned char);  // TODO: return type
    virtual IOReturn /*RT?*/ freeMulticastPacket(IO80211NetworkPacket *);  // TODO: return type
    virtual IOReturn /*RT?*/ triggerAWDLMulticastTx(unsigned int,unsigned char);  // TODO: return type
    virtual IOReturn /*RT?*/ p2pMulticastTx(IO80211NetworkPacket * *,unsigned int);  // TODO: return type
    virtual IOReturn /*RT?*/ setInterfaceSuspended(void);  // TODO: return type
    virtual IOReturn /*RT?*/ clearInterfaceSuspended(void);  // TODO: return type
    virtual IOReturn /*RT?*/ isInterfaceSuspended(void);  // TODO: return type
    virtual IOReturn /*RT?*/ setPeerManagerDataDisabled(bool);  // TODO: return type
    virtual IOReturn /*RT?*/ getPeerManagerDataDisabled(void);  // TODO: return type
    virtual IOReturn /*RT?*/ stashMulticastPacketPair(IO80211NetworkPacket *,IO80211NetworkPacket *);  // TODO: return type
    virtual IOReturn /*RT?*/ sendPendingPacketsToStack(IO80211NetworkPacket * *,unsigned int);  // TODO: return type
    virtual IOReturn /*RT?*/ freePendingPackets(IO80211NetworkPacket * *,unsigned int);  // TODO: return type
    virtual IOReturn /*RT?*/ setAwdlCurrentChannelSequenceIndex(unsigned int);  // TODO: return type
    virtual IOReturn /*RT?*/ getPeerSlotDataStats(ether_addr *,bool,peerSlotDataStats *);  // TODO: return type
    virtual IOReturn /*RT?*/ getPeerDataStats(ether_addr *,bool,packetCounters_t *);  // TODO: return type
    virtual IOReturn /*RT?*/ clearPeerDataStats(ether_addr *,bool);  // TODO: return type
    virtual IOReturn /*RT?*/ findPeer(ether_addr &);  // TODO: return type
};

class IO80211Controller : public IOEthernetController {
public:
    virtual IOReturn /*RT?*/ IO80211Controller(OSMetaClass const *);  // TODO: return type
    virtual IOReturn /*RT?*/ getMetaClass(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211Controller0(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211Controller1(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211Controller2(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211Controller3(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211Controller4(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211Controller5(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211Controller6(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211Controller7(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211Controller8(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211Controller9(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211Controller10(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211Controller11(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211Controller12(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211Controller13(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211Controller14(void);  // TODO: return type
    virtual IOReturn /*RT?*/ _RESERVEDIO80211Controller15(void);  // TODO: return type
    virtual bool start(IOService *);
    virtual IOReturn /*RT?*/ setGlobalLogger(CCLogStream *);  // TODO: return type
    virtual IOReturn /*RT?*/ createIOReporters(IOService *);  // TODO: return type
    virtual IOReturn /*RT?*/ setupControlPathLogging(void);  // TODO: return type
    virtual IOReturn /*RT?*/ findAndAttachToFaultReporter(void);  // TODO: return type
    virtual IOReturn /*RT?*/ logDebug(char const *,...);  // TODO: return type
    virtual IOReturn /*RT?*/ GetProvider(void);  // TODO: return type
    virtual IOReturn /*RT?*/ handleIOReporterLegendIsCalm(IO80211TimerSource *);  // TODO: return type
    virtual IOReturn /*RT?*/ allocFramePool(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getReporterProvider(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getTimerFactory(void);  // TODO: return type
    virtual bool init(OSDictionary *);
    virtual IOReturn /*RT?*/ debugStateInit(void);  // TODO: return type
    virtual void free(void);
    virtual void stop(IOService *);
    virtual IOReturn /*RT?*/ releaseIOReporters(void);  // TODO: return type
    virtual IOReturn /*RT?*/ refreshFamilyLQMLogs(FamilyLqmLogId,char *);  // TODO: return type
    virtual IOReturn /*RT?*/ getFamilyLQMLogs(apple80211_lqm_summary *);  // TODO: return type
    virtual IOReturn /*RT?*/ setInfraChannel(apple80211_channel *);  // TODO: return type
    virtual IOReturn /*RT?*/ getAwdlInterface(void);  // TODO: return type
    virtual IOReturn /*RT?*/ calculateInterfacesCoex(void);  // TODO: return type
    virtual IO80211FlowQueueLegacy* requestFlowQueue(FlowIdMetadata const *);
    virtual void releaseFlowQueue(IO80211FlowQueue *);
    virtual IOReturn /*RT?*/ dispatchPacket(__mbuf *,unsigned int,char *,unsigned int);  // TODO: return type
    virtual IOReturn /*RT?*/ getDriverTextLog(void);  // TODO: return type
    virtual IOReturn /*RT?*/ isStarted(void);  // TODO: return type
    virtual IOReturn /*RT?*/ logDebug(unsigned long long,char const *,...);  // TODO: return type
    virtual IOReturn /*RT?*/ io80211isDebuggable(bool *);  // TODO: return type
    virtual void vlogDebug(unsigned long long,char const *,__va_list_tag *);
    virtual void vlogDebugBPF(unsigned long long,char const *,__va_list_tag *);
    virtual IOReturn /*RT?*/ clearLogTargets(void);  // TODO: return type
    virtual IOReturn /*RT?*/ setLogTarget(IO80211VirtualInterface *,bool);  // TODO: return type
    virtual UInt32 selfDiagnosticsReport(int,char const *,unsigned int);
    virtual IOReturn /*RT?*/ getHtCapabilityLength(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getHtCapability(void);  // TODO: return type
    virtual IOReturn /*RT?*/ setHtCapability(ieee80211_ht_capability_ie *);  // TODO: return type
    virtual IOReturn /*RT?*/ setCurrentChannel(apple80211_channel *);  // TODO: return type
    virtual IOReturn /*RT?*/ setPrimaryInterfaceDatapathState(bool);  // TODO: return type
    virtual IOReturn /*RT?*/ setChannelSequenceList(apple80211_awdl_sync_channel_sequence *);  // TODO: return type
    virtual IOReturn /*RT?*/ calculateInterfacesAvaiability(void);  // TODO: return type
    virtual void updateInterfaceCoexRiskPct(unsigned long long);
    virtual IOReturn /*RT?*/ printChannels(void);  // TODO: return type
    virtual IOReturn /*RT?*/ scanStarted(scanSource,apple80211_scan_data *);  // TODO: return type
    virtual IOReturn /*RT?*/ scanDone(scanSource,int);  // TODO: return type
    virtual IOReturn /*RT?*/ handleChannelSwitchAnnouncement(apple80211_channel_switch_announcement *);  // TODO: return type
    virtual IOReturn /*RT?*/ joinStarted(scanSource,joinStatus);  // TODO: return type
    virtual IOReturn /*RT?*/ joinDone(scanSource,joinStatus);  // TODO: return type
    virtual IOReturn /*RT?*/ getPid(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getProcessName(char *,unsigned long);  // TODO: return type
    virtual IOReturn /*RT?*/ isAwdlAssistedDiscoveryEnabled(void);  // TODO: return type
    virtual IOReturn /*RT?*/ notifyHostapState(apple80211_hostap_state *);  // TODO: return type
    virtual IOReturn /*RT?*/ getNanInterface(void);  // TODO: return type
    virtual IOReturn /*RT?*/ nanConnectionStarted(void);  // TODO: return type
    virtual IOReturn /*RT?*/ inputInfraPacket(__mbuf *);  // TODO: return type
    virtual IOReturn /*RT?*/ getSSIDData(apple80211_ssid_data *);  // TODO: return type
    virtual IOReturn /*RT?*/ getSSIDTransitionEnabled(apple80211_ssid_transition_feature_enabled *);  // TODO: return type
    virtual IOReturn /*RT?*/ getBSSIDData(IO80211SkywalkInterface *,apple80211_bssid_data *);  // TODO: return type
    virtual IOReturn /*RT?*/ getBeaconPeriod(apple80211_beacon_period_data *);  // TODO: return type
    virtual IOReturn /*RT?*/ getDTIMInt(apple80211_dtim_int_data *);  // TODO: return type
    virtual IOReturn /*RT?*/ setPowerStats(apple80211_stat_report *,apple80211_power_debug_sub_info *);  // TODO: return type
    virtual void setFrameStats(apple80211_stat_report *,apple80211_frame_counters *,apple80211_channel *);
    virtual void setInfraSpecificFrameStats(apple80211_stat_report *,apple80211_infra_specific_stats *);
    virtual IOReturn /*RT?*/ setLeakyAPStats(apple80211_leaky_ap_event *);  // TODO: return type
    virtual IOReturn /*RT?*/ setChipCounterStats(apple80211_stat_report *,apple80211_chip_stats *,apple80211_channel *);  // TODO: return type
    virtual IOReturn /*RT?*/ setExtendedChipCounterStats(apple80211_stat_report *,void *);  // TODO: return type
    virtual IOReturn /*RT?*/ getInfraChannel(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getInfraChannel(apple80211_channel_data *);  // TODO: return type
    virtual IOReturn /*RT?*/ setCountryCode(apple80211_country_code_data *);  // TODO: return type
    virtual IOReturn /*RT?*/ getCountryCode(apple80211_country_code_data *);  // TODO: return type
    virtual IOReturn /*RT?*/ setAMPDUstat(apple80211_stat_report *,apple80211_ampdu_stat_report *,apple80211_channel *);  // TODO: return type
    virtual IOReturn /*RT?*/ setBTCoexstat(apple80211_stat_report *,apple80211_btCoex_report *);  // TODO: return type
    virtual IOReturn /*RT?*/ setLTECoexstat(apple80211_stat_report *,apple80211_lteCoex_report *);  // TODO: return type
    virtual IOReturn /*RT?*/ setChanExtendedCCA(apple80211_stat_report *,apple80211_cca_report *);  // TODO: return type
    virtual IOReturn /*RT?*/ setChanCCA(apple80211_stat_report *,int);  // TODO: return type
    virtual IOReturn /*RT?*/ setInterfaceRSSI(int);  // TODO: return type
    virtual IOReturn /*RT?*/ getInterfaceMonitor(void);  // TODO: return type
    virtual void setInterfaceNF(int);
    virtual IOReturn /*RT?*/ setInterfaceSNR(int);  // TODO: return type
    virtual IOReturn /*RT?*/ updateAwdlRadioDutyCyclePct(unsigned short);  // TODO: return type
    virtual IOReturn /*RT?*/ getAwdlRadioDutyCyclePct(void);  // TODO: return type
    virtual IOReturn /*RT?*/ logTxLatency(unsigned long long,unsigned int);  // TODO: return type
    virtual IOReturn /*RT?*/ incrementTxLatency(apple80211_latency *,unsigned int);  // TODO: return type
    virtual IOReturn /*RT?*/ getTxLatencyClearOnRead(apple80211_latency_all_ac &);  // TODO: return type
    virtual IOReturn /*RT?*/ incrementRxLatency(apple80211_latency *,unsigned int);  // TODO: return type
    virtual IOReturn /*RT?*/ logRxLatency(unsigned long long,unsigned int);  // TODO: return type
    virtual IOReturn /*RT?*/ getRxLatencyClearOnRead(apple80211_latency_all_ac &);  // TODO: return type
    virtual IOReturn /*RT?*/ setChanNoiseFloor(apple80211_stat_report *,int);  // TODO: return type
    virtual IOReturn /*RT?*/ setChanNoiseFloorLTE(apple80211_stat_report *,int);  // TODO: return type
    virtual bool createWorkQueue(void);
    virtual IOReturn /*RT?*/ getWorkQueue(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getIO80211CommandGate(void);  // TODO: return type
    virtual IO80211SkywalkInterface* getPrimarySkywalkInterface(void);
    virtual IOReturn getHardwareAddress(IOEthernetAddress *);
    virtual IOReturn /*RT?*/ acquireMaxBuffer(void);  // TODO: return type
    virtual IOReturn /*RT?*/ releaseMaxBuffer(IO80211CagedBuffer *);  // TODO: return type
    virtual IOReturn /*RT?*/ getMaxBufferLock(void);  // TODO: return type
    virtual const char* stringFromReturn(int);
    virtual int errnoFromReturn(int);
    virtual IOReturn /*RT?*/ getASSOCIATE_RESULT(IO80211SkywalkInterface *,apple80211_assoc_result_data *);  // TODO: return type
    virtual IOReturn /*RT?*/ getASSOCIATE_EXTENDED_RESULT(IO80211SkywalkInterface *,apple80211_assoc_result_data *);  // TODO: return type
    virtual bool attachInterface(OSObject *,IOService *);
    virtual void detachInterface(OSObject *,bool);
    virtual bool attachVirtualInterface(IO80211VirtualInterface * *,ether_addr *,unsigned int,bool);
    virtual bool detachVirtualInterface(IO80211VirtualInterface *,bool);
    virtual IO80211VirtualInterface* createVirtualInterface(ether_addr *,unsigned int);
    virtual void dataLinkLayerAttachComplete(void);
    virtual void requestPacketTx(void *,unsigned int);
    virtual int bpfOutputPacket(OSObject *,unsigned int,__mbuf *);
    virtual IOReturn /*RT?*/ copyIn(unsigned long long,void *,unsigned long);  // TODO: return type
    virtual IOReturn /*RT?*/ copyOut(void const *,unsigned long long,unsigned long);  // TODO: return type
    virtual IOReturn /*RT?*/ getUserspaceP2POptions(void);  // TODO: return type
    virtual IOReturn /*RT?*/ setUserspaceP2POptions(unsigned long long);  // TODO: return type
    virtual UInt32 getDataQueueDepth(OSObject *);
    virtual SInt32 enableVirtualInterface(IO80211VirtualInterface *);
    virtual SInt32 disableVirtualInterface(IO80211VirtualInterface *);
    virtual IOReturn /*RT?*/ getPeerStatsID(ether_addr *,unsigned char *);  // TODO: return type
    virtual IOReturn /*RT?*/ removePeerStatsID(unsigned char);  // TODO: return type
    virtual IOReturn /*RT?*/ lockIOReporterLegend(void);  // TODO: return type
    virtual IOReturn /*RT?*/ unlockIOReporterLegend(void);  // TODO: return type
    virtual IOReturn /*RT?*/ syncReporterLegendWork(void);  // TODO: return type
    virtual IOReturn /*RT?*/ reporterLegendHasChanges(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getIOReporterLegendCalmTimerValue(unsigned int);  // TODO: return type
    virtual IOReturn /*RT?*/ getSynthesizedIOReportLegendWithGroupSubgroup(apple80211_ior_legend_query *);  // TODO: return type
    virtual IOReturn /*RT?*/ syncReporterLegendWorkInternal(void);  // TODO: return type
    virtual IOReturn /*RT?*/ removeReporterFromLegend(IOService *,IOReporter *,char const *,char const *);  // TODO: return type
    virtual IOReturn /*RT?*/ removeReporterFromLegendSync(IOService *,IOReporter *,char const *,char const *);  // TODO: return type
    virtual IOReturn /*RT?*/ verifyContentsOfLegend(OSDictionary *,char const *,char const *);  // TODO: return type
    virtual IOReturn /*RT?*/ getCommonFaultReporter(void);  // TODO: return type
    virtual IOReturn /*RT?*/ verifyNoDuplicateChannelIDsInLegend(OSArray *,OSDictionary *,char const *,char const *);  // TODO: return type
    virtual IOReturn /*RT?*/ verifyNoDuplicateChannelIDs(IOService *,IOReporter *,char const *,char const *);  // TODO: return type
    virtual IOReturn /*RT?*/ addReporterLegend(IOService *,IOReporter *,char const *,char const *);  // TODO: return type
    virtual IOReturn /*RT?*/ verifyContentsOfReportersLegend(IOReporter *,char const *,char const *);  // TODO: return type
    virtual IOReturn /*RT?*/ addReporterLegendSync(IOService *,IOReporter *,char const *,char const *);  // TODO: return type
    virtual IOReturn /*RT?*/ setIORValidation(bool,bool);  // TODO: return type
    virtual IOReturn configureReport(IOReportChannelList *,unsigned int,void *,void *);
    virtual IOReturn /*RT?*/ getNanDataInterface(void);  // TODO: return type
    virtual IOReturn updateReport(IOReportChannelList *,unsigned int,void *,void *);
    virtual IOReturn /*RT?*/ getPrimaryInterfaceScanManager(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getInfraInterface(void);  // TODO: return type
    virtual IOReturn /*RT?*/ resetIO80211ReporterHistory(void);  // TODO: return type
    virtual IOReturn /*RT?*/ setDisplayState(bool);  // TODO: return type
    virtual UInt32 getFeatures(void);
    virtual const OSString * newVendorString(void);
    virtual const OSString * newModelString(void);
    virtual bool createWorkLoop(void);
    virtual IOWorkLoop* getWorkLoop(void);
    virtual SInt32 monitorModeSetEnabled(bool,unsigned int);
    virtual UInt32 hardwareOutputQueueDepth(void);
    virtual SInt32 performCountryCodeOperation(IO80211CountryCodeOp);
    virtual SInt32 enableFeature(IO80211FeatureCode,void *);
    virtual IOReturn /*RT?*/ setSleeping(bool);  // TODO: return type
    virtual bool requiresExplicitMBufRelease(void);
    virtual IOReturn /*RT?*/ getSaplInterface(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getNanMgmtInterface(void);  // TODO: return type
    virtual IOReturn /*RT?*/ setNanInterface(IO80211VirtualInterface *);  // TODO: return type
    virtual IOReturn /*RT?*/ setNanMgmtInterface(IO80211VirtualInterface *);  // TODO: return type
    virtual IOReturn /*RT?*/ setNanDataInterface(IO80211VirtualInterface *);  // TODO: return type
    virtual IOReturn /*RT?*/ getLLWInterface(void);  // TODO: return type
    virtual IOReturn /*RT?*/ setAwdlInterface(IO80211VirtualInterface *);  // TODO: return type
    virtual IOReturn /*RT?*/ setSapInterface(IO80211VirtualInterface *);  // TODO: return type
    virtual bool flowIdSupported(void);
    virtual void getLogPipes(CCPipe * *,CCPipe * *,CCPipe * *);
    virtual void enableFeatureForLoggingFlags(unsigned long long);
    virtual IOReturn requestQueueSizeAndTimeout(unsigned short *,unsigned short *);
    virtual IOReturn enablePacketTimestamping(void);
    virtual IOReturn disablePacketTimestamping(void);
    virtual UInt getPacketTSCounter(void);
    virtual IOReturn /*RT?*/ getEventCCStream(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getControllerMonitor(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getRangingManager(void);  // TODO: return type
    virtual bool wasDynSARInFailSafeMode(void);
    virtual IOReturn /*RT?*/ isIOCTLLogicDisabled(void);  // TODO: return type
    virtual IOReturn /*RT?*/ isIOUCPreferred(void);  // TODO: return type
    virtual IOReturn /*RT?*/ shouldPanicOnIOCTL(void);  // TODO: return type
    virtual IOReturn /*RT?*/ isEventSocketDisabled(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getLqmCrashTracerFeatureFlagEnabled(void);  // TODO: return type
    virtual IOReturn /*RT?*/ setLqmCrashTracerFeatureFlagEnabled(unsigned int);  // TODO: return type
    virtual IOReturn /*RT?*/ getP2PForULLRTInfraFeatureFlagEnabled(void);  // TODO: return type
    virtual IOReturn /*RT?*/ setP2PForULLRTInfraFeatureFlagEnabled(bool);  // TODO: return type
    virtual void postMessage(IO80211SkywalkInterface *,unsigned int,void *,unsigned long,bool);
    virtual IOReturn /*RT?*/ postMessageSync(IO80211SkywalkInterface *,unsigned int,void *,unsigned long,bool);  // TODO: return type
    virtual IOReturn setPromiscuousMode(bool);
    virtual IOReturn setMulticastMode(bool);
    virtual IOReturn setMulticastList(ether_addr const *,unsigned int);
    virtual IOReturn /*RT?*/ onDispatchQueue(void);  // TODO: return type
    virtual IOReturn /*RT?*/ isLQMOSLOGEnabled(void);  // TODO: return type
    virtual IOReturn /*RT?*/ isLQMIOLOGEnabled(void);  // TODO: return type
    virtual IOReturn /*RT?*/ isLQMCCLOGEnabled(void);  // TODO: return type
    virtual IOReturn /*RT?*/ logLQMToCC(char const *,...);  // TODO: return type
    virtual IOReturn /*RT?*/ getRNGAgent(void);  // TODO: return type
    virtual void updateAdvisoryScoresIfNeed(void);
    virtual UInt64 getAVCAdvisoryInfo(IO80211InterfaceAVCAdvisory *);
    virtual IOReturn /*RT?*/ CreatePostOffice(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getPostOffice(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getPLATFORM_CONFIG(IO80211SkywalkInterface *,apple80211_platform_config *);  // TODO: return type
    virtual IOReturn /*RT?*/ getActionFramePoolCapacity(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getMgmtFramePool(void);  // TODO: return type
    virtual IOReturn /*RT?*/ loadCardCapabilities(IO80211SkywalkInterface *);  // TODO: return type
    virtual IOReturn /*RT?*/ isCardCapabilitiesSet(apple80211_card_capability);  // TODO: return type
    virtual IOReturn /*RT?*/ getOSFeatureFlags(void);  // TODO: return type
    virtual IOReturn /*RT?*/ setOSFeatureFlags(apple80211_feature_flags *);  // TODO: return type
    virtual IOReturn /*RT?*/ updateWoWReasonToIoReg(unsigned int,char *,unsigned long,unsigned int);  // TODO: return type
    virtual IOReturn /*RT?*/ getInterfaceActiveBitmap(void);  // TODO: return type
    virtual IOReturn /*RT?*/ getGlobalLogger(void);  // TODO: return type
    virtual IOReturn /*RT?*/ allocIO80211RecursiveLock(void);  // TODO: return type
};

#endif