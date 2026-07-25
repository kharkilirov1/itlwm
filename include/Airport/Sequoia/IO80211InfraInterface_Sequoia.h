// IO80211InfraInterface � ������, ����������� �� IO80211Family 15.7.4 (24G517)
// ��������� �� �������� KDK; return type �� ���������� � mangling � ������� TODO.

class IO80211InfraInterface {
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
