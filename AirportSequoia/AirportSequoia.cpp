//
//  AirportSequoia.cpp
//  Нативный Intel Wi-Fi драйвер для macOS Sequoia (15.x) — этап 1
//  Порт AirportItlwmV2 (Sonoma 14.4) на классы IO80211Family 15.7.4
//

#include "AirportSequoia.hpp"
#include <sys/_netstat.h>
#include <crypto/sha1.h>
#include <net80211/ieee80211_crypto.h>
#include <net80211/ieee80211_ioctl.h>
#include <net80211/ieee80211_node.h>
#include <net80211/ieee80211_priv.h>
#include <sys/CTimeout.hpp>

#include "IOPCIEDeviceWrapper.hpp"

OSDefineMetaClassAndStructors(AirportSequoia, IO80211Controller);
OSDefineMetaClassAndStructors(AirportSequoiaInfra, IO80211InfraProtocol);
OSDefineMetaClassAndStructors(CTimeout, OSObject);

// Compatibility context used by timeout.c and _mbuf.cpp.  The PCI wrapper
// enforces a single active device before publishing this pair.
IO80211WorkQueue *_fWorkloop;
IOCommandGate *_fCommandGate;

#define DRVLOG(fmt, ...)  IOLog("[AirportSequoia v" AIRPORT_SEQUOIA_VERSION "] " fmt "\n", ##__VA_ARGS__)
#define DRVERR(fmt, ...)  IOLog("[AirportSequoia ERROR] " fmt "\n", ##__VA_ARGS__)

namespace {

struct __attribute__((packed)) SequoiaWclScanSsid {
    UInt32 version;
    UInt32 length;
    UInt8 bytes[32];
    UInt8 opaque[16];
};

struct __attribute__((packed)) SequoiaWclScanRequestView {
    UInt32 version;
    UInt8 privateMac;
    UInt8 opaque005[3];
    UInt32 homeAwayTime;
    UInt32 lowPowerRetry;
    UInt32 scanType;
    ether_addr bssid;
    UInt8 opaque01a[2];
    UInt32 ssidLength;
    UInt8 ssid[32];
    UInt32 bssType;
    UInt16 flags;
    UInt16 opaque046;
    UInt32 timing0;
    UInt32 timing1;
    UInt32 timing2;
    UInt32 channelCount;
    apple80211_channel channels[400];
    UInt32 ssidCount;
    SequoiaWclScanSsid ssids[10];
    UInt32 shortSsid;
};

// IO80211Family produces 0x12-byte records from offset 0x218. The Broadcom
// consumer describes the same bytes through a shifted 0x21a view; both place
// BSSID, MLD address, and chanspec at 0x21c/0x222/0x228 + n * 0x12.
struct __attribute__((packed)) SequoiaWclAssocCandidate {
    UInt32 opaque00;
    ether_addr bssid;
    ether_addr mldAddress;
    UInt16 channel;
};

struct __attribute__((packed)) SequoiaWclAssocCandidatesView {
    UInt8 opaque000[0x0c];
    UInt16 mode;
    UInt8 opaque00e[2];
    UInt32 authLower;
    UInt32 authUpper;
    UInt32 authUpperExt;
    UInt32 ssidLength;
    UInt8 ssid[32];
    apple80211_key key;
    UInt16 rsnIeLength;
    UInt8 rsnIe[257];
    UInt32 opaque1d7;
    UInt16 opaque1db;
    UInt8 opaque1dd[0x0f];
    UInt32 passwordLength;
    UInt8 password[32];
    UInt32 allowAuths;
    UInt32 candidateCount;
    SequoiaWclAssocCandidate candidates[24];
    UInt8 opaque3c8[8];
    UInt32 bandInfoBitmap;
    UInt32 opaque3d4;
    UInt32 opaque3d8;
};

static_assert(sizeof(SequoiaWclScanSsid) == 0x38,
              "Sequoia WCL scan SSID layout mismatch");
static_assert(sizeof(SequoiaWclScanRequestView) == 0x1550,
              "Sequoia WCL scan request size mismatch");
static_assert(__offsetof(SequoiaWclScanRequestView, ssidLength) == 0x1c,
              "Sequoia WCL scan SSID length offset mismatch");
static_assert(__offsetof(SequoiaWclScanRequestView, channelCount) == 0x54,
              "Sequoia WCL scan channel count offset mismatch");
static_assert(__offsetof(SequoiaWclScanRequestView, channels) == 0x58,
              "Sequoia WCL scan channel array offset mismatch");
static_assert(__offsetof(SequoiaWclScanRequestView, ssidCount) == 0x1318,
              "Sequoia WCL scan SSID count offset mismatch");
static_assert(__offsetof(SequoiaWclScanRequestView, ssids) == 0x131c,
              "Sequoia WCL scan SSID array offset mismatch");
static_assert(__offsetof(SequoiaWclScanRequestView, shortSsid) == 0x154c,
              "Sequoia WCL scan short SSID offset mismatch");

static_assert(sizeof(apple80211_key) == 0x94,
              "Sequoia embedded apple80211_key size mismatch");
static_assert(sizeof(SequoiaWclAssocCandidate) == 0x12,
              "Sequoia WCL association candidate size mismatch");
static_assert(sizeof(SequoiaWclAssocCandidatesView) == 0x3dc,
              "Sequoia WCL association request size mismatch");
static_assert(__offsetof(SequoiaWclAssocCandidatesView, mode) == 0x0c,
              "Sequoia WCL association mode offset mismatch");
static_assert(__offsetof(SequoiaWclAssocCandidatesView, authLower) == 0x10,
              "Sequoia WCL association auth lower offset mismatch");
static_assert(__offsetof(SequoiaWclAssocCandidatesView, ssidLength) == 0x1c,
              "Sequoia WCL association SSID length offset mismatch");
static_assert(__offsetof(SequoiaWclAssocCandidatesView, key) == 0x40,
              "Sequoia WCL association key offset mismatch");
static_assert(__offsetof(SequoiaWclAssocCandidatesView, rsnIeLength) == 0xd4,
              "Sequoia WCL association RSN length offset mismatch");
static_assert(__offsetof(SequoiaWclAssocCandidatesView, passwordLength) == 0x1ec,
              "Sequoia WCL association password length offset mismatch");
static_assert(__offsetof(SequoiaWclAssocCandidatesView, allowAuths) == 0x210,
              "Sequoia WCL association allowed-auth offset mismatch");
static_assert(__offsetof(SequoiaWclAssocCandidatesView, candidateCount) == 0x214,
              "Sequoia WCL association candidate count offset mismatch");
static_assert(__offsetof(SequoiaWclAssocCandidatesView, candidates) == 0x218,
              "Sequoia WCL association candidate array offset mismatch");
static_assert(__offsetof(SequoiaWclAssocCandidatesView, bandInfoBitmap) == 0x3d0,
              "Sequoia WCL association band bitmap offset mismatch");
static_assert(__offsetof(SequoiaWclAssocCandidatesView, opaque3d4) == 0x3d4,
              "Sequoia WCL association request tail offset mismatch");
static_assert(__offsetof(SequoiaWclAssocCandidatesView, opaque3d8) == 0x3d8,
              "Sequoia WCL association request tail end offset mismatch");

} // namespace

const char* hexdump(uint8_t *buf, size_t len) {
    ssize_t str_len = len * 3 + 1;
    char *str = (char*)IOMalloc(str_len);
    if (!str)
        return nullptr;
    for (size_t i = 0; i < len; i++)
        snprintf(str + 3 * i, (len - i) * 3, "%02x ", buf[i]);
    str[MAX(str_len - 2, 0)] = 0;
    return str;
}

static int sequoiaAppleScanChannelFlags(int flags)
{
    int result = APPLE80211_C_FLAG_ACTIVE | APPLE80211_C_FLAG_20MHZ;
    if (flags & IEEE80211_CHAN_2GHZ)
        result |= APPLE80211_C_FLAG_2GHZ;
    if (flags & IEEE80211_CHAN_5GHZ)
        result |= APPLE80211_C_FLAG_5GHZ;
    return result;
}

static const int kSequoiaMinDbm = -100;

static int sequoiaAppleChannelFlags(int flags, int width)
{
    int result = 0;
    if (flags & IEEE80211_CHAN_2GHZ)
        result |= APPLE80211_C_FLAG_2GHZ;
    if (flags & IEEE80211_CHAN_5GHZ)
        result |= APPLE80211_C_FLAG_5GHZ;
    if (!(flags & IEEE80211_CHAN_PASSIVE))
        result |= APPLE80211_C_FLAG_ACTIVE;
    if (flags & IEEE80211_CHAN_DFS)
        result |= APPLE80211_C_FLAG_DFS;

    switch (width) {
        case IEEE80211_CHAN_WIDTH_80P80:
        case IEEE80211_CHAN_WIDTH_160:
            result |= APPLE80211_C_FLAG_160MHZ;
            break;
        case IEEE80211_CHAN_WIDTH_80:
            result |= APPLE80211_C_FLAG_80MHZ;
            break;
        case IEEE80211_CHAN_WIDTH_40:
            result |= APPLE80211_C_FLAG_40MHZ;
            if (flags & IEEE80211_CHAN_HT40U)
                result |= APPLE80211_C_FLAG_EXT_ABV;
            break;
        case IEEE80211_CHAN_WIDTH_20:
            result |= APPLE80211_C_FLAG_20MHZ;
            break;
        default:
            if (flags & IEEE80211_CHAN_HT20)
                result |= APPLE80211_C_FLAG_20MHZ;
            else if ((flags & IEEE80211_CHAN_CCK) ||
                     (flags & IEEE80211_CHAN_OFDM))
                result |= APPLE80211_C_FLAG_10MHZ;
            break;
    }
    return result;
}

static IOReturn
sequoiaValidateRsnSecurity(const UInt8 *ie, UInt16 length)
{
    if (!ie || length < 2 || ie[0] != IEEE80211_ELEMID_RSN ||
        (UInt32)ie[1] + 2 != length)
        return kIOReturnBadArgument;

    UInt32 cursor = 2;
    const auto hasBytes = [length](UInt32 offset, UInt32 count) {
        return offset <= length && count <= (UInt32)length - offset;
    };
    const auto readLittle16 = [](const UInt8 *bytes) {
        return (UInt16)(bytes[0] | ((UInt16)bytes[1] << 8));
    };

    if (!hasBytes(cursor, 2) || readLittle16(ie + cursor) != 1)
        return kIOReturnBadArgument;
    cursor += 2;
    if (!hasBytes(cursor, 4))
        return kIOReturnBadArgument;
    cursor += 4; // group data cipher suite

    if (!hasBytes(cursor, 2))
        return kIOReturnBadArgument;
    UInt16 pairwiseCount = readLittle16(ie + cursor);
    cursor += 2;
    if (pairwiseCount == 0 ||
        pairwiseCount > ((UInt32)length - cursor) / 4)
        return kIOReturnBadArgument;
    cursor += (UInt32)pairwiseCount * 4;

    if (!hasBytes(cursor, 2))
        return kIOReturnBadArgument;
    UInt16 akmCount = readLittle16(ie + cursor);
    cursor += 2;
    if (akmCount == 0 || akmCount > ((UInt32)length - cursor) / 4)
        return kIOReturnBadArgument;
    cursor += (UInt32)akmCount * 4;

    if (cursor == length)
        return kIOReturnSuccess;
    if (!hasBytes(cursor, 2))
        return kIOReturnBadArgument;
    UInt16 capabilities = readLittle16(ie + cursor);
    cursor += 2;
    if (capabilities & (IEEE80211_RSNCAP_MFPC | IEEE80211_RSNCAP_MFPR))
        return kIOReturnUnsupported;

    if (cursor == length)
        return kIOReturnSuccess;
    if (!hasBytes(cursor, 2))
        return kIOReturnBadArgument;
    UInt16 pmkidCount = readLittle16(ie + cursor);
    cursor += 2;
    if (pmkidCount > ((UInt32)length - cursor) / IEEE80211_PMKID_LEN)
        return kIOReturnBadArgument;
    cursor += (UInt32)pmkidCount * IEEE80211_PMKID_LEN;

    if (cursor == length)
        return kIOReturnSuccess;
    // Any well-formed trailing suite is the group-management cipher used by
    // PMF. This bridge has no IGTK/BIP installation path yet.
    if ((UInt32)length - cursor == 4)
        return kIOReturnUnsupported;
    return kIOReturnBadArgument;
}

// ---------------------------------------------------------------------------
// AirportSequoiaInfra
// ---------------------------------------------------------------------------

bool AirportSequoiaInfra::init(IOService *provider)
{
    if (!IO80211InfraProtocol::init()) {
        DRVERR("IO80211InfraProtocol init failed\n");
        return false;
    }
    instance = OSDynamicCast(AirportSequoia, provider);
    if (!instance)
        return false;
    this->fHalService = instance->fHalService;
    return true;
}

void AirportSequoiaInfra::free()
{
    DRVLOG("infra free");
    IO80211InfraProtocol::free();
}

IOReturn AirportSequoiaInfra::setMacAddress(ether_addr &address)
{
    if (!instance)
        return kIOReturnNotReady;
    return instance->setHardwareAddress(&address, ETHER_ADDR_LEN);
}

