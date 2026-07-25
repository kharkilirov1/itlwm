// IO80211SkywalkInterface � ������, ����������� �� IO80211Family 15.7.4 (24G517)
// ��������� �� �������� KDK; return type �� ���������� � mangling � ������� TODO.

class IO80211SkywalkInterface {
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
