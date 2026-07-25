//
//  AirportSequoia.hpp
//  Нативный Intel Wi-Fi драйвер для macOS Sequoia (15.x)
//  Архитектура: IO80211Controller + IO80211InfraInterface (Skywalk)
//  Базовые классы восстановлены из IO80211Family 15.7.4 (24G517, KDK)
//

#ifndef AIRPORT_SEQUOIA_HPP
#define AIRPORT_SEQUOIA_HPP

#include <IOKit/IOService.h>
#include <IOKit/pci/IOPCIDevice.h>
#include <IOKit/network/IOEthernetController.h>
#include <Airport/Sequoia/IO80211Sequoia.h>

#define AIRPORT_SEQUOIA_VERSION "0.1.0"

// Интерфейсный объект: живёт в стеке Skywalk, отдаёт системе get/set 80211 API.
// TODO(этап 2): реализовать get/set поверх net80211 (OpenBSD-стек itlwm).
class AirportSequoiaInfra : public IO80211InfraInterface {
    OSDeclareDefaultStructors(AirportSequoiaInfra)

public:
    bool init(IOService *provider) override;
    void free() override;
};

// Контроллер драйвера: цепляется к PCI-устройству Intel,
// поднимает firmware (TODO: интеграция с HAL itlwm) и создаёт интерфейсы.
class AirportSequoia : public IO80211Controller {
    OSDeclareDefaultStructors(AirportSequoia)

public:
    IOService * probe(IOService *provider, SInt32 *score) override;
    bool start(IOService *provider) override;
    void stop(IOService *provider) override;
    void free() override;

    // --- IO80211Controller: минимально необходимые переопределения (этап 1) ---
    // TODO(этап 1): createInterface / createVirtualInterface по сигнатурам из IO80211Controller_Sequoia.h
    // TODO(этап 1): getHardwareAssists, enable/disable, outputPacket

private:
    IOPCIDevice *fPCIDevice;
    AirportSequoiaInfra *fInfraInterface;
    IOWorkLoop *fWorkLoop;
};

#endif /* AIRPORT_SEQUOIA_HPP */
