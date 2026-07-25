//
//  AirportSequoiaEthernetInterface.hpp
//  BSD-интерфейс (IOEthernetInterface) поверх Skywalk-интерфейса.
//

#ifndef AIRPORT_SEQUOIA_ETHERNET_INTERFACE_HPP
#define AIRPORT_SEQUOIA_ETHERNET_INTERFACE_HPP

extern "C" {
#include <net/bpf.h>
}
#include <IOKit/IOLib.h>
#include <libkern/OSKextLib.h>
#include <sys/kernel_types.h>
#include <IOKit/network/IOEthernetInterface.h>
#include <Airport/apple80211_var.h>

class IO80211SkywalkInterface;

class AirportSequoiaEthernetInterface : public IOEthernetInterface {
    OSDeclareDefaultStructors(AirportSequoiaEthernetInterface)

public:
    IOReturn attachToDataLinkLayer(IOOptionBits options, void *parameter) override;
    void detachFromDataLinkLayer(IOOptionBits options, void *parameter) override;

    bool initWithSkywalkInterfaceAndProvider(IONetworkController *controller, IO80211SkywalkInterface *interface);

    bool setLinkState(IO80211LinkState state);

    static errno_t bpfOutputPacket(ifnet_t interface, u_int32_t data_link_type, mbuf_t packet);
    static errno_t bpfTap(ifnet_t interface, u_int32_t data_link_type, bpf_tap_mode direction);

    UInt32 inputPacket(mbuf_t packet, UInt32 length = 0, IOOptionBits options = 0, void *param = 0) override;

    IOService * getProvider(void) const override;

private:
    IO80211SkywalkInterface *interface;
    bool isAttach;
};

#endif /* AIRPORT_SEQUOIA_ETHERNET_INTERFACE_HPP */