#define IO80211_SEQUOIA_DEFINE_INFRA_STUB(slot, method, parameter_type) \
    IOReturn AirportSequoiaInfra::method(parameter_type *data)          \
    {                                                                   \
        return dispatchProtocol(method##Tag(), data);                   \
    }
IO80211_SEQUOIA_INFRA_PROTOCOL_METHODS(IO80211_SEQUOIA_DEFINE_INFRA_STUB)
#undef IO80211_SEQUOIA_DEFINE_INFRA_STUB

IOReturn AirportSequoiaInfra::
dispatchProtocol(setWCL_SCAN_REQTag, apple80211ScanRequest *data)
{
    if (!data)
        return kIOReturnBadArgument;
    AirportSequoia *controller = instance;
    if (!controller)
        return kIOReturnNotReady;

    controller->retain();
    retain();
    IOCommandGate *gate = controller->retainMainCommandGate();
    if (!gate) {
        release();
        controller->release();
        return kIOReturnNotReady;
    }
    IOReturn result =
        gate->runAction(handleWclScanGated, this, data);
    gate->release();
    release();
    controller->release();
    return result;
}

IOReturn AirportSequoiaInfra::
dispatchProtocol(setWCL_ASSOCIATETag, apple80211AssocCandidates *data)
{
    if (!data)
        return kIOReturnBadArgument;
    AirportSequoia *controller = instance;
    if (!controller)
        return kIOReturnNotReady;

    controller->retain();
    retain();
    IOCommandGate *gate = controller->retainMainCommandGate();
    if (!gate) {
        release();
        controller->release();
        return kIOReturnNotReady;
    }
    IOReturn result =
        gate->runAction(handleWclAssociateGated, this, data);
    gate->release();
    release();
    controller->release();
    return result;
}

IOReturn AirportSequoiaInfra::
handleWclScanGated(OSObject *owner, void *arg0, void *arg1, void *arg2,
                   void *arg3)
{
    (void)arg2;
    (void)arg3;
    AirportSequoia *controller = OSDynamicCast(AirportSequoia, owner);
    AirportSequoiaInfra *interface =
        OSDynamicCast(AirportSequoiaInfra, (OSObject *)arg0);
    if (!controller || !interface || interface->instance != controller ||
        controller->isStopping())
        return kIOReturnNotReady;
    return interface->handleProtocol(
        setWCL_SCAN_REQTag(), (apple80211ScanRequest *)arg1);
}

IOReturn AirportSequoiaInfra::
handleWclAssociateGated(OSObject *owner, void *arg0, void *arg1, void *arg2,
                        void *arg3)
{
    (void)arg2;
    (void)arg3;
    AirportSequoia *controller = OSDynamicCast(AirportSequoia, owner);
    AirportSequoiaInfra *interface =
        OSDynamicCast(AirportSequoiaInfra, (OSObject *)arg0);
    if (!controller || !interface || interface->instance != controller ||
        controller->isStopping())
        return kIOReturnNotReady;
    return interface->handleProtocol(
        setWCL_ASSOCIATETag(), (apple80211AssocCandidates *)arg1);
}

IOReturn AirportSequoiaInfra::
associateSSID(const UInt8 *ssid, UInt32 ssidLength,
              const ether_addr &bssid, UInt32 authLower,
              UInt32 authUpper, const UInt8 *key, UInt32 keyLength,
              UInt16 keyIndex)
{
    ieee80211com *ic = fHalService ? fHalService->get80211Controller() : NULL;
    if (!ic || !ssid || ssidLength == 0 ||
        ssidLength > sizeof(ic->ic_des_essid) ||
        keyLength > APPLE80211_KEY_BUFF_LEN ||
        (keyLength != 0 && !key))
        return kIOReturnBadArgument;

    const UInt32 wpa3AuthTypes =
        APPLE80211_AUTHTYPE_WPA3_SAE |
        APPLE80211_AUTHTYPE_WPA3_FT_SAE |
        APPLE80211_AUTHTYPE_WPA3_ENTERPRISE |
        APPLE80211_AUTHTYPE_WPA3_FT_ENTERPRISE;
    const UInt32 supportedAuthTypes =
        APPLE80211_AUTHTYPE_WPA |
        APPLE80211_AUTHTYPE_WPA_PSK |
        APPLE80211_AUTHTYPE_WPA2 |
        APPLE80211_AUTHTYPE_WPA2_PSK |
        APPLE80211_AUTHTYPE_SHA256_PSK |
        APPLE80211_AUTHTYPE_SHA256_8021X;
    if (authLower != APPLE80211_AUTHTYPE_OPEN ||
        (authUpper & wpa3AuthTypes) != 0 ||
        (authUpper & ~supportedAuthTypes) != 0)
        return kIOReturnUnsupported;

    const UInt32 pskAuthTypes =
        APPLE80211_AUTHTYPE_WPA_PSK |
        APPLE80211_AUTHTYPE_WPA2_PSK |
        APPLE80211_AUTHTYPE_SHA256_PSK;
    const bool usesPsk = (authUpper & pskAuthTypes) != 0;
    const bool usesWep =
        authUpper == APPLE80211_AUTHTYPE_NONE &&
        authLower == APPLE80211_AUTHTYPE_OPEN && keyLength != 0;

    if (usesPsk && keyLength != sizeof(ic->ic_psk))
        return kIOReturnBadArgument;
    if (usesWep &&
        (keyIndex >= IEEE80211_WEP_NKID ||
         (keyLength != 5 && keyLength != 13)))
        return kIOReturnBadArgument;
    if (!usesPsk && !usesWep && keyLength != 0)
        return kIOReturnBadArgument;

    ieee80211_wpaparams wpa;
    ieee80211_nwkey nwkey;
    bzero(&wpa, sizeof(wpa));
    bzero(&nwkey, sizeof(nwkey));

    ieee80211_disable_rsn(ic);
    ieee80211_disable_wep(ic);

    bzero(ic->ic_des_essid, sizeof(ic->ic_des_essid));
    memcpy(ic->ic_des_essid, ssid, ssidLength);
    ic->ic_des_esslen = ssidLength;
    ic->ic_flags &= ~IEEE80211_F_AUTO_JOIN;

    bool zeroBssid = true;
    for (UInt i = 0; i < ETHER_ADDR_LEN; i++)
        zeroBssid &= bssid.octet[i] == 0;
    if (zeroBssid) {
        bzero(ic->ic_des_bssid, sizeof(ic->ic_des_bssid));
        ic->ic_flags &= ~IEEE80211_F_DESBSSID;
    } else {
        IEEE80211_ADDR_COPY(ic->ic_des_bssid, bssid.octet);
        ic->ic_flags |= IEEE80211_F_DESBSSID;
    }

    if (authUpper &
        (APPLE80211_AUTHTYPE_WPA | APPLE80211_AUTHTYPE_WPA_PSK))
        wpa.i_protos |= IEEE80211_WPA_PROTO_WPA1;
    if (authUpper &
        (APPLE80211_AUTHTYPE_WPA2 |
         APPLE80211_AUTHTYPE_WPA2_PSK |
         APPLE80211_AUTHTYPE_SHA256_PSK |
         APPLE80211_AUTHTYPE_SHA256_8021X))
        wpa.i_protos |= IEEE80211_WPA_PROTO_WPA2;

    if (usesPsk) {
        if (authUpper & (APPLE80211_AUTHTYPE_WPA_PSK |
                         APPLE80211_AUTHTYPE_WPA2_PSK))
            wpa.i_akms |= IEEE80211_WPA_AKM_PSK;
        if (authUpper & APPLE80211_AUTHTYPE_SHA256_PSK)
            wpa.i_akms |= IEEE80211_WPA_AKM_SHA256_PSK;
        wpa.i_enabled = 1;
        memcpy(ic->ic_psk, key, sizeof(ic->ic_psk));
        ic->ic_flags |= IEEE80211_F_PSK;
    }

    const UInt32 enterpriseAuthTypes =
        APPLE80211_AUTHTYPE_WPA |
        APPLE80211_AUTHTYPE_WPA2 |
        APPLE80211_AUTHTYPE_SHA256_8021X;
    if (authUpper & enterpriseAuthTypes) {
        if (authUpper &
            (APPLE80211_AUTHTYPE_WPA | APPLE80211_AUTHTYPE_WPA2))
            wpa.i_akms |= IEEE80211_WPA_AKM_8021X;
        if (authUpper & APPLE80211_AUTHTYPE_SHA256_8021X)
            wpa.i_akms |= IEEE80211_WPA_AKM_SHA256_8021X;
        wpa.i_enabled = 1;
    }

    if (wpa.i_enabled) {
        int error = ieee80211_ioctl_setwpaparms(ic, &wpa);
        if (error != 0 && error != ENETRESET)
            return kIOReturnIOError;
    }

    if (usesWep) {
        nwkey.i_wepon = IEEE80211_NWKEY_WEP;
        nwkey.i_defkid = keyIndex + 1;
        nwkey.i_key[keyIndex].i_keylen = keyLength;
        nwkey.i_key[keyIndex].i_keydat = const_cast<UInt8 *>(key);
        int error = ieee80211_ioctl_setnwkeys(ic, &nwkey);
        if (error != 0 && error != ENETRESET)
            return kIOReturnIOError;
    }

    return kIOReturnSuccess;
}

IOReturn AirportSequoiaInfra::
handleProtocol(setWCL_SCAN_REQTag, apple80211ScanRequest *data)
{
    if (!data)
        return kIOReturnBadArgument;
    if (!instance || !fHalService)
        return kIOReturnNotReady;

    ieee80211com *ic = fHalService->get80211Controller();
    if (!ic || ic->ic_state <= IEEE80211_S_INIT)
        return kIOReturnNotReady;
    if (ic->ic_state == IEEE80211_S_AUTH ||
        ic->ic_state == IEEE80211_S_ASSOC)
        return kIOReturnBusy;

    const SequoiaWclScanRequestView *request =
        reinterpret_cast<const SequoiaWclScanRequestView *>(data);
    if (request->ssidLength > sizeof(request->ssid) ||
        request->channelCount > ARRAY_SIZE(request->channels) ||
        request->ssidCount > ARRAY_SIZE(request->ssids))
        return kIOReturnBadArgument;
    for (UInt32 i = 0; i < request->ssidCount; i++) {
        if (request->ssids[i].length > sizeof(request->ssids[i].bytes))
            return kIOReturnBadArgument;
    }
    bool zeroBssid = true;
    for (UInt index = 0; index < ETHER_ADDR_LEN; index++)
        zeroBssid &= request->bssid.octet[index] == 0;

    // The current net80211 scan API has no per-request channel/SSID allow-list
    // and no private-MAC hook. Reject those semantics rather than silently
    // broadening the request or leaking the permanent address.
    if (request->privateMac || request->channelCount != 0 ||
        request->ssidLength != 0 || request->ssidCount != 0 ||
        request->shortSsid != 0 || !zeroBssid)
        return kIOReturnUnsupported;
    if (request->bssType == APPLE80211_AP_MODE_IBSS)
        return kIOReturnUnsupported;
    // net80211's generic begin-scan path may send probes, so it cannot honor
    // WCL's explicit passive-scan contract. Fail closed until that route has a
    // real passive implementation.
    if (request->scanType != APPLE80211_SCAN_TYPE_NONE &&
        request->scanType != APPLE80211_SCAN_TYPE_BACKGROUND)
        return kIOReturnUnsupported;
    if (request->scanType == APPLE80211_SCAN_TYPE_BACKGROUND &&
        ic->ic_state != IEEE80211_S_RUN)
        return kIOReturnUnsupported;
    if (ic->ic_state != IEEE80211_S_RUN && ic->ic_des_esslen != 0)
        return kIOReturnBusy;

    if (instance->isWclScanPending() ||
        (ic->ic_flags & IEEE80211_F_BGSCAN))
        return kIOReturnBusy;
    if (ic->ic_flags & IEEE80211_F_ASCAN) {
        // HAL enable starts an unfiltered active scan before WCL arrives. If
        // it has no desired ESS yet, this request is semantically identical:
        // attach the WCL completion token instead of deadlocking slot 592.
        if (ic->ic_state == IEEE80211_S_SCAN &&
            ic->ic_des_esslen == 0 &&
            request->scanType == APPLE80211_SCAN_TYPE_NONE) {
            return instance->claimWclScanToken() ?
                kIOReturnSuccess : kIOReturnBusy;
        }
        return kIOReturnBusy;
    }

    // This marks only acceptance into net80211. Completion is posted exactly
    // once when the corresponding real SCAN_DONE event arrives.
    if (!instance->claimWclScanToken())
        return kIOReturnBusy;
    if (ic->ic_state == IEEE80211_S_RUN) {
        ieee80211_begin_cache_bgscan(&ic->ic_ac.ac_if);
        if (ic->ic_flags & IEEE80211_F_BGSCAN)
            return kIOReturnSuccess;
        instance->clearWclScanToken();
        return kIOReturnNotReady;
    }

    ieee80211_begin_scan(&ic->ic_ac.ac_if);
    if (ic->ic_flags & IEEE80211_F_ASCAN)
        return kIOReturnSuccess;
    instance->clearWclScanToken();
    return kIOReturnIOError;
}

IOReturn AirportSequoiaInfra::
handleProtocol(setWCL_ASSOCIATETag, apple80211AssocCandidates *data)
{
    if (!data)
        return kIOReturnBadArgument;
    if (!instance || !fHalService)
        return kIOReturnNotReady;

    ieee80211com *ic = fHalService->get80211Controller();
    if (!ic || ic->ic_state < IEEE80211_S_SCAN)
        return kIOReturnNotReady;
    if (ic->ic_state == IEEE80211_S_AUTH ||
        ic->ic_state == IEEE80211_S_ASSOC ||
        instance->isWclScanPending() ||
        (ic->ic_flags & IEEE80211_F_BGSCAN))
        return kIOReturnBusy;
    if ((ic->ic_flags & IEEE80211_F_ASCAN) &&
        (ic->ic_state != IEEE80211_S_SCAN || ic->ic_des_esslen != 0))
        return kIOReturnBusy;

    const SequoiaWclAssocCandidatesView *request =
        reinterpret_cast<const SequoiaWclAssocCandidatesView *>(data);
    if (request->mode == APPLE80211_AP_MODE_IBSS)
        return kIOReturnUnsupported;
    // authUpperExt selects alternate security routes (including OWE), and bit
    // 0x40000000 in allowAuths changes the PMF policy. Neither route is
    // complete in this bridge, so never reinterpret it as ordinary open/WPA.
    if (request->authUpperExt != 0 ||
        (request->allowAuths & 0x40000000U) != 0)
        return kIOReturnUnsupported;
    if (request->ssidLength == 0 ||
        request->ssidLength > sizeof(request->ssid) ||
        request->rsnIeLength > sizeof(request->rsnIe) ||
        request->passwordLength > sizeof(request->password) ||
        request->candidateCount > ARRAY_SIZE(request->candidates) ||
        request->key.key_len > sizeof(request->key.key) ||
        request->key.key_rsc_len > sizeof(request->key.key_rsc) ||
        request->key.wowl_kck_len > sizeof(request->key.wowl_kck_key) ||
        request->key.wowl_kek_len > sizeof(request->key.wowl_kek_key))
        return kIOReturnBadArgument;
    if (request->candidateCount > 1)
        return kIOReturnUnsupported;
    if (request->key.wowl_kck_len != 0 ||
        request->key.wowl_kek_len != 0)
        return kIOReturnUnsupported;

    if (request->rsnIeLength != 0) {
        if (request->rsnIeLength < 2 ||
            (UInt32)request->rsnIe[1] + 2 != request->rsnIeLength)
            return kIOReturnBadArgument;
        if (request->rsnIe[0] == IEEE80211_ELEMID_VENDOR) {
            static const UInt8 wpaOuiType[] = {0x00, 0x50, 0xf2, 0x01};
            if (request->rsnIeLength < 6 ||
                memcmp(request->rsnIe + 2, wpaOuiType,
                       sizeof(wpaOuiType)) != 0)
                return kIOReturnBadArgument;
        } else if (request->rsnIe[0] == IEEE80211_ELEMID_RSN) {
            IOReturn validation =
                sequoiaValidateRsnSecurity(request->rsnIe,
                                           request->rsnIeLength);
            if (validation != kIOReturnSuccess)
                return validation;
        } else {
            return kIOReturnBadArgument;
        }
    }

    ether_addr desiredBssid = {};
    if (request->candidateCount == 1) {
        const ether_addr &candidateBssid = request->candidates[0].bssid;
        bool zeroBssid = true;
        for (UInt index = 0; index < ETHER_ADDR_LEN; index++)
            zeroBssid &= candidateBssid.octet[index] == 0;
        if (zeroBssid || IEEE80211_IS_MULTICAST(candidateBssid.octet))
            return kIOReturnBadArgument;
        bool zeroMldAddress = true;
        for (UInt index = 0; index < ETHER_ADDR_LEN; index++)
            zeroMldAddress &=
                request->candidates[0].mldAddress.octet[index] == 0;
        if (!zeroMldAddress)
            return kIOReturnUnsupported;
        memcpy(desiredBssid.octet, candidateBssid.octet, ETHER_ADDR_LEN);
    }

    bool zeroKeyAddress = true;
    for (UInt index = 0; index < ETHER_ADDR_LEN; index++)
        zeroKeyAddress &= request->key.key_ea.octet[index] == 0;
    if (!zeroKeyAddress &&
        (request->candidateCount != 1 ||
         memcmp(request->key.key_ea.octet, desiredBssid.octet,
                ETHER_ADDR_LEN) != 0))
        return kIOReturnBadArgument;

#ifdef USE_APPLE_SUPPLICANT
    if (request->rsnIeLength > sizeof(ic->ic_rsn_ie_override))
        return kIOReturnBadArgument;
#else
    if (request->rsnIeLength)
        return kIOReturnUnsupported;
#endif

    const UInt32 pskAuthTypes =
        APPLE80211_AUTHTYPE_WPA_PSK |
        APPLE80211_AUTHTYPE_WPA2_PSK |
        APPLE80211_AUTHTYPE_SHA256_PSK;
    const bool usesPskAuth = (request->authUpper & pskAuthTypes) != 0;
    const bool openAuth =
        request->authLower == APPLE80211_AUTHTYPE_OPEN &&
        request->authUpper == APPLE80211_AUTHTYPE_NONE;
    const UInt8 *associationKey = NULL;
    UInt32 associationKeyLength = 0;
    UInt8 derivedPsk[IEEE80211_PMK_LEN];
    bool hasDerivedPsk = false;
    switch (request->key.key_cipher_type) {
        case APPLE80211_CIPHER_NONE:
            if (request->key.key_len != 0 ||
                request->passwordLength != 0 || usesPskAuth)
                return kIOReturnBadArgument;
            break;
        case APPLE80211_CIPHER_WEP_40:
        case APPLE80211_CIPHER_WEP_104: {
            const UInt32 expectedLength =
                request->key.key_cipher_type == APPLE80211_CIPHER_WEP_40 ?
                    5 : 13;
            if (!openAuth || request->passwordLength != 0 ||
                request->key.key_len != expectedLength)
                return kIOReturnBadArgument;
            associationKey = request->key.key;
            associationKeyLength = request->key.key_len;
            break;
        }
        case APPLE80211_CIPHER_PMK:
        case APPLE80211_CIPHER_PMKSA:
            if (!usesPskAuth || request->passwordLength != 0 ||
                request->key.key_len != IEEE80211_PMK_LEN)
                return kIOReturnBadArgument;
            associationKey = request->key.key;
            associationKeyLength = request->key.key_len;
            break;
        case APPLE80211_CIPHER_PWD: {
            if (!usesPskAuth || request->key.key_len != 0 ||
                request->passwordLength < 8 ||
                memchr(request->password, '\0',
                       request->passwordLength) != NULL)
                return kIOReturnBadArgument;
            char passphrase[sizeof(request->password) + 1];
            memcpy(passphrase, request->password, request->passwordLength);
            passphrase[request->passwordLength] = '\0';
            if (pbkdf2_sha1(passphrase, request->ssid,
                            request->ssidLength, 4096, derivedPsk,
                            sizeof(derivedPsk)) != 0) {
                explicit_bzero(passphrase, sizeof(passphrase));
                return kIOReturnIOError;
            }
            explicit_bzero(passphrase, sizeof(passphrase));
            associationKey = derivedPsk;
            associationKeyLength = sizeof(derivedPsk);
            hasDerivedPsk = true;
            break;
        }
        default:
            return kIOReturnUnsupported;
    }

    IOReturn result =
        associateSSID(request->ssid, request->ssidLength, desiredBssid,
                      request->authLower, request->authUpper,
                      associationKey, associationKeyLength,
                      request->key.key_index);
    if (hasDerivedPsk)
        explicit_bzero(derivedPsk, sizeof(derivedPsk));
    if (result != kIOReturnSuccess)
        return result;

#ifdef USE_APPLE_SUPPLICANT
    bzero(ic->ic_rsn_ie_override, sizeof(ic->ic_rsn_ie_override));
    if (request->rsnIeLength)
        memcpy(ic->ic_rsn_ie_override, request->rsnIe,
               request->rsnIeLength);
#endif

    // Reuse the HAL's initial active scan after installing the desired
    // SSID/BSSID. Starting a second scan while ASCAN is set can otherwise
    // leave the cold-start route permanently busy.
    if (ic->ic_flags & IEEE80211_F_ASCAN)
        return kIOReturnSuccess;
    ieee80211_begin_scan(&ic->ic_ac.ac_if);
    return (ic->ic_flags & IEEE80211_F_ASCAN) ?
        kIOReturnSuccess : kIOReturnIOError;
}

IOReturn AirportSequoiaInfra::
handleProtocol(setWCL_LINK_UP_DONETag, void *data)
{
    (void)data;
    // WCL emits this after IP configuration. The native Broadcom route only
    // applies vendor-specific radio power tuning here; Intel has no equivalent.
    // Security state was already finalized by the successful GTK callback.
    DRVLOG("WCL_LINK_UP_DONE post-IP");
    return kIOReturnSuccess;
}

IOReturn AirportSequoiaInfra::
handleProtocol(getCHANNELTag, apple80211_channel_data *data)
{
    if (!data || !fHalService)
        return kIOReturnBadArgument;
    ieee80211com *ic = fHalService->get80211Controller();
    if (!ic || ic->ic_state != IEEE80211_S_RUN || !ic->ic_bss ||
        !ic->ic_bss->ni_chan)
        return kIOReturnNotReady;
    bzero(data, sizeof(*data));
    data->version = APPLE80211_VERSION;
    data->channel.version = APPLE80211_VERSION;
    data->channel.channel = ieee80211_chan2ieee(ic, ic->ic_bss->ni_chan);
    data->channel.flags =
        sequoiaAppleChannelFlags(ic->ic_bss->ni_chan->ic_flags,
                                 ic->ic_bss->ni_chw);
    return kIOReturnSuccess;
}

IOReturn AirportSequoiaInfra::
handleProtocol(getPOWERSAVETag, apple80211_powersave_data *data)
{
    if (!data)
        return kIOReturnBadArgument;
    bzero(data, sizeof(*data));
    data->version = APPLE80211_VERSION;
    data->powersave_level = APPLE80211_POWERSAVE_MODE_DISABLED;
    return kIOReturnSuccess;
}

IOReturn AirportSequoiaInfra::
handleProtocol(getTXPOWERTag, apple80211_txpower_data *data)
{
    if (!data || !fHalService)
        return kIOReturnBadArgument;
    ieee80211com *ic = fHalService->get80211Controller();
    if (!ic || ic->ic_state != IEEE80211_S_RUN)
        return kIOReturnNotReady;
    bzero(data, sizeof(*data));
    data->version = APPLE80211_VERSION;
    data->txpower = ic->ic_txpower;
    data->txpower_unit = APPLE80211_UNIT_PERCENT;
    return kIOReturnSuccess;
}

IOReturn AirportSequoiaInfra::
handleProtocol(getRATETag, apple80211_rate_data *data)
{
    if (!data || !fHalService)
        return kIOReturnBadArgument;
    ieee80211com *ic = fHalService->get80211Controller();
    if (!ic || ic->ic_state != IEEE80211_S_RUN || !ic->ic_bss)
        return kIOReturnNotReady;

    bzero(data, sizeof(*data));
    data->version = APPLE80211_VERSION;
    data->num_radios = 1;
    int index = 0;
    bool shortGI = ieee80211_node_supports_sgi(ic->ic_bss);
    if (ic->ic_curmode == IEEE80211_MODE_11AC) {
        if (shortGI)
            index++;
        int nss = fHalService->getDriverInfo()->getTxNSS();
        if (nss < 1 || nss > 4)
            return kIOReturnError;
        switch (ic->ic_bss->ni_chw) {
            case IEEE80211_CHAN_WIDTH_40: index += 4; break;
            case IEEE80211_CHAN_WIDTH_80: index += 8; break;
            case IEEE80211_CHAN_WIDTH_80P80:
            case IEEE80211_CHAN_WIDTH_160: index += 12; break;
            default: break;
        }
        index += 2 * (nss - 1);
        const ieee80211_vht_rateset *rates = &ieee80211_std_ratesets_11ac[index];
        if (!rates->nrates)
            return kIOReturnError;
        data->rate[0] =
            rates->rates[ic->ic_bss->ni_txmcs % rates->nrates] / 2;
    } else if (ic->ic_curmode == IEEE80211_MODE_11N) {
        if (shortGI)
            index++;
        if (ic->ic_bss->ni_chw == IEEE80211_CHAN_WIDTH_40)
            index += IEEE80211_HT_RATESET_MIMO4_SGI + 1;
        index += ic->ic_bss->ni_txmcs / 16;
        int nss = ic->ic_bss->ni_txmcs / 8 + 1;
        if (nss < 1 || nss > 4)
            return kIOReturnError;
        index += 2 * (nss - 1);
        data->rate[0] =
            ieee80211_std_ratesets_11n[index]
                .rates[ic->ic_bss->ni_txmcs % 8] / 2;
    } else {
        if (ic->ic_bss->ni_txrate >= ic->ic_bss->ni_rates.rs_nrates)
            return kIOReturnError;
        data->rate[0] =
            ic->ic_bss->ni_rates.rs_rates[ic->ic_bss->ni_txrate];
    }
    return kIOReturnSuccess;
}

IOReturn AirportSequoiaInfra::
handleProtocol(getOP_MODETag, apple80211_opmode_data *data)
{
    if (!data)
        return kIOReturnBadArgument;
    bzero(data, sizeof(*data));
    data->version = APPLE80211_VERSION;
    data->op_mode = APPLE80211_M_STA;
    return kIOReturnSuccess;
}

IOReturn AirportSequoiaInfra::
handleProtocol(getRSSITag, apple80211_rssi_data *data)
{
    if (!data || !fHalService)
        return kIOReturnBadArgument;
    ieee80211com *ic = fHalService->get80211Controller();
    if (!ic || ic->ic_state != IEEE80211_S_RUN || !ic->ic_bss)
        return kIOReturnNotReady;
    bzero(data, sizeof(*data));
    data->version = APPLE80211_VERSION;
    data->num_radios = 1;
    data->rssi_unit = APPLE80211_UNIT_DBM;
    int rssi = kSequoiaMinDbm + ic->ic_bss->ni_rssi;
    data->rssi[0] = data->aggregate_rssi =
        data->rssi_ext[0] = data->aggregate_rssi_ext = rssi;
    return kIOReturnSuccess;
}

IOReturn AirportSequoiaInfra::
handleProtocol(getSUPPORTED_CHANNELSTag, apple80211_sup_channel_data *data)
{
    if (!data || !fHalService)
        return kIOReturnBadArgument;
    ieee80211com *ic = fHalService->get80211Controller();
    if (!ic)
        return kIOReturnNotReady;
    bzero(data, sizeof(*data));
    data->version = APPLE80211_VERSION;
    for (int i = 0;
         i < IEEE80211_CHAN_MAX &&
         data->num_channels < ARRAY_SIZE(data->supported_channels);
         i++) {
        if (!ic->ic_channels[i].ic_freq)
            continue;
        apple80211_channel &channel =
            data->supported_channels[data->num_channels++];
        channel.version = APPLE80211_VERSION;
        channel.channel = ieee80211_chan2ieee(ic, &ic->ic_channels[i]);
        channel.flags =
            sequoiaAppleScanChannelFlags(ic->ic_channels[i].ic_flags);
    }
    return kIOReturnSuccess;
}

IOReturn AirportSequoiaInfra::
handleProtocol(getMCSTag, apple80211_mcs_data *data)
{
    if (!data || !fHalService)
        return kIOReturnBadArgument;
    ieee80211com *ic = fHalService->get80211Controller();
    if (!ic || ic->ic_state != IEEE80211_S_RUN || !ic->ic_bss)
        return kIOReturnNotReady;
    bzero(data, sizeof(*data));
    data->version = APPLE80211_VERSION;
    data->index = ic->ic_bss->ni_txmcs;
    return kIOReturnSuccess;
}

IOReturn AirportSequoiaInfra::
handleProtocol(getMCS_VHTTag, apple80211_mcs_vht_data *data)
{
    if (!data || !fHalService)
        return kIOReturnBadArgument;
    ieee80211com *ic = fHalService->get80211Controller();
    if (!ic || !ic->ic_bss || ic->ic_curmode < IEEE80211_MODE_11AC)
        return kIOReturnNotReady;
    bzero(data, sizeof(*data));
    data->version = APPLE80211_VERSION;
    data->guard_interval =
        (ieee80211_node_supports_vht_sgi80(ic->ic_bss) ||
         ieee80211_node_supports_vht_sgi160(ic->ic_bss))
            ? APPLE80211_GI_SHORT
            : APPLE80211_GI_LONG;
    data->index = ic->ic_bss->ni_txmcs;
    data->nss = fHalService->getDriverInfo()->getTxNSS();
    switch (ic->ic_bss->ni_chw) {
        case IEEE80211_CHAN_WIDTH_40: data->bw = 40; break;
        case IEEE80211_CHAN_WIDTH_80: data->bw = 80; break;
        case IEEE80211_CHAN_WIDTH_80P80:
        case IEEE80211_CHAN_WIDTH_160: data->bw = 160; break;
        default: data->bw = 20; break;
    }
    return kIOReturnSuccess;
}

IOReturn AirportSequoiaInfra::
handleProtocol(getNSSTag, apple80211_nss_data *data)
{
    if (!data || !fHalService || !fHalService->getDriverInfo())
        return kIOReturnBadArgument;
    bzero(data, sizeof(*data));
    data->version = APPLE80211_VERSION;
    data->nss = fHalService->getDriverInfo()->getTxNSS();
    return kIOReturnSuccess;
}

IOReturn AirportSequoiaInfra::removeKey(const apple80211_key *key)
{
    if (!key || !fHalService)
        return kIOReturnBadArgument;
    ieee80211com *ic = fHalService->get80211Controller();
    if (!ic || !ic->ic_delete_key)
        return kIOReturnNotReady;

    bool unicast =
        (key->key_flags & APPLE80211_KEY_FLAG_UNICAST) != 0;
    bool multicast =
        (key->key_flags & APPLE80211_KEY_FLAG_MULTICAST) != 0;
    if (!unicast && !multicast) {
        // Preserve the legacy Apple convention: TX-only identifies the PTK,
        // while zero/RX-only flags identify a group key.
        unicast = (key->key_flags & APPLE80211_KEY_FLAG_TX) != 0;
        multicast = !unicast;
    }
    if (multicast && key->key_index >= ARRAY_SIZE(ic->ic_nw_keys))
        return kIOReturnBadArgument;

    if (unicast) {
        ieee80211_node *node = ic->ic_bss;
        if (!node)
            return kIOReturnNotReady;
        ieee80211_key *target = &node->ni_pairwise_key;
        if (target->k_cipher != IEEE80211_CIPHER_NONE)
            (*ic->ic_delete_key)(ic, node, target);
        explicit_bzero(target, sizeof(*target));
        node->ni_flags &= ~(IEEE80211_NODE_TXRXPROT |
                            IEEE80211_NODE_RXPROT);
        node->ni_port_valid = 0;
        node->ni_rsn_supp_state = RSNA_SUPP_INITIALIZE;
    }

    if (multicast) {
        ieee80211_key *target = &ic->ic_nw_keys[key->key_index];
        if (target->k_cipher != IEEE80211_CIPHER_NONE)
            (*ic->ic_delete_key)(ic, NULL, target);
        explicit_bzero(target, sizeof(*target));

        bool hasGroupDataKey = false;
        for (size_t index = 0; index < ARRAY_SIZE(ic->ic_nw_keys); index++) {
            const ieee80211_key *candidate = &ic->ic_nw_keys[index];
            if (candidate->k_cipher != IEEE80211_CIPHER_NONE &&
                (candidate->k_flags & IEEE80211_KEY_GROUP) != 0 &&
                (candidate->k_flags & IEEE80211_KEY_IGTK) == 0) {
                hasGroupDataKey = true;
                break;
            }
        }
        if (!hasGroupDataKey && ic->ic_bss) {
            ic->ic_bss->ni_flags &= ~IEEE80211_NODE_TXRXPROT;
            ic->ic_bss->ni_port_valid = 0;
        }
    }
    return kIOReturnSuccess;
}

IOReturn AirportSequoiaInfra::
installPTK(const UInt8 *key, size_t keyLength, const UInt8 *rsc)
{
    ieee80211com *ic = fHalService ? fHalService->get80211Controller() : NULL;
    ieee80211_node *node = ic ? ic->ic_bss : NULL;
    if (!key || !node || !ic->ic_set_key || !ic->ic_delete_key)
        return kIOReturnNotReady;
    if (node->ni_rsncipher == IEEE80211_CIPHER_USEGROUP)
        return kIOReturnUnsupported;
    int cipherLength = ieee80211_cipher_keylen(node->ni_rsncipher);
    if (cipherLength <= 0)
        return kIOReturnBadArgument;
    const size_t expectedLength = static_cast<size_t>(cipherLength);
    if (keyLength != expectedLength)
        return kIOReturnBadArgument;

    ieee80211_key *target = &node->ni_pairwise_key;
    if (target->k_cipher == node->ni_rsncipher &&
        static_cast<size_t>(target->k_len) == expectedLength &&
        memcmp(target->k_key, key, expectedLength) == 0) {
        // Duplicate Apple callbacks must not reset the hardware PN/replay
        // state (KRACK-style key reinstallation).
        node->ni_flags &=
            ~static_cast<u_int32_t>(IEEE80211_NODE_RSN_NEW_PTK);
        return kIOReturnSuccess;
    }

    node->ni_flags &= ~(IEEE80211_NODE_TXRXPROT |
                        IEEE80211_NODE_RXPROT);
    node->ni_port_valid = 0;
    if (target->k_cipher != IEEE80211_CIPHER_NONE)
        (*ic->ic_delete_key)(ic, node, target);
    explicit_bzero(target, sizeof(*target));
    target->k_cipher = node->ni_rsncipher;
    target->k_rsc[0] = rsc ? LE_READ_6(rsc) : 0;
    target->k_len = static_cast<u_int>(expectedLength);
    memcpy(target->k_key, key, target->k_len);
    if ((*ic->ic_set_key)(ic, node, target) != 0) {
        (*ic->ic_delete_key)(ic, node, target);
        explicit_bzero(target, sizeof(*target));
        node->ni_rsn_supp_state = RSNA_SUPP_INITIALIZE;
        return kIOReturnIOError;
    }
    node->ni_flags &=
        ~static_cast<u_int32_t>(IEEE80211_NODE_RSN_NEW_PTK);
    node->ni_rsn_supp_state = RNSA_SUPP_PTKDONE;
    node->ni_flags |= IEEE80211_NODE_RXPROT;
    return kIOReturnSuccess;
}

IOReturn AirportSequoiaInfra::
installGTK(const UInt8 *key, size_t keyLength, UInt16 keyId,
           const UInt8 *rsc)
{
    ieee80211com *ic = fHalService ? fHalService->get80211Controller() : NULL;
    ieee80211_node *node = ic ? ic->ic_bss : NULL;
    if (!key || !rsc || !node || !ic->ic_set_key || !ic->ic_delete_key ||
        keyId >= ARRAY_SIZE(ic->ic_nw_keys))
        return kIOReturnBadArgument;
    if (node->ni_rsn_supp_state != RNSA_SUPP_PTKDONE)
        return kIOReturnNotReady;
    int cipherLength = ieee80211_cipher_keylen(node->ni_rsngroupcipher);
    if (cipherLength <= 0)
        return kIOReturnBadArgument;
    const size_t expectedLength = static_cast<size_t>(cipherLength);
    if (keyLength != expectedLength)
        return kIOReturnBadArgument;

    ieee80211_key *target = &ic->ic_nw_keys[keyId];
    const bool duplicate =
        target->k_cipher == node->ni_rsngroupcipher &&
        static_cast<size_t>(target->k_len) == expectedLength &&
        memcmp(target->k_key, key, expectedLength) == 0;
    if (!duplicate) {
        node->ni_flags &= ~IEEE80211_NODE_TXRXPROT;
        node->ni_port_valid = 0;
        if (target->k_cipher != IEEE80211_CIPHER_NONE)
            (*ic->ic_delete_key)(ic, NULL, target);
        explicit_bzero(target, sizeof(*target));
        target->k_id = static_cast<UInt8>(keyId);
        target->k_cipher = node->ni_rsngroupcipher;
        target->k_flags = IEEE80211_KEY_GROUP;
        target->k_rsc[0] = LE_READ_6(rsc);
        target->k_len = static_cast<u_int>(expectedLength);
        memcpy(target->k_key, key, target->k_len);
        if ((*ic->ic_set_key)(ic, node, target) != 0) {
            (*ic->ic_delete_key)(ic, NULL, target);
            explicit_bzero(target, sizeof(*target));
            return kIOReturnIOError;
        }
    }

    // Both a new GTK and a duplicate-safe callback converge here. Reinstalling
    // the same key would reset hardware replay state, but skipping this
    // finalization would leave WPA data traffic permanently blocked.
    node->ni_flags |= IEEE80211_NODE_TXRXPROT;
    node->ni_port_valid = 1;
    ieee80211_set_link_state(ic, LINK_STATE_UP);
    node->ni_assoc_fail = 0;
    if (ic->ic_opmode == IEEE80211_M_STA)
        ic->ic_rsngroupcipher = node->ni_rsngroupcipher;
    DRVLOG("GTK %s; controlled port valid",
           duplicate ? "reused without reinstall" : "installed");
    return kIOReturnSuccess;
}

IOReturn AirportSequoiaInfra::
handleProtocol(setCIPHER_KEYTag, apple80211_key *key)
{
    if (!key || key->key_len > sizeof(key->key) ||
        key->key_rsc_len > sizeof(key->key_rsc) ||
        key->wowl_kck_len > sizeof(key->wowl_kck_key) ||
        key->wowl_kek_len > sizeof(key->wowl_kek_key))
        return kIOReturnBadArgument;
    if (key->wowl_kck_len != 0 || key->wowl_kek_len != 0)
        return kIOReturnUnsupported;

    ieee80211com *keyIc =
        fHalService ? fHalService->get80211Controller() : NULL;
    bool zeroKeyAddress = true;
    for (UInt index = 0; index < ETHER_ADDR_LEN; index++)
        zeroKeyAddress &= key->key_ea.octet[index] == 0;
    if (!zeroKeyAddress) {
        if (!keyIc || !keyIc->ic_bss)
            return kIOReturnNotReady;
        if (memcmp(key->key_ea.octet, keyIc->ic_bss->ni_macaddr,
                   ETHER_ADDR_LEN) != 0)
            return kIOReturnBadArgument;
    }

    DRVLOG("setCIPHER_KEY cipher=%u flags=%u index=%u len=%u",
           key->key_cipher_type, key->key_flags, key->key_index,
           key->key_len);
    switch (key->key_cipher_type) {
        case APPLE80211_CIPHER_NONE:
            return removeKey(key);
        case APPLE80211_CIPHER_TKIP:
        case APPLE80211_CIPHER_AES_CCM: {
            bool unicast =
                (key->key_flags & APPLE80211_KEY_FLAG_UNICAST) != 0;
            bool multicast =
                (key->key_flags & APPLE80211_KEY_FLAG_MULTICAST) != 0;
            if (unicast && multicast)
                return kIOReturnBadArgument;
            if (!unicast && !multicast) {
                unicast =
                    (key->key_flags & APPLE80211_KEY_FLAG_TX) != 0;
                multicast = !unicast;
            }
            ieee80211_node *node = keyIc ? keyIc->ic_bss : NULL;
            if (!node)
                return kIOReturnNotReady;
            const enum ieee80211_cipher requestedCipher =
                key->key_cipher_type == APPLE80211_CIPHER_TKIP ?
                    IEEE80211_CIPHER_TKIP : IEEE80211_CIPHER_CCMP;
            if (key->key_rsc_len != 0 && key->key_rsc_len < 6)
                return kIOReturnBadArgument;
            const UInt8 *keyRsc =
                key->key_rsc_len == 0 ? NULL : key->key_rsc;
            if (unicast) {
                if (node->ni_rsncipher != requestedCipher)
                    return kIOReturnUnsupported;
                return installPTK(key->key, key->key_len, keyRsc);
            }
            if (node->ni_rsngroupcipher != requestedCipher)
                return kIOReturnUnsupported;
            if (!keyRsc)
                return kIOReturnBadArgument;
            return installGTK(key->key, key->key_len, key->key_index,
                              keyRsc);
        }
        case APPLE80211_CIPHER_AES_OCB:
            return kIOReturnUnsupported;
        case APPLE80211_CIPHER_MSK:
        case APPLE80211_CIPHER_PMKSA: {
            ieee80211com *ic = keyIc;
            if (!ic || !ic->ic_bss)
                return kIOReturnNotReady;
            if (key->key_len != IEEE80211_PMK_LEN)
                return kIOReturnBadArgument;
            enum ieee80211_akm akm =
                (enum ieee80211_akm)ic->ic_bss->ni_rsnakms;
            if (akm == 0)
                return kIOReturnNotReady;
            if (key->key_cipher_type == APPLE80211_CIPHER_MSK &&
                !ieee80211_is_8021x_akm(akm))
                return kIOReturnUnsupported;
            return ieee80211_pmksa_add(ic, akm,
                                       ic->ic_bss->ni_macaddr, key->key, 0)
                       ? kIOReturnSuccess
                       : kIOReturnError;
        }
        default:
            return kIOReturnUnsupported;
    }
}

// ---------------------------------------------------------------------------
// AirportSequoia
// ---------------------------------------------------------------------------

IOCommandGate *AirportSequoia::retainMainCommandGate()
{
    if (!lifecycleLock)
        return NULL;

    IOInterruptState interruptState =
        IOSimpleLockLockDisableInterrupt(lifecycleLock);
    IOCommandGate *gate =
        (!stopping && fHalService) ? fHalService->getMainCommandGate() : NULL;
    if (gate)
        gate->retain();
    IOSimpleLockUnlockEnableInterrupt(lifecycleLock, interruptState);
    return gate;
}

bool AirportSequoia::isStopping()
{
    if (!lifecycleLock)
        return true;
    IOInterruptState interruptState =
        IOSimpleLockLockDisableInterrupt(lifecycleLock);
    bool value = stopping;
    IOSimpleLockUnlockEnableInterrupt(lifecycleLock, interruptState);
    return value;
}

bool AirportSequoia::claimWclScanToken()
{
    if (!lifecycleLock)
        return false;
    IOInterruptState interruptState =
        IOSimpleLockLockDisableInterrupt(lifecycleLock);
    bool claimed = !stopping && !wclScanPending;
    if (claimed)
        wclScanPending = true;
    IOSimpleLockUnlockEnableInterrupt(lifecycleLock, interruptState);
    return claimed;
}

bool AirportSequoia::isWclScanPending()
{
    if (!lifecycleLock)
        return false;
    IOInterruptState interruptState =
        IOSimpleLockLockDisableInterrupt(lifecycleLock);
    bool pending = wclScanPending;
    IOSimpleLockUnlockEnableInterrupt(lifecycleLock, interruptState);
    return pending;
}

void AirportSequoia::clearWclScanToken()
{
    if (!lifecycleLock)
        return;
    IOInterruptState interruptState =
        IOSimpleLockLockDisableInterrupt(lifecycleLock);
    wclScanPending = false;
    IOSimpleLockUnlockEnableInterrupt(lifecycleLock, interruptState);
}

void AirportSequoia::clearEventHandlerAfterDrain()
{
    if (!fHalService)
        return;
    ieee80211com *ic = fHalService->get80211Controller();
    if (ic && ic->ic_event_handler == eventHandler)
        ic->ic_event_handler = NULL;
}

IOReturn AirportSequoia::
quiesceGated(OSObject *owner, void *arg0, void *arg1, void *arg2, void *arg3)
{
    (void)owner;
    (void)arg0;
    (void)arg1;
    (void)arg2;
    (void)arg3;
    return kIOReturnSuccess;
}

void AirportSequoia::quiesceAsyncSources()
{
    IOCommandGate *barrierGate = NULL;
    if (lifecycleLock) {
        IOInterruptState interruptState =
            IOSimpleLockLockDisableInterrupt(lifecycleLock);
        stopping = true;
        wclScanPending = false;
        if (fHalService)
            barrierGate = fHalService->getMainCommandGate();
        if (barrierGate)
            barrierGate->retain();
        IOSimpleLockUnlockEnableInterrupt(lifecycleLock, interruptState);
    } else {
        stopping = true;
        wclScanPending = false;
    }

    // WCL calls are synchronous gate actions. This waits for an action that
    // was admitted before stopping became visible. A caller that retained the
    // gate but enters later is rejected by the gated stopping check.
    if (barrierGate) {
        barrierGate->runAction(quiesceGated);
        barrierGate->release();
    }

    // Power callbacks use the command gate, so cancel/wait for them before
    // the gate or any interface/HAL state can be released.
    unregistPM();

    if (watchdogTimer) {
        watchdogTimer->cancelTimeout();
        watchdogTimer->disable();
        if (fWatchdogWorkLoop)
            fWatchdogWorkLoop->removeEventSource(watchdogTimer);
        watchdogTimer->release();
        watchdogTimer = NULL;
    }
    if (fWatchdogWorkLoop) {
        fWatchdogWorkLoop->release();
        fWatchdogWorkLoop = NULL;
    }
}

void AirportSequoia::releaseAll()
{
    quiesceAsyncSources();

    IOCommandGate *commandGate = NULL;
    IOWorkLoop *mainWorkLoop = NULL;
    if (lifecycleLock) {
        IOInterruptState interruptState =
            IOSimpleLockLockDisableInterrupt(lifecycleLock);
        if (fHalService) {
            commandGate = fHalService->getMainCommandGate();
            mainWorkLoop = fHalService->getMainWorkLoop();
        }
        if (_fCommandGate == commandGate)
            _fCommandGate = NULL;
        IOSimpleLockUnlockEnableInterrupt(lifecycleLock, interruptState);
    } else if (fHalService) {
        commandGate = fHalService->getMainCommandGate();
        mainWorkLoop = fHalService->getMainWorkLoop();
        if (_fCommandGate == commandGate)
            _fCommandGate = NULL;
    }
    if (commandGate && mainWorkLoop)
        mainWorkLoop->removeEventSource(commandGate);
    // Removing the event source is the barrier for any WCL action that already
    // entered the shared gate. Only then may the HAL drop its borrowed
    // controller/work-loop context.
    if (fHalService)
        fHalService->deinitWithController();
    if (commandGate)
        commandGate->release();
    if (fHalService) {
        fHalService->release();
        fHalService = NULL;
    }
}

void AirportSequoia::
eventHandler(struct ieee80211com *ic, int msgCode, void *data)
{
    (void)data;
    if (!ic)
        return;
    AirportSequoia *that = OSDynamicCast(AirportSequoia, ic->ic_ac.ac_if.controller);
    if (!that || !that->lifecycleLock)
        return;

    IO80211InfraInterface *interface = NULL;
    bool deliver = false;
    IOInterruptState interruptState =
        IOSimpleLockLockDisableInterrupt(that->lifecycleLock);
    if (!that->stopping && that->fNetIf) {
        switch (msgCode) {
            case IEEE80211_EVT_COUNTRY_CODE_UPDATE:
            case IEEE80211_EVT_STA_ASSOC_DONE:
            case IEEE80211_EVT_STA_DEAUTH:
                deliver = true;
                break;
            case IEEE80211_EVT_SCAN_DONE:
                if (that->wclScanPending) {
                    that->wclScanPending = false;
                    deliver = true;
                }
                break;
            default:
                break;
        }
        if (deliver) {
            that->retain();
            interface = that->fNetIf;
            interface->retain();
        }
    }
    IOSimpleLockUnlockEnableInterrupt(that->lifecycleLock, interruptState);
    if (!deliver)
        return;

    switch (msgCode) {
        case IEEE80211_EVT_COUNTRY_CODE_UPDATE:
            interface->postMessage(APPLE80211_M_COUNTRY_CODE_CHANGED, NULL, 0, false);
            break;
        case IEEE80211_EVT_STA_ASSOC_DONE:
            interface->postMessage(APPLE80211_M_ASSOC_DONE, NULL, 0, false);
            break;
        case IEEE80211_EVT_STA_DEAUTH:
            interface->postMessage(APPLE80211_M_DEAUTH_RECEIVED, NULL, 0, false);
            break;
        case IEEE80211_EVT_SCAN_DONE: {
            UInt32 status = 0;
            interface->postMessage(APPLE80211_M_SCAN_DONE, &status,
                                   sizeof(status), false);
            break;
        }
        default:
            break;
    }
    interface->release();
    that->release();
}

void AirportSequoia::watchdogAction(IOTimerEventSource *timer)
{
    if (isStopping() || !timer || timer != watchdogTimer || !fHalService)
        return;
    ieee80211com *ic = fHalService->get80211Controller();
    if (!ic)
        return;
    struct _ifnet *ifp = &ic->ic_ac.ac_if;
    if (!ifp->if_watchdog)
        return;
    (*ifp->if_watchdog)(ifp);
    if (!isStopping() && timer == watchdogTimer)
        timer->setTimeoutMS(kWatchDogTimerPeriod);
}

bool AirportSequoia::init(OSDictionary *properties)
{
    DRVLOG("%s\n", __PRETTY_FUNCTION__);
    pciNub = NULL;
    fHalService = NULL;
    fNetIf = NULL;
    bsdInterface = NULL;
    fpNetStats = NULL;
    watchdogTimer = NULL;
    fWatchdogWorkLoop = NULL;
    lifecycleLock = NULL;
    wclScanPending = false;
    power_state = 0;
    currentStatus = 0;
    powerOnThreadCall = NULL;
    powerOffThreadCall = NULL;
    pmPowerState = kPowerStateOff;
    pmPolicyMaker = NULL;
    pmPCICapPtr = 0;
    magicPacketEnabled = false;
    magicPacketSupported = false;
    bzero(geoLocationCountryCode, sizeof(geoLocationCountryCode));
    stopping = false;
    if (!IO80211Controller::init(properties))
        return false;
    lifecycleLock = IOSimpleLockAlloc();
    return lifecycleLock != NULL;
}

IOService *AirportSequoia::probe(IOService *provider, SInt32 *score)
{
    DRVLOG("%s\n", __PRETTY_FUNCTION__);
    IOPCIEDeviceWrapper *wrapper = OSDynamicCast(IOPCIEDeviceWrapper, provider);
    if (!wrapper) {
        DRVERR("%s Not a IOPCIEDeviceWrapper instance\n", __FUNCTION__);
        return NULL;
    }
    pciNub = wrapper->pciNub;
    fHalService = wrapper->createHalService();
    if (!pciNub || !fHalService) {
        DRVERR("%s Not a valid IOPCIEDeviceWrapper instance\n", __FUNCTION__);
        OSSafeReleaseNULL(fHalService);
        return NULL;
    }
    IOService *result = IO80211Controller::probe(provider, score);
    if (!result) {
        OSSafeReleaseNULL(fHalService);
    }
    return result;
}

bool AirportSequoia::start(IOService *provider)
{
    DRVLOG("%s\n", __PRETTY_FUNCTION__);
    IOPCIEDeviceWrapper *wrapper =
        OSDynamicCast(IOPCIEDeviceWrapper, provider);
    if (!wrapper)
        return false;
    int boot_value = 0;
    UInt8 builtIn = 0;
    OSData *builtInData = OSData::withBytes(&builtIn, sizeof(builtIn));
    if (!builtInData)
        return false;
    bool propertiesReady = setProperty("built-in", builtInData);
    builtInData->release();
    propertiesReady = propertiesReady &&
                      setProperty("DriverKitDriver", kOSBooleanFalse);
    if (!propertiesReady || !IO80211Controller::start(provider))
        return false;

    IOInterruptState interruptState =
        IOSimpleLockLockDisableInterrupt(lifecycleLock);
    stopping = false;
    wclScanPending = false;
    IOSimpleLockUnlockEnableInterrupt(lifecycleLock, interruptState);
    bool halAttached = false;
    bool infraServiceAttached = false;
    bool infraControllerAttached = false;
    bool bsdAttached = false;
    bool infraStarted = false;
    bool adapterEnabled = false;
    auto failStart = [&]() -> bool {
        quiesceAsyncSources();
        struct _ifnet *ifp = NULL;
        if (fHalService && fHalService->get80211Controller())
            ifp = &fHalService->get80211Controller()->ic_ac.ac_if;
        if (adapterEnabled && bsdInterface)
            disableAdapter(bsdInterface);
        if (infraStarted && fNetIf)
            fNetIf->stop(this);
        if (halAttached && fHalService && pciNub) {
            fHalService->detach(pciNub);
            clearEventHandlerAfterDrain();
            halAttached = false;
        }
        if (ifp && bsdInterface)
            ether_ifdetach(ifp);
        if (bsdAttached && bsdInterface) {
            IONetworkController::detachInterface(bsdInterface, true);
            OSSafeReleaseNULL(bsdInterface);
        }
        if (infraControllerAttached && fNetIf)
            detachInterface(fNetIf, true);
        else if (infraServiceAttached && fNetIf)
            fNetIf->detach(this);
        OSSafeReleaseNULL(fNetIf);
        IO80211Controller::stop(provider);
        releaseAll();
        return false;
    };

    pciNub->setBusMasterEnable(true);
    pciNub->setIOEnable(true);
    pciNub->setMemoryEnable(true);
    pciNub->configWrite8(0x41, 0);
    if (pciNub->requestPowerDomainState(kIOPMPowerOn,
                                        (IOPowerConnection *) getParentEntry(gIOPowerPlane), IOPMLowestState) != IOPMNoErr) {
        return failStart();
    }
    if (initPCIPowerManagment(pciNub) == false) {
        return failStart();
    }
    IO80211WorkQueue *mainWorkLoop =
        OSDynamicCast(IO80211WorkQueue, wrapper->getWorkLoop());
    if (mainWorkLoop == NULL) {
        DRVERR("No work queue!!\n");
        return failStart();
    }
    IOCommandGate *mainCommandGate =
        IOCommandGate::commandGate(this,
                                   (IOCommandGate::Action)AirportSequoia::tsleepHandler);
    if (mainCommandGate == NULL) {
        DRVERR("No command gate!!\n");
        return failStart();
    }
    if (mainWorkLoop->addEventSource(mainCommandGate) != kIOReturnSuccess) {
        DRVERR("add command gate failed\n");
        mainCommandGate->release();
        return failStart();
    }
    if (!fHalService->initWithController(this, mainWorkLoop, mainCommandGate)) {
        DRVERR("HAL controller initialization failed\n");
        mainWorkLoop->removeEventSource(mainCommandGate);
        mainCommandGate->release();
        return failStart();
    }
    _fCommandGate = mainCommandGate;
    const IONetworkMedium *primaryMedium;
    if (!createMediumTables(&primaryMedium) ||
        !setCurrentMedium(primaryMedium) || !setSelectedMedium(primaryMedium)) {
        DRVERR("setup medium fail\n");
        return failStart();
    }
    if (PE_parse_boot_argn("-novht", &boot_value, sizeof(boot_value)))
        fHalService->get80211Controller()->ic_userflags |= IEEE80211_F_NOVHT;
    if (PE_parse_boot_argn("-noht40", &boot_value, sizeof(boot_value)))
        fHalService->get80211Controller()->ic_userflags |= IEEE80211_F_NOHT40;

    // === Загрузка firmware Intel ===
    if (!fHalService->attach(pciNub)) {
        DRVERR("HAL attach fail\n");
        return failStart();
    }
    halAttached = true;
    fHalService->get80211Controller()->ic_event_handler = eventHandler;
    DRVLOG("firmware loaded: %s\n", fHalService->getDriverInfo()->getFirmwareName());

    fWatchdogWorkLoop = IOWorkLoop::workLoop();
    if (fWatchdogWorkLoop == NULL) {
        DRVERR("init watchdog workloop fail\n");
        return failStart();
    }
    watchdogTimer = IOTimerEventSource::timerEventSource(this, OSMemberFunctionCast(IOTimerEventSource::Action, this, &AirportSequoia::watchdogAction));
    if (!watchdogTimer) {
        DRVERR("init watchdog fail\n");
        return failStart();
    }
    if (fWatchdogWorkLoop->addEventSource(watchdogTimer) != kIOReturnSuccess) {
        DRVERR("add watchdog timer failed\n");
        return failStart();
    }
    // === Skywalk-интерфейс ===
    fNetIf = new AirportSequoiaInfra;
    if (!fNetIf || !fNetIf->init(this)) {
        DRVERR("infra interface init fail\n");
        return failStart();
    }
    fNetIf->setInterfaceRole(1);
    fNetIf->setInterfaceId(1);

    if (!fNetIf->attach(this)) {
        DRVERR("attach to service fail\n");
        return failStart();
    }
    infraServiceAttached = true;
    if (!attachInterface(fNetIf, this)) {
        DRVERR("attachInterface fail\n");
        return failStart();
    }
    infraControllerAttached = true;
    if (!IONetworkController::attachInterface((IONetworkInterface **)&bsdInterface, true)) {
        DRVERR("attach to IONetworkController interface fail\n");
        return failStart();
    }
    bsdAttached = true;
    static_assert(
        sizeof(IOSkywalkNetworkInterface::RegistrationInfo) == 0x130,
        "IOSkywalkNetworkInterface::RegistrationInfo ABI size mismatch");
    static_assert(
        sizeof(IOSkywalkEthernetInterface::RegistrationInfo) == 0x130,
        "IOSkywalkEthernetInterface::RegistrationInfo ABI size mismatch");
    IOSkywalkEthernetInterface::RegistrationInfo registration = {};
    if (!fNetIf->IOSkywalkEthernetInterface::initRegistrationInfo(
            &registration, 1, sizeof(registration)) ||
        !fNetIf->mExpansionData || !fNetIf->mExpansionData2) {
        DRVERR("Skywalk RegistrationInfo initialization failed\n");
        return failStart();
    }

    IOSkywalkNetworkInterface::RegistrationInfo *networkRegistrationCopy =
        (IOSkywalkNetworkInterface::RegistrationInfo *)
            IOMalloc(sizeof(*networkRegistrationCopy));
    IOSkywalkEthernetInterface::RegistrationInfo *ethernetRegistrationCopy =
        (IOSkywalkEthernetInterface::RegistrationInfo *)
            IOMalloc(sizeof(*ethernetRegistrationCopy));
    if (!networkRegistrationCopy || !ethernetRegistrationCopy) {
        if (networkRegistrationCopy)
            IOFree(networkRegistrationCopy, sizeof(*networkRegistrationCopy));
        if (ethernetRegistrationCopy)
            IOFree(ethernetRegistrationCopy, sizeof(*ethernetRegistrationCopy));
        DRVERR("Skywalk RegistrationInfo allocation failed\n");
        return failStart();
    }
    memcpy(networkRegistrationCopy, &registration,
           sizeof(*networkRegistrationCopy));
    memcpy(ethernetRegistrationCopy, &registration,
           sizeof(*ethernetRegistrationCopy));
    fNetIf->mExpansionData->fRegistrationInfo = networkRegistrationCopy;
    fNetIf->mExpansionData2->fRegistrationInfo = ethernetRegistrationCopy;
    if (fNetIf->getInterfaceRole() == 1)
        fNetIf->deferBSDAttach(true);

    if (!fNetIf->start(this)) {
        DRVERR("infra interface start failed\n");
        return failStart();
    }
    infraStarted = true;

    if (!setLinkStatus(kIONetworkLinkValid))
        return failStart();
    if (TAILQ_EMPTY(&fHalService->get80211Controller()->ic_ess))
        fHalService->get80211Controller()->ic_flags |= IEEE80211_F_AUTO_JOIN;

    IOReturn enableResult = enableAdapter((IONetworkInterface *)bsdInterface);
    if (enableResult != kIOReturnSuccess) {
        DRVERR("enableAdapter failed: 0x%x\n", enableResult);
        return failStart();
    }
    adapterEnabled = true;

    registerService();
    return true;
}

void AirportSequoia::stop(IOService *provider)
{
    DRVLOG("%s\n", __PRETTY_FUNCTION__);
    quiesceAsyncSources();

    struct _ifnet *ifp = NULL;
    if (fHalService && fHalService->get80211Controller())
        ifp = &fHalService->get80211Controller()->ic_ac.ac_if;

    if (fHalService && bsdInterface)
        disableAdapter(bsdInterface);
    if (fNetIf && bsdInterface)
        setLinkStatus(kIONetworkLinkValid);
    if (fNetIf)
        fNetIf->stop(this);
    // Stop IRQ/timer/taskq producers and drain every HAL callback while the
    // BSD interface they may reference is still valid.  The Skywalk service
    // is already stopped, so no new requests can enter the HAL.
    if (fHalService && pciNub) {
        fHalService->detach(pciNub);
        clearEventHandlerAfterDrain();
    }
    if (ifp && bsdInterface)
        ether_ifdetach(ifp);
    if (bsdInterface) {
        IONetworkController::detachInterface(bsdInterface, true);
        OSSafeReleaseNULL(bsdInterface);
    }
    if (fNetIf) {
        detachInterface(fNetIf, true);
        OSSafeReleaseNULL(fNetIf);
    }
    IO80211Controller::stop(provider);
    releaseAll();
}

void AirportSequoia::free()
{
    DRVLOG("%s\n", __PRETTY_FUNCTION__);
    releaseAll();
    IOSimpleLock *lock = lifecycleLock;
    lifecycleLock = NULL;
    if (lock)
        IOSimpleLockFree(lock);
    IO80211Controller::free();
}

// --- IO80211Controller appended slots --------------------------------------

bool AirportSequoia::isCommandProhibited(int command)
{
    (void)command;
    return false;
}

bool AirportSequoia::createWorkQueue()
{
    IO80211WorkQueue *workQueue = getWorkQueue();
    DRVLOG("%s %d\n", __FUNCTION__, workQueue != NULL);
    return workQueue != NULL;
}

IO80211WorkQueue *AirportSequoia::getWorkQueue() const
{
    IOPCIEDeviceWrapper *wrapper =
        OSDynamicCast(IOPCIEDeviceWrapper, getProvider());
    if (wrapper)
        return OSDynamicCast(IO80211WorkQueue, wrapper->getWorkLoop());
    if (fHalService)
        return OSDynamicCast(IO80211WorkQueue,
                             fHalService->getMainWorkLoop());
    return NULL;
}

SInt32 AirportSequoia::
handleCardSpecific(IO80211SkywalkInterface *interface, unsigned long command,
                   void *data, bool isSet)
{
    (void)interface;
    (void)command;
    (void)data;
    (void)isSet;
    return kIOReturnUnsupported;
}

IOReturn AirportSequoia::
getDRIVER_VERSION(IO80211SkywalkInterface *interface,
                  apple80211_version_data *data)
{
    (void)interface;
    if (!data || !fHalService || !fHalService->getDriverInfo())
        return kIOReturnBadArgument;
    bzero(data, sizeof(*data));
    data->version = APPLE80211_VERSION;
    snprintf(data->string, sizeof(data->string),
             "AirportSequoia %s, fw: %s", AIRPORT_SEQUOIA_VERSION,
             fHalService->getDriverInfo()->getFirmwareVersion());
    data->string_len = strnlen(data->string, sizeof(data->string));
    return kIOReturnSuccess;
}

IOReturn AirportSequoia::
getHARDWARE_VERSION(IO80211SkywalkInterface *interface,
                    apple80211_version_data *data)
{
    (void)interface;
    if (!data || !fHalService || !fHalService->getDriverInfo())
        return kIOReturnBadArgument;
    bzero(data, sizeof(*data));
    data->version = APPLE80211_VERSION;
    strlcpy(data->string, fHalService->getDriverInfo()->getFirmwareVersion(),
            sizeof(data->string));
    data->string_len = strnlen(data->string, sizeof(data->string));
    return kIOReturnSuccess;
}

IOReturn AirportSequoia::
getCARD_CAPABILITIES(IO80211SkywalkInterface *interface,
                     apple80211_capability_data *data)
{
    (void)interface;
    if (!data || !fHalService || !fHalService->get80211Controller())
        return kIOReturnBadArgument;

    UInt32 caps = fHalService->get80211Controller()->ic_caps;
    bzero(data, sizeof(*data));
    data->version = APPLE80211_VERSION;
    if (caps & IEEE80211_C_WEP)
        data->capabilities[0] |= 1 << APPLE80211_CAP_WEP;
    if (caps & IEEE80211_C_RSN) {
        data->capabilities[0] |=
            (1 << APPLE80211_CAP_TKIP) | (1 << APPLE80211_CAP_AES_CCM);
        data->capabilities[1] |=
            (1 << (APPLE80211_CAP_WPA1 - 8)) |
            (1 << (APPLE80211_CAP_WPA2 - 8)) |
            (1 << (APPLE80211_CAP_TKIPMIC - 8));
    }
    if (caps & IEEE80211_C_SHSLOT)
        data->capabilities[1] |= 1 << (APPLE80211_CAP_SHSLOT - 8);
    if (caps & IEEE80211_C_SHPREAMBLE)
        data->capabilities[1] |= 1 << (APPLE80211_CAP_SHPREAMBLE - 8);

    // Values retained from the working AirportItlwmV2 capability contract.
    data->capabilities[2] = 0xff;
    data->capabilities[3] = 0x2b;
    data->capabilities[5] = 0x40;
    data->capabilities[6] = 0x8 | 0x4 | 0x80;
    *(uint16_t *)&data->capabilities[8] = 0x0201;
    return kIOReturnSuccess;
}

IOReturn AirportSequoia::
getPOWER(IO80211SkywalkInterface *interface, apple80211_power_data *data)
{
    (void)interface;
    if (!data)
        return kIOReturnBadArgument;
    data->version = APPLE80211_VERSION;
    data->num_radios = 4;
    for (UInt i = 0; i < data->num_radios; i++)
        data->power_state[i] = power_state;
    return kIOReturnSuccess;
}

IOReturn AirportSequoia::
setPOWER(IO80211SkywalkInterface *interface, apple80211_power_data *data)
{
    (void)interface;
    if (!data || data->num_radios == 0 || !bsdInterface)
        return kIOReturnBadArgument;

    bool requestedOn = data->power_state[0] != 0;
    if (requestedOn == (power_state != 0))
        return kIOReturnSuccess;
    if (requestedOn)
        return enableAdapter((IONetworkInterface *)bsdInterface);
    disableAdapter((IONetworkInterface *)bsdInterface);
    return kIOReturnSuccess;
}

IOReturn AirportSequoia::
getCOUNTRY_CODE(IO80211SkywalkInterface *interface,
                apple80211_country_code_data *data)
{
    (void)interface;
    if (!data || !fHalService || !fHalService->getDriverInfo())
        return kIOReturnBadArgument;

    char userOverride[3] = {};
    const char *firmware =
        fHalService->getDriverInfo()->getFirmwareCountryCode();
    PE_parse_boot_argn("itlwm_cc", userOverride, sizeof(userOverride));
    const char *country = userOverride[0] ? userOverride : firmware;
    if (!userOverride[0] && firmware && firmware[0] == 'Z' &&
        firmware[1] == 'Z' && geoLocationCountryCode[0])
        country = geoLocationCountryCode;
    if (!country)
        return kIOReturnNotFound;

    data->version = APPLE80211_VERSION;
    bzero(data->cc, sizeof(data->cc));
    memcpy(data->cc, country, MIN(sizeof(data->cc), (size_t)2));
    return kIOReturnSuccess;
}

IOReturn AirportSequoia::
setCOUNTRY_CODE(IO80211SkywalkInterface *interface,
                apple80211_country_code_data *data)
{
    (void)interface;
    if (!data)
        return kIOReturnBadArgument;
    if (data->cc[0] == 'x' || data->cc[0] == 'X') {
        bzero(geoLocationCountryCode, sizeof(geoLocationCountryCode));
    } else if (data->cc[0] && data->cc[1]) {
        geoLocationCountryCode[0] = data->cc[0];
        geoLocationCountryCode[1] = data->cc[1];
        geoLocationCountryCode[2] = '\0';
    } else {
        return kIOReturnBadArgument;
    }
    if (fNetIf)
        fNetIf->postMessage(APPLE80211_M_COUNTRY_CODE_CHANGED, NULL, 0,
                            false);
    return kIOReturnSuccess;
}

IOReturn AirportSequoia::
setGET_DEBUG_INFO(IO80211SkywalkInterface *interface,
                  apple80211_debug_command *data)
{
    (void)interface;
    (void)data;
    return kIOReturnUnsupported;
}

CCLogStream *AirportSequoia::getLogger() const
{
    return NULL;
}

CCFaultReporter *AirportSequoia::getFaultReporterFromDriver()
{
    return NULL;
}

// --- Сеть -------------------------------------------------------------------

IONetworkInterface *AirportSequoia::createInterface()
{
    AirportSequoiaEthernetInterface *netif = new AirportSequoiaEthernetInterface;
    if (!netif)
        return NULL;
    if (!netif->initWithSkywalkInterfaceAndProvider(this, fNetIf)) {
        netif->release();
        return NULL;
    }
    return netif;
}

bool AirportSequoia::configureInterface(IONetworkInterface *netif)
{
    IONetworkData *nd;
    struct _ifnet *ifp = &fHalService->get80211Controller()->ic_ac.ac_if;

    if (IO80211Controller::configureInterface(netif) == false) {
        DRVERR("super failed\n");
        return false;
    }

    nd = netif->getParameter(kIONetworkStatsKey);
    if (!nd || !(fpNetStats = (IONetworkStats *)nd->getBuffer())) {
        DRVERR("network statistics buffer unavailable?\n");
        return false;
    }
    ifp->netStat = fpNetStats;
    ether_ifattach(ifp, OSDynamicCast(IOEthernetInterface, netif));
    fpNetStats->collisions = 0;
#ifdef __PRIVATE_SPI__
    netif->configureOutputPullModel(fHalService->getDriverInfo()->getTxQueueSize(), 0, 0, IOEthernetInterface::kOutputPacketSchedulingModelNormal, 0);
#endif
    return true;
}

bool AirportSequoia::createMediumTables(const IONetworkMedium **primary)
{
    IONetworkMedium *medium;

    OSDictionary *mediumDict = OSDictionary::withCapacity(2);
    if (mediumDict == NULL) {
        DRVERR("Cannot allocate OSDictionary\n");
        return false;
    }

    medium = IONetworkMedium::medium(kIOMediumIEEE80211, 54000000);
    if (!medium) {
        mediumDict->release();
        return false;
    }
    if (!IONetworkMedium::addMedium(mediumDict, medium)) {
        medium->release();
        mediumDict->release();
        return false;
    }
    medium->release();
    if (primary) {
        *primary = medium;
    }
    medium = IONetworkMedium::medium(kIOMediumIEEE80211None, 0);
    if (!medium) {
        mediumDict->release();
        return false;
    }
    if (!IONetworkMedium::addMedium(mediumDict, medium)) {
        medium->release();
        mediumDict->release();
        return false;
    }
    medium->release();

    bool result = publishMediumDictionary(mediumDict);
    if (!result) {
        DRVERR("Cannot publish medium dictionary!\n");
    }

    mediumDict->release();
    return result;
}

IOReturn AirportSequoia::selectMedium(const IONetworkMedium *medium)
{
    setSelectedMedium(medium);
    return kIOReturnSuccess;
}

bool AirportSequoia::
setLinkStatus(UInt32 status, const IONetworkMedium *activeMedium, UInt64 speed, OSData *data)
{
    struct _ifnet *ifq = &fHalService->get80211Controller()->ic_ac.ac_if;
    if (status == currentStatus) {
        return true;
    }
    bool ret = IO80211Controller::setLinkStatus(status, activeMedium, speed, data);
    currentStatus = status;
    if (fNetIf) {
        if (status & kIONetworkLinkActive) {
#ifdef __PRIVATE_SPI__
            bsdInterface->startOutputThread();
#endif
            getCommandGate()->runAction(setLinkStateGated, (void *)kIO80211NetworkLinkUp, (void *)0);
        } else if (!(status & kIONetworkLinkNoNetworkChange)) {
#ifdef __PRIVATE_SPI__
            bsdInterface->stopOutputThread();
            bsdInterface->flushOutputQueue();
#endif
            ifq_flush(&ifq->if_snd);
            mq_purge(&fHalService->get80211Controller()->ic_mgtq);
            getCommandGate()->runAction(setLinkStateGated, (void *)kIO80211NetworkLinkDown, (void *)(uint64_t)fHalService->get80211Controller()->ic_deauth_reason);
        }
    }
    return ret;
}

IOReturn AirportSequoia::
setLinkStateGated(OSObject *target, void *arg0, void *arg1, void *arg2, void *arg3)
{
    AirportSequoia *that = OSDynamicCast(AirportSequoia, target);
    // Sequoia: setLinkState(IO80211LinkState, unsigned int, bool, unsigned int)
    IOReturn ret = that->fNetIf->setLinkState((IO80211LinkState)(uint64_t)arg0, (unsigned int)(uint64_t)arg1, false, 0);
    that->fNetIf->setRunningState((IO80211LinkState)(uint64_t)arg0 == kIO80211NetworkLinkUp);
    that->fNetIf->postMessage(APPLE80211_M_LINK_CHANGED, NULL, 0, false);
    that->fNetIf->postMessage(APPLE80211_M_BSSID_CHANGED, NULL, 0, false);
    that->fNetIf->postMessage(APPLE80211_M_SSID_CHANGED, NULL, 0, false);
    if ((IO80211LinkState)(uint64_t)arg0 == kIO80211NetworkLinkUp) {
        that->fNetIf->reportLinkStatus(3, 0x80);
    } else {
        that->fNetIf->reportLinkStatus(1, 0);
    }
    that->bsdInterface->setLinkState((IO80211LinkState)(uint64_t)arg0);
    return ret;
}

#ifdef __PRIVATE_SPI__
IOReturn AirportSequoia::outputStart(IONetworkInterface *interface, IOOptionBits options)
{
    struct _ifnet *ifp = &fHalService->get80211Controller()->ic_ac.ac_if;
    mbuf_t m = NULL;
    if (ifq_is_oactive(&ifp->if_snd))
        return kIOReturnNoResources;
    while (kIOReturnSuccess == interface->dequeueOutputPackets(1, &m)) {
        if (outputPacket(m, NULL) != kIOReturnOutputSuccess ||
            ifq_is_oactive(&ifp->if_snd))
            return kIOReturnNoResources;
    }
    return kIOReturnSuccess;
}

IOReturn AirportSequoia::networkInterfaceNotification(IONetworkInterface *interface, uint32_t type, void *argument)
{
    DRVLOG("%s\n", __FUNCTION__);
    return kIOReturnSuccess;
}
#endif

UInt32 AirportSequoia::outputPacket(mbuf_t m, void *param)
{
    (void)param;
    struct _ifnet *ifp = &fHalService->get80211Controller()->ic_ac.ac_if;

    if (fHalService->get80211Controller()->ic_state != IEEE80211_S_RUN || ifp->if_snd.queue == NULL) {
        if (m && mbuf_type(m) != MBUF_TYPE_FREE)
            freePacket(m);
        return kIOReturnOutputDropped;
    }
    if (m == NULL) {
        DRVERR("%s m==NULL!!\n", __FUNCTION__);
        if (ifp->netStat)
            ifp->netStat->outputErrors++;
        return kIOReturnOutputDropped;
    }
    if (mbuf_type(m) == MBUF_TYPE_FREE) {
        DRVERR("%s mbuf is FREE!!\n", __FUNCTION__);
        if (ifp->netStat)
            ifp->netStat->outputErrors++;
        return kIOReturnOutputDropped;
    }
    if (!(mbuf_flags(m) & MBUF_PKTHDR)) {
        DRVERR("%s pkthdr is NULL!!\n", __FUNCTION__);
        if (ifp->netStat)
            ifp->netStat->outputErrors++;
        freePacket(m);
        return kIOReturnOutputDropped;
    }
    size_t len = mbuf_len(m);
    ether_header_t *eh = (ether_header_t *)mbuf_data(m);
    if (len >= sizeof(ether_header_t) && eh->ether_type == htons(ETHERTYPE_PAE)) { // EAPOL
        const char* dump = hexdump((uint8_t*)mbuf_data(m), len);
        DRVLOG("output EAPOL packet, len: %zu, data: %s\n", len, dump ? dump : "no mem");
        if (dump)
            IOFree((void*)dump, 3 * len + 1);
    }
    if (!ifp->if_snd.queue->lockEnqueue(m)) {
        freePacket(m);
        return kIOReturnOutputDropped;
    }
    (*ifp->if_start)(ifp);
    return kIOReturnOutputSuccess;
}

const OSString *AirportSequoia::newVendorString() const
{
    return OSString::withCString("Apple");
}

const OSString *AirportSequoia::newModelString() const
{
    return OSString::withCString(fHalService->getDriverInfo()->getFirmwareName());
}

IOReturn AirportSequoia::getHardwareAddress(IOEthernetAddress *addrP)
{
    if (IEEE80211_ADDR_EQ(etheranyaddr, fHalService->get80211Controller()->ic_myaddr))
        return kIOReturnError;
    else {
        IEEE80211_ADDR_COPY(addrP, fHalService->get80211Controller()->ic_myaddr);
        return kIOReturnSuccess;
    }
}

IOReturn AirportSequoia::setHardwareAddress(const void *addrP, UInt32 addrBytes)
{
    if (!fNetIf || !addrP)
        return kIOReturnError;
    if_setlladdr(&fHalService->get80211Controller()->ic_ac.ac_if, (const UInt8 *)addrP);
    if (fHalService->get80211Controller()->ic_state > IEEE80211_S_INIT) {
        fHalService->disable(bsdInterface);
        fHalService->enable(bsdInterface);
    }
    return kIOReturnSuccess;
}

UInt32 AirportSequoia::getFeatures() const
{
    return fHalService->getDriverInfo()->supportedFeatures();
}

IOReturn AirportSequoia::setPromiscuousMode(IOEnetPromiscuousMode mode)
{
    return kIOReturnSuccess;
}

IOReturn AirportSequoia::setMulticastMode(IOEnetMulticastMode mode)
{
    return kIOReturnSuccess;
}

IOReturn AirportSequoia::setMulticastList(IOEthernetAddress *addr, UInt32 len)
{
    return fHalService->getDriverController()->setMulticastList(addr, len);
}

IOReturn AirportSequoia::setMulticastList(ether_addr const *addr, UInt count)
{
    return fHalService->getDriverController()->setMulticastList(
        reinterpret_cast<IOEthernetAddress *>(const_cast<ether_addr *>(addr)),
        count);
}

IOReturn AirportSequoia::getPacketFilters(const OSSymbol *group, UInt32 *filters) const
{
    IOReturn rtn = kIOReturnSuccess;
    if (group == gIOEthernetWakeOnLANFilterGroup && magicPacketSupported)
        *filters = kIOEthernetWakeOnMagicPacket;
    else if (group == gIONetworkFilterGroup)
        *filters = kIOPacketFilterMulticast | kIOPacketFilterPromiscuous;
    else
        rtn = IOEthernetController::getPacketFilters(group, filters);
    return rtn;
}

IOReturn AirportSequoia::enableAdapter(IONetworkInterface *netif)
{
    if (!fHalService || !netif || !watchdogTimer)
        return kIOReturnNotReady;
    IOReturn ret = fHalService->enable(netif);
    if (ret != kIOReturnSuccess)
        return ret;
    power_state = 1;
    watchdogTimer->setTimeoutMS(kWatchDogTimerPeriod);
    watchdogTimer->enable();
    return ret;
}

void AirportSequoia::disableAdapter(IONetworkInterface *netif)
{
    if (watchdogTimer) {
        watchdogTimer->cancelTimeout();
        watchdogTimer->disable();
    }
    if (fHalService && netif)
        fHalService->disable(netif);
    power_state = 0;
}

IOReturn AirportSequoia::
tsleepHandler(OSObject *owner, void *arg0, void *arg1, void *arg2, void *arg3)
{
    AirportSequoia *dev = OSDynamicCast(AirportSequoia, owner);
    if (dev == 0)
        return kIOReturnError;
    IOCommandGate *commandGate =
        dev->fHalService ? dev->fHalService->getMainCommandGate() : NULL;
    if (!commandGate)
        return kIOReturnNotReady;

    if (arg1 == 0) {
        if (commandGate->commandSleep(arg0, THREAD_INTERRUPTIBLE) == THREAD_AWAKENED)
            return kIOReturnSuccess;
        else
            return kIOReturnTimeout;
    } else {
        AbsoluteTime deadline;
        clock_interval_to_deadline((*(int*)arg1), kNanosecondScale, reinterpret_cast<uint64_t*>(&deadline));
        if (commandGate->commandSleep(arg0, deadline, THREAD_INTERRUPTIBLE) == THREAD_AWAKENED)
            return kIOReturnSuccess;
        else
            return kIOReturnTimeout;
    }
}

// --- Power management --------------------------------------------------------

bool AirportSequoia::initPCIPowerManagment(IOPCIDevice *provider)
{
    UInt16 reg16;

    reg16 = provider->configRead16(kIOPCIConfigCommand);
    reg16 |= (kIOPCICommandBusMaster | kIOPCICommandMemorySpace | kIOPCICommandMemWrInvalidate);
    reg16 &= ~kIOPCICommandIOSpace;
    provider->configWrite16(kIOPCIConfigCommand, reg16);
    provider->findPCICapability(kIOPCIPowerManagementCapability, &pmPCICapPtr);
    if (pmPCICapPtr) {
        UInt16 pciPMCReg = provider->configRead32(pmPCICapPtr) >> 16;
        if (pciPMCReg & kPCIPMCPMESupportFromD3Cold)
            magicPacketSupported = true;
        provider->configWrite16((pmPCICapPtr + 4), 0x8000);
        IOSleep(10);
    }
    return true;
}

static IOPMPowerState powerStateArray[kPowerStateCount] =
{
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, kIOPMDeviceUsable, kIOPMPowerOn, kIOPMPowerOn, 0, 0, 0, 0, 0, 0, 0, 0}
};

