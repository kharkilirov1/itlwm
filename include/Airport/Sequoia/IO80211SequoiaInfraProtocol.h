// IO80211InfraProtocol ABI for IO80211Family 15.7.4 (24G517).
// Method order is the object-vptr slot order (Itanium header words excluded).
#ifndef IO80211_SEQUOIA_INFRA_PROTOCOL_H
#define IO80211_SEQUOIA_INFRA_PROTOCOL_H

#include <Airport/Sequoia/IO80211Sequoia.h>

struct apple80211_channel_data;
struct apple80211_powersave_data;
struct apple80211_txpower_data;
struct apple80211_rate_data;
struct apple80211_opmode_data;
struct apple80211_rssi_data;
struct apple80211_sup_channel_data;
struct apple80211_guard_interval_data;
struct apple80211_mcs_data;
struct apple80211_power_debug_info;
struct apple80211_ht_capability;
struct apple80211_mcs_vht_data;
struct apple80211_channels_info;
struct apple80211_vht_capability;
struct apple80211_roam_profile_all_bands;
struct apple80211_chip_stats;
struct apple80211_dbg_guard_time_params;
struct apple80211_leaky_ap_setting;
struct apple80211_country_channel_data;
struct apple80211_private_mac_data;
struct apple80211_ranging_enable_request_t;
struct apple80211_ranging_start_request_t;
struct apple80211_rsdb_capability;
struct apple80211_tko_params;
struct apple80211_tko_dump;
struct apple80211_btcoex_profile;
struct apple80211_btcoex_profile_active_data;
struct apple80211_trap_info_data;
struct apple80211_thermal_index_t;
struct apple80211_btcoex_max_nss_for_ap_data;
struct apple80211_btcoex_2g_chain_disable;
struct apple80211_power_budget_t;
struct apple80211_offload_tcpka_enable_t;
struct apple80211_ranging_capabilities_t;
struct apple80211_lqm_config_t;
struct apple80211_trap_mini_dump_data;
struct apple80211_beacon_info_t;
struct apple80211_chip_power_limit;
struct apple80211_nss_data;
struct apple80211_hw_mac_address;
struct appl80211_chip_diags_data;
struct apple80211_hp2p_ctrl;
struct bss_blacklist;
struct apple80211_txrx_chain_info;
struct apple80211_mimo_status;
struct apple80211_pmk;
struct apple80211_dynsar_detail;
struct apple80211_lqm_summary;
struct apple80211_slow_wifi_feature_enabled;
struct apple80211_timesync_info;
struct apple80211_sensing_data_t;
struct apple80211_fw_hot_channels;
struct apple80211_low_latency_info;
struct apple80211_beacon_msg;
struct apple80211_wcl_traffic_counters;
struct apple80211_he_counters_ctl;
struct apple80211ChannelInfo;
struct apple80211_rsn_xe_data;
struct apple80211_sib_coex_status;
struct apple80211_wifi_bt_5g_policy_t;
struct apple80211_extended_bss_info;
struct apple80211_wcl_low_latency_stats;
struct apple80211_bgscan_cached_network_data_list;
struct apple80211_wcl_wnm_offload_t;
struct apple80211_noise_per_ant_t;
struct apple80211_fw_clock_info;
struct apple80211_timesync_stats;
struct apple80211_system_sleep_config;
struct apple80211_key;
struct apple80211_network_data;
struct apple80211_apmode_data;
struct apple80211_ie_data;
struct apple80211_wow_test_data;
struct apple80211_virt_if_create_data;
struct apple80211_offload_arp_data;
struct apple80211_offload_ndp_data;
struct apple80211_gas_query_t;
struct apple80211_reset_command;
struct apple80211_crash_command;
struct apple80211_ranging_authenticate_request_t;
struct apple80211_dynamic_rssi_window_config;
struct apple80211_usb_host_notification_data;
struct apple80211_set_property_unserialized_data;
struct apple80211_roam_cache_data;
struct apple80211_pm_mode;
struct apple80211_wifi_assertion_data;
struct apple80211_state_data;
struct apple80211_sensing_enable_t;
struct apple80211_sensing_disable_t;
struct apple80211_6G_mode;
struct apple80211_leave_network;
struct apple80211_reassoc;
struct apple80211_set_roam_lock;
struct apple80211_legacy_roam_profile_config;
struct apple80211_roam_profile_config;
struct apple80211_user_roam_cache;
struct apple80211_wcl_real_time_mode;
struct apple80211_wcl_arp_mode;
struct triggerCC;
struct apple80211ScanRequest;
struct apple80211AssocCandidates;
struct apple80211_wcl_qos_params;
struct scanHomeAndAwayTime;
struct apple80211_voice_ind_state;
struct apple80211_wcl_update_link_state;
struct apple80211_wcl_ulofdma_state;
struct apple80211_wcl_action_frame;
struct apple80211_feature_flags;
struct apple80211_dhcp_renewal_data;
struct apple80211_battery_ps_config;
struct apple80211_mimo_config;
struct apple80211_bg_motion_profile;
struct apple80211_bg_network;
struct apple80211_bg_scan;
struct apple80211_bg_params;
struct apple80211_power_profile;
struct apple80211_interface_setting;
struct apple80211_bypass_tx_power_cap;
struct apple80211_facetime_wificalling_params;
struct apple80211_ipv4_params;
struct apple80211_wcl_wnm_config_t;

