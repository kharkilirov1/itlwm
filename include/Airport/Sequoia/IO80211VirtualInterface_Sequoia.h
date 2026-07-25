// IO80211VirtualInterface � ������, ����������� �� IO80211Family 15.7.4 (24G517)
// ��������� �� �������� KDK; return type �� ���������� � mangling � ������� TODO.

class IO80211VirtualInterface {
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