void AirportSequoia::unregistPM()
{
    IOService *policyMaker = NULL;
    thread_call_t offCall = NULL;
    thread_call_t onCall = NULL;
    if (lifecycleLock) {
        IOInterruptState interruptState =
            IOSimpleLockLockDisableInterrupt(lifecycleLock);
        policyMaker = pmPolicyMaker;
        if (policyMaker)
            policyMaker->retain();
        offCall = powerOffThreadCall;
        onCall = powerOnThreadCall;
        powerOffThreadCall = NULL;
        powerOnThreadCall = NULL;
        IOSimpleLockUnlockEnableInterrupt(lifecycleLock, interruptState);
    } else {
        policyMaker = pmPolicyMaker;
        if (policyMaker)
            policyMaker->retain();
        offCall = powerOffThreadCall;
        onCall = powerOnThreadCall;
        powerOffThreadCall = NULL;
        powerOnThreadCall = NULL;
    }

    unsigned cancelledRetains = 0;
    if (offCall) {
        if (thread_call_cancel_wait(offCall))
            cancelledRetains++;
        if (!thread_call_free(offCall))
            DRVERR("failed to free drained power-off thread call");
    }
    if (onCall) {
        if (thread_call_cancel_wait(onCall))
            cancelledRetains++;
        if (!thread_call_free(onCall))
            DRVERR("failed to free drained power-on thread call");
    }
    // Each newly queued transition returned a five-second deferred ack. A
    // cancelled callback will never reach its gated trampoline, so acknowledge
    // it here before dropping the borrowed policy-maker pointer.
    unsigned cancelledAcks = cancelledRetains;
    while (cancelledAcks-- != 0 && policyMaker)
        policyMaker->acknowledgeSetPowerState();

    if (lifecycleLock) {
        IOInterruptState interruptState =
            IOSimpleLockLockDisableInterrupt(lifecycleLock);
        if (pmPolicyMaker == policyMaker)
            pmPolicyMaker = NULL;
        IOSimpleLockUnlockEnableInterrupt(lifecycleLock, interruptState);
    } else if (pmPolicyMaker == policyMaker) {
        pmPolicyMaker = NULL;
    }

    // setPowerState also retained once for each newly queued callback.
    while (cancelledRetains-- != 0)
        release();
    if (policyMaker)
        policyMaker->release();
}