// M(slot, method, parameter_type)
#define IO80211_SEQUOIA_INFRA_PROTOCOL_METHODS(M)                                  \
    M(465, getCHANNEL, apple80211_channel_data)                                    \
    M(466, getPOWERSAVE, apple80211_powersave_data)                                \
    M(467, getTXPOWER, apple80211_txpower_data)                                    \
    M(468, getRATE, apple80211_rate_data)                                          \
    M(469, getOP_MODE, apple80211_opmode_data)                                     \
    M(470, getRSSI, apple80211_rssi_data)                                          \
    M(471, getSUPPORTED_CHANNELS, apple80211_sup_channel_data)                     \
    M(472, getGUARD_INTERVAL, apple80211_guard_interval_data)                      \
    M(473, getMCS, apple80211_mcs_data)                                            \
    M(474, getPOWER_DEBUG_INFO, apple80211_power_debug_info)                       \
    M(475, getHT_CAPABILITY, apple80211_ht_capability)                             \
    M(476, getMCS_VHT, apple80211_mcs_vht_data)                                   \
    M(477, getCHANNELS_INFO, apple80211_channels_info)                             \
    M(478, getVHT_CAPABILITY, apple80211_vht_capability)                           \
    M(479, getROAM_PROFILE, apple80211_roam_profile_all_bands)                     \
    M(480, getCHIP_COUNTER_STATS, apple80211_chip_stats)                           \
    M(481, getDBG_GUARD_TIME_PARAMS, apple80211_dbg_guard_time_params)             \
    M(482, getLEAKY_AP_STATS_MODE, apple80211_leaky_ap_setting)                    \
    M(483, getCOUNTRY_CHANNELS, apple80211_country_channel_data)                   \
    M(484, getPRIVATE_MAC, apple80211_private_mac_data)                            \
    M(485, getRANGING_ENABLE, apple80211_ranging_enable_request_t)                 \
    M(486, getRANGING_START, apple80211_ranging_start_request_t)                   \
    M(487, getAWDL_RSDB_CAPS, apple80211_rsdb_capability)                          \
    M(488, getTKO_PARAMS, apple80211_tko_params)                                   \
    M(489, getTKO_DUMP, apple80211_tko_dump)                                       \
    M(490, getHW_SUPPORTED_CHANNELS, apple80211_sup_channel_data)                  \
    M(491, getBTCOEX_PROFILE, apple80211_btcoex_profile)                           \
    M(492, getBTCOEX_PROFILE_ACTIVE, apple80211_btcoex_profile_active_data)        \
    M(493, getTRAP_INFO, apple80211_trap_info_data)                                \
    M(494, getTHERMAL_INDEX, apple80211_thermal_index_t)                           \
    M(495, getMAX_NSS_FOR_AP, apple80211_btcoex_max_nss_for_ap_data)               \
    M(496, getBTCOEX_2G_CHAIN_DISABLE, apple80211_btcoex_2g_chain_disable)         \
    M(497, getPOWER_BUDGET, apple80211_power_budget_t)                             \
    M(498, getOFFLOAD_TCPKA_ENABLE, apple80211_offload_tcpka_enable_t)             \
    M(499, getRANGING_CAPS, apple80211_ranging_capabilities_t)                     \
    M(500, getLQM_CONFIG, apple80211_lqm_config_t)                                 \
    M(501, getTRAP_CRASHTRACER_MINI_DUMP, apple80211_trap_mini_dump_data)          \
    M(502, getBEACON_INFO, apple80211_beacon_info_t)                               \
    M(503, getCHIP_POWER_RANGE, apple80211_chip_power_limit)                       \
    M(504, getNSS, apple80211_nss_data)                                            \
    M(505, getHW_ADDR, apple80211_hw_mac_address)                                  \
    M(506, getCHIP_DIAGS, appl80211_chip_diags_data)                               \
    M(507, getHP2P_CTRL, apple80211_hp2p_ctrl)                                     \
    M(508, getBSS_BLACKLIST, bss_blacklist)                                        \
    M(509, getTXRX_CHAIN_INFO, apple80211_txrx_chain_info)                         \
    M(510, getMIMO_STATUS, apple80211_mimo_status)                                 \
    M(511, getCUR_PMK, apple80211_pmk)                                             \
    M(512, getDYNSAR_DETAIL, apple80211_dynsar_detail)                             \
    M(513, getCOUNTRY_CHANNELS_INFO, apple80211_channels_info)                     \
    M(514, getLQM_SUMMARY, apple80211_lqm_summary)                                 \
    M(515, getSLOW_WIFI_FEATURE_ENABLED, apple80211_slow_wifi_feature_enabled)     \
    M(516, getTIMESYNC_INFO, apple80211_timesync_info)                             \
    M(517, getSENSING_DATA, apple80211_sensing_data_t)                             \
    M(518, getWCL_FW_HOT_CHANNELS, apple80211_fw_hot_channels)                     \
    M(519, getWCL_LOW_LATENCY_INFO, apple80211_low_latency_info)                   \
    M(520, getWCL_BSS_INFO, apple80211_beacon_msg)                                 \
    M(521, getWCL_TRAFFIC_COUNTERS, apple80211_wcl_traffic_counters)               \
    M(522, getWCL_GET_TX_BLANKING_STATUS, unsigned int)                            \
    M(523, getHE_COUNTERS, apple80211_he_counters_ctl)                             \
    M(524, getWCL_CHANNELS_INFO, apple80211ChannelInfo)                            \
    M(525, getRSN_XE, apple80211_rsn_xe_data)                                     \
    M(526, getSIB_COEX_STATUS, apple80211_sib_coex_status)                         \
    M(527, getWIFI_BT_5G_POLICY, apple80211_wifi_bt_5g_policy_t)                   \
    M(528, getWCL_EXTENDED_BSS_INFO, apple80211_extended_bss_info)                 \
    M(529, getWCL_LOW_LATENCY_INFO_STATS, apple80211_wcl_low_latency_stats)        \
    M(530, getWCL_BGSCAN_CACHE_RESULT, apple80211_bgscan_cached_network_data_list) \
    M(531, getWCL_WNM_OFFLOAD, apple80211_wcl_wnm_offload_t)                       \
    M(532, getWIFI_NOISE_PER_ANT, apple80211_noise_per_ant_t)                      \
    M(533, getFW_CLOCK_INFO, apple80211_fw_clock_info)                             \
    M(534, getTIMESYNC_STATS, apple80211_timesync_stats)                           \
    M(535, getSYSTEM_SLEEP_CONFIG, apple80211_system_sleep_config)                 \
    M(536, setCIPHER_KEY, apple80211_key)                                          \
    M(537, setCHANNEL, apple80211_channel_data)                                    \
    M(538, setPOWERSAVE, apple80211_powersave_data)                                \
    M(539, setTXPOWER, apple80211_txpower_data)                                    \
    M(540, setRATE, apple80211_rate_data)                                          \
    M(541, setIBSS_MODE, apple80211_network_data)                                  \
    M(542, setAP_MODE, apple80211_apmode_data)                                     \
    M(543, setIE, apple80211_ie_data)                                              \
    M(544, setWOW_TEST, apple80211_wow_test_data)                                  \
    M(545, setCLEAR_PMKSA_CACHE, void)                                             \
    M(546, setVIRTUAL_IF_CREATE, apple80211_virt_if_create_data)                   \
    M(547, setHT_CAPABILITY, apple80211_ht_capability)                             \
    M(548, setOFFLOAD_ARP, apple80211_offload_arp_data)                            \
    M(549, setOFFLOAD_NDP, apple80211_offload_ndp_data)                            \
    M(550, setGAS_REQ, apple80211_gas_query_t)                                     \
    M(551, setVHT_CAPABILITY, apple80211_vht_capability)                           \
    M(552, setROAM_PROFILE, apple80211_roam_profile_all_bands)                     \
    M(553, setDBG_GUARD_TIME_PARAMS, apple80211_dbg_guard_time_params)             \
    M(554, setLEAKY_AP_STATS_MODE, apple80211_leaky_ap_setting)                    \
    M(555, setPRIVATE_MAC, apple80211_private_mac_data)                            \
    M(556, setRESET_CHIP, apple80211_reset_command)                                \
    M(557, setCRASH, apple80211_crash_command)                                     \
    M(558, setRANGING_ENABLE, apple80211_ranging_enable_request_t)                 \
    M(559, setRANGING_START, apple80211_ranging_start_request_t)                   \
    M(560, setRANGING_AUTHENTICATE, apple80211_ranging_authenticate_request_t)     \
    M(561, setTKO_PARAMS, apple80211_tko_params)                                   \
    M(562, setBTCOEX_PROFILE, apple80211_btcoex_profile)                           \
    M(563, setBTCOEX_PROFILE_ACTIVE, apple80211_btcoex_profile_active_data)        \
    M(564, setTHERMAL_INDEX, apple80211_thermal_index_t)                           \
    M(565, setBTCOEX_2G_CHAIN_DISABLE, apple80211_btcoex_2g_chain_disable)         \
    M(566, setPOWER_BUDGET, apple80211_power_budget_t)                             \
    M(567, setOFFLOAD_TCPKA_ENABLE, apple80211_offload_tcpka_enable_t)             \
    M(568, setLQM_CONFIG, apple80211_lqm_config_t)                                 \
    M(569, setDYNAMIC_RSSI_WINDOW_CONFIG, apple80211_dynamic_rssi_window_config)   \
    M(570, setUSB_HOST_NOTIFICATION, apple80211_usb_host_notification_data)        \
    M(571, setHP2P_CTRL, apple80211_hp2p_ctrl)                                     \
    M(572, setBSS_BLACKLIST, bss_blacklist)                                        \
    M(573, setSET_PROPERTY, apple80211_set_property_unserialized_data)             \
    M(574, setROAM_CACHE_UPDATE, apple80211_roam_cache_data)                       \
    M(575, setPM_MODE, apple80211_pm_mode)                                         \
    M(576, setSET_WIFI_ASSERTION_STATE, apple80211_wifi_assertion_data)            \
    M(577, setREALTIME_QOS_MSCS, apple80211_state_data)                            \
    M(578, setSENSING_ENABLE, apple80211_sensing_enable_t)                         \
    M(579, setSENSING_DISABLE, apple80211_sensing_disable_t)                       \
    M(580, set6G_MODE, apple80211_6G_mode)                                         \
    M(581, setWCL_LEAVE_NETWORK, apple80211_leave_network)                         \
    M(582, setWCL_REASSOC, apple80211_reassoc)                                     \
    M(583, setWCL_SET_ROAM_LOCK, apple80211_set_roam_lock)                         \
    M(584, setWCL_LEGACY_ROAM_PROFILE_CONFIG, apple80211_legacy_roam_profile_config) \
    M(585, setWCL_ROAM_PROFILE_CONFIG, apple80211_roam_profile_config)             \
    M(586, setWCL_ROAM_USER_CACHE, apple80211_user_roam_cache)                     \
    M(587, setWCL_SCAN_ABORT, void)                                                \
    M(588, setWCL_REAL_TIME_MODE, apple80211_wcl_real_time_mode)                   \
    M(589, setWCL_ARP_MODE, apple80211_wcl_arp_mode)                               \
    M(590, setWCL_JOIN_ABORT, void)                                                \
    M(591, setWCL_TRIGGER_CC, triggerCC)                                           \
    M(592, setWCL_SCAN_REQ, apple80211ScanRequest)                                 \
    M(593, setWCL_ASSOCIATE, apple80211AssocCandidates)                            \
    M(594, setWCL_QOS_PARAMS, apple80211_wcl_qos_params)                           \
    M(595, setWCL_LINK_UP_DONE, void)                                              \
    M(596, setWCL_SET_SCAN_HOME_AWAY_TIME, scanHomeAndAwayTime)                    \
    M(597, setVOICE_IND_STATE, apple80211_voice_ind_state)                         \
    M(598, setWCL_LINK_STATE_UPDATE, apple80211_wcl_update_link_state)             \
    M(599, setRSN_XE, apple80211_rsn_xe_data)                                     \
    M(600, setWIFI_BT_5G_POLICY, apple80211_wifi_bt_5g_policy_t)                   \
    M(601, setWCL_ULOFDMA_STATE, apple80211_wcl_ulofdma_state)                     \
    M(602, setWCL_ACTION_FRAME, apple80211_wcl_action_frame)                       \
    M(603, setGAS_ABORT, void)                                                     \
    M(604, setOS_FEATURE_FLAGS, apple80211_feature_flags)                          \
    M(605, setDHCP_RENEWAL_DATA, apple80211_dhcp_renewal_data)                     \
    M(606, setBATTERY_POWERSAVE_CONFIG, apple80211_battery_ps_config)              \
    M(607, setMIMO_CONFIG, apple80211_mimo_config)                                 \
    M(608, setWCL_CONFIG_BG_MOTIONPROFILE, apple80211_bg_motion_profile)           \
    M(609, setWCL_CONFIG_BG_NETWORK, apple80211_bg_network)                        \
    M(610, setWCL_CONFIG_BGSCAN, apple80211_bg_scan)                               \
    M(611, setWCL_CONFIG_BG_PARAMS, apple80211_bg_params)                          \
    M(612, setPOWER_PROFILE, apple80211_power_profile)                             \
    M(613, setHEARTBEAT, void)                                                     \
    M(614, setINTERFACE_SETTING, apple80211_interface_setting)                     \
    M(615, setBYPASS_TX_POWER_CAP, apple80211_bypass_tx_power_cap)                 \
    M(616, setFACETIME_WIFICALLING_PARAMS, apple80211_facetime_wificalling_params) \
    M(617, setIPV4_PARAMS, apple80211_ipv4_params)                                 \
    M(618, setWCL_WNM_OPS, apple80211_wcl_wnm_config_t)                           \
    M(619, setWCL_WNM_OFFLOAD, apple80211_wcl_wnm_offload_t)

class IO80211InfraProtocol : public IO80211InfraInterface {
    OSDeclareAbstractStructors(IO80211InfraProtocol)

public:
#define IO80211_SEQUOIA_DECLARE_INFRA_PROTOCOL_PURE(slot, method, parameter_type) \
    virtual IOReturn method(parameter_type *) = 0;
    IO80211_SEQUOIA_INFRA_PROTOCOL_METHODS(
        IO80211_SEQUOIA_DECLARE_INFRA_PROTOCOL_PURE)
#undef IO80211_SEQUOIA_DECLARE_INFRA_PROTOCOL_PURE
};

static_assert(sizeof(IO80211InfraProtocol) == 0x120,
              "IO80211InfraProtocol Sequoia ABI size mismatch");

#endif /* IO80211_SEQUOIA_INFRA_PROTOCOL_H */
