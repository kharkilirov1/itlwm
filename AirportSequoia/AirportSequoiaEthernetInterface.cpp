//
//  AirportSequoiaEthernetInterface.cpp
//

#include "AirportSequoiaEthernetInterface.hpp"
#include <Airport/Sequoia/IO80211Sequoia.h>

#include <sys/_if_ether.h>
#include <net80211/ieee80211_var.h>

#define super IOEthernetInterface
OSDefineMetaClassAndStructors(AirportSequoiaEthernetInterface, IOEthernetInterface);

extern const char* hexdump(uint8_t *buf, size_t len);

bool AirportSequoiaEthernetInterface::
initWithSkywalkInterfaceAndProvider(IONetworkController *controller, IO80211SkywalkInterface *interface)
{
    bool ret = super::init(controller);
    if (ret)
        this->interface = interface;
    this->isAttach = false;
    return ret;
}

IOReturn AirportSequoiaEthernetInterface::
attachToDataLinkLayer(IOOptionBits options, void *parameter)
{
    XYLog("%s\n", __FUNCTION__);
    char infName[IFNAMSIZ];
    IOReturn ret = super::attachToDataLinkLayer(options, parameter);
    if (ret == kIOReturnSuccess && interface) {
        UInt8 builtIn = 0;
        IOEthernetAddress addr;
        interface->setProperty("built-in", OSData::withBytes(&builtIn, sizeof(builtIn)));
        snprintf(infName, sizeof(infName), "%s%u", ifnet_name(getIfnet()), ifnet_unit(getIfnet()));
        interface->setProperty("IOInterfaceName", OSString::withCString(infName));
        interface->setProperty(kIOInterfaceUnit, OSNumber::withNumber(ifnet_unit(getIfnet()), 8));
        interface->setProperty(kIOInterfaceNamePrefix, OSString::withCString(ifnet_name(getIfnet())));
        if (OSDynamicCast(IOEthernetController, getController())->getHardwareAddress(&addr) == kIOReturnSuccess)
            setProperty(kIOMACAddress, (void *)&addr, kIOEthernetAddressSize);
        interface->registerService();
        interface->prepareBSDInterface(getIfnet(), 0);
    }
    isAttach = true;
    return ret;
}

void AirportSequoiaEthernetInterface::
detachFromDataLinkLayer(IOOptionBits options, void *parameter)
{
    super::detachFromDataLinkLayer(options, parameter);
    isAttach = false;
}

/**
 * Хак: после attach отдаём skywalk-интерфейс как provider,
 * чтобы IOSkywalkFamily не падал на касте.
 */
IOService *AirportSequoiaEthernetInterface::
getProvider() const
{
    return isAttach ? (IOService *)this->interface : super::getProvider();
}

errno_t AirportSequoiaEthernetInterface::
bpfOutputPacket(ifnet_t interface, u_int32_t data_link_type, mbuf_t packet)
{
    XYLog("%s data_link_type: %d\n", __FUNCTION__, data_link_type);
    AirportSequoiaEthernetInterface *networkInterface = (AirportSequoiaEthernetInterface *)ifnet_softc(interface);
    return networkInterface->enqueueOutputPacket(packet);
}

errno_t AirportSequoiaEthernetInterface::
bpfTap(ifnet_t interface, u_int32_t data_link_type, bpf_tap_mode direction)
{
    XYLog("%s data_link_type: %d direction: %d\n", __FUNCTION__, data_link_type, direction);
    return 0;
}

bool AirportSequoiaEthernetInterface::
setLinkState(IO80211LinkState state)
{
    if (state == kIO80211NetworkLinkUp) {
        ifnet_set_flags(getIfnet(), ifnet_flags(getIfnet()) | (IFF_UP | IFF_RUNNING), (IFF_UP | IFF_RUNNING));
    } else {
        ifnet_set_flags(getIfnet(), ifnet_flags(getIfnet()) & ~(IFF_UP | IFF_RUNNING), 0);
    }
    return true;
}

UInt32 AirportSequoiaEthernetInterface::
inputPacket(mbuf_t packet, UInt32 length, IOOptionBits options, void *param)
{
    ether_header_t *eh;
    size_t len = mbuf_len(packet);

    eh = (ether_header_t *)mbuf_data(packet);
    if (len >= sizeof(ether_header_t) && eh->ether_type == htons(ETHERTYPE_PAE)) { // EAPOL
        const char* dump = hexdump((uint8_t*)mbuf_data(packet), len);
        XYLog("input EAPOL packet, len: %zu, data: %s\n", len, dump ? dump : "no mem");
        if (dump)
            IOFree((void*)dump, 3 * len + 1);
    }
    return IOEthernetInterface::inputPacket(packet, length, options, param);
}