IOReturn AirportSequoia::setPowerState(unsigned long powerStateOrdinal, IOService *policyMaker)
{
    (void)policyMaker;
    IOReturn result = IOPMAckImplied;
    bool dropRetain = false;

    if (!lifecycleLock)
        return result;
    IOInterruptState interruptState =
        IOSimpleLockLockDisableInterrupt(lifecycleLock);
    if (!stopping && pmPowerState != powerStateOrdinal) {
        thread_call_t call = NULL;
        switch (powerStateOrdinal) {
            case kPowerStateOff:
                call = powerOffThreadCall;
                break;
            case kPowerStateOn:
                call = powerOnThreadCall;
                break;
            default:
                break;
        }
        if (call) {
            // Keep both the controller and the thread-call object alive across
            // the hand-off. unregistPM cannot extract/free call while this
            // lock is held.
            retain();
            if (thread_call_enter(call)) {
                dropRetain = true;
            } else {
                result = 5000000;
            }
        }
    }
    IOSimpleLockUnlockEnableInterrupt(lifecycleLock, interruptState);
    if (dropRetain)
        release();
    return result;
}

IOReturn AirportSequoia::setWakeOnMagicPacket(bool active)
{
    magicPacketEnabled = active;
    return kIOReturnSuccess;
}

IOReturn AirportSequoia::
setPowerStateOffGated(OSObject *owner, void *arg0, void *arg1, void *arg2,
                      void *arg3)
{
    (void)arg0;
    (void)arg1;
    (void)arg2;
    (void)arg3;
    AirportSequoia *self = OSDynamicCast(AirportSequoia, owner);
    if (!self)
        return kIOReturnBadArgument;
    if (!self->isStopping())
        self->setPowerStateOff();
    else
        self->acknowledgePowerTransition();
    self->release();
    return kIOReturnSuccess;
}

