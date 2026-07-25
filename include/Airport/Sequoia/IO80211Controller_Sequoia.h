// IO80211Controller � ������, ����������� �� IO80211Family 15.7.4 (24G517)
// ��������� �� �������� KDK; return type �� ���������� � mangling � ������� TODO.

class IO80211Controller {
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
