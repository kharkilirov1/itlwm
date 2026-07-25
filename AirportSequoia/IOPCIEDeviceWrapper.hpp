//
//  IOPCIEDeviceWrapper.hpp
//  PCI-prober: матчит Intel-карту и создаёт HAL (iwx/iwm/iwn).
//

#ifndef IOPCIE_DEVICE_WRAPPER_HPP
#define IOPCIE_DEVICE_WRAPPER_HPP

#include <IOKit/IOService.h>
#include <IOKit/pci/IOPCIDevice.h>
#include <IOKit/IOLib.h>
#include <IOKit/IOTypes.h>

#include <HAL/ItlHalService.hpp>

class IOPCIEDeviceWrapper : public IOService {
    OSDeclareDefaultStructors(IOPCIEDeviceWrapper)

public:
    IOService * probe(IOService *provider, SInt32 *score) override;
    bool start(IOService *provider) override;
    void stop(IOService *provider) override;
    IOWorkLoop * getWorkLoop() const override;
    IOReturn setPowerState(unsigned long powerStateOrdinal, IOService *whatDevice) override;

public:
    ItlHalService *fHalService;
    IOPCIDevice *pciNub;
};

#endif /* IOPCIE_DEVICE_WRAPPER_HPP */