IOReturn AirportSequoia::
setPowerStateOnGated(OSObject *owner, void *arg0, void *arg1, void *arg2,
                     void *arg3)
{
    (void)arg0;
    (void)arg1;
    (void)arg2;
    (void)arg3;
    AirportSequoia *self = OSDynamicCast(AirportSequoia, owner);
    if (!self)
        return kIOReturnBadArgument;
    if (!self->isStopping())
        self->setPowerStateOn();
    else
        self->acknowledgePowerTransition();
    self->release();
    return kIOReturnSuccess;
}

static void handleSetPowerStateOff(thread_call_param_t param0, thread_call_param_t param1)
{
    (void)param1;
    AirportSequoia *self = (AirportSequoia *)param0;
    if (!self)
        return;
    IOCommandGate *gate = self->retainMainCommandGate();
    if (gate) {
        IOReturn result =
            gate->runAction(AirportSequoia::setPowerStateOffGated);
        gate->release();
        if (result == kIOReturnSuccess)
            return;
    }
    self->acknowledgePowerTransition();
    self->release();
}

static void handleSetPowerStateOn(thread_call_param_t param0, thread_call_param_t param1)
{
    (void)param1;
    AirportSequoia *self = (AirportSequoia *)param0;
    if (!self)
        return;
    IOCommandGate *gate = self->retainMainCommandGate();
    if (gate) {
        IOReturn result =
            gate->runAction(AirportSequoia::setPowerStateOnGated);
        gate->release();
        if (result == kIOReturnSuccess)
            return;
    }
    self->acknowledgePowerTransition();
    self->release();
}

IOReturn AirportSequoia::registerWithPolicyMaker(IOService *policyMaker)
{
    if (!policyMaker)
        return kIOReturnBadArgument;
    unregistPM();

    thread_call_t offCall =
        thread_call_allocate(handleSetPowerStateOff,
                             (thread_call_param_t)this);
    thread_call_t onCall =
        thread_call_allocate(handleSetPowerStateOn,
                             (thread_call_param_t)this);
    if (!offCall || !onCall) {
        if (offCall)
            thread_call_free(offCall);
        if (onCall)
            thread_call_free(onCall);
        return kIOReturnNoMemory;
    }

    IOInterruptState interruptState =
        IOSimpleLockLockDisableInterrupt(lifecycleLock);
    bool published = !stopping;
    if (published) {
        pmPowerState = kPowerStateOn;
        pmPolicyMaker = policyMaker;
        powerOffThreadCall = offCall;
        powerOnThreadCall = onCall;
    }
    IOSimpleLockUnlockEnableInterrupt(lifecycleLock, interruptState);
    if (!published) {
        thread_call_free(offCall);
        thread_call_free(onCall);
        return kIOReturnNotReady;
    }

    IOReturn ret =
        policyMaker->registerPowerDriver(this, powerStateArray,
                                         kPowerStateCount);
    if (ret != kIOReturnSuccess)
        unregistPM();
    return ret;
}

void AirportSequoia::acknowledgePowerTransition()
{
    IOService *policyMaker = NULL;
    if (lifecycleLock) {
        IOInterruptState interruptState =
            IOSimpleLockLockDisableInterrupt(lifecycleLock);
        policyMaker = pmPolicyMaker;
        if (policyMaker)
            policyMaker->retain();
        IOSimpleLockUnlockEnableInterrupt(lifecycleLock, interruptState);
    }
    if (policyMaker) {
        policyMaker->acknowledgeSetPowerState();
        policyMaker->release();
    }
}

void AirportSequoia::setPowerStateOff()
{
    DRVLOG("%s\n", __FUNCTION__);
    disableAdapter(bsdInterface);
    if (lifecycleLock) {
        IOInterruptState interruptState =
            IOSimpleLockLockDisableInterrupt(lifecycleLock);
        pmPowerState = kPowerStateOff;
        IOSimpleLockUnlockEnableInterrupt(lifecycleLock, interruptState);
    }
    acknowledgePowerTransition();
}

void AirportSequoia::setPowerStateOn()
{
    DRVLOG("%s\n", __FUNCTION__);
    IOReturn ret =
        enableAdapter((IONetworkInterface *)bsdInterface);
    if (ret == kIOReturnSuccess) {
        if (lifecycleLock) {
            IOInterruptState interruptState =
                IOSimpleLockLockDisableInterrupt(lifecycleLock);
            pmPowerState = kPowerStateOn;
            IOSimpleLockUnlockEnableInterrupt(lifecycleLock, interruptState);
        }
    } else {
        DRVERR("wake enableAdapter failed: 0x%x", ret);
    }
    acknowledgePowerTransition();
}
