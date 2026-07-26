#ifndef ITLWM_SEQUOIA_IONETWORKCONTROLLER_OVERLAY_H
#define ITLWM_SEQUOIA_IONETWORKCONTROLLER_OVERLAY_H

#if !defined(ITLWM_SEQUOIA_ABI)

// Keep every non-Sequoia target on the unmodified SDK declaration.
#include "../../../MacKernelSDK/Headers/IOKit/network/IONetworkController.h"

#else

// Preload the dependencies of the SDK header before temporarily replacing
// its reserved-slot declaration macro. This confines the replacement to
// IONetworkController itself.
#include <Availability.h>
#include <IOKit/IOService.h>
#include <IOKit/IOWorkLoop.h>
#include <IOKit/network/IONetworkInterface.h>
#include <IOKit/network/IOKernelDebugger.h>

#pragma push_macro("OSMetaClassDeclareReservedUnused")
#undef OSMetaClassDeclareReservedUnused

#if APPLE_KEXT_VTABLE_PADDING
#define ITLWM_IONC_UNUSED(c, i) private: virtual void _RESERVED ## c ## i ()
#else
#define ITLWM_IONC_UNUSED(c, i)
#endif

#define ITLWM_IONC_SLOT_0(c)  ITLWM_IONC_UNUSED(c, 0)
#define ITLWM_IONC_SLOT_1(c)  ITLWM_IONC_UNUSED(c, 1)
#define ITLWM_IONC_SLOT_2(c)  ITLWM_IONC_UNUSED(c, 2)
#define ITLWM_IONC_SLOT_3(c)  ITLWM_IONC_UNUSED(c, 3)
#define ITLWM_IONC_SLOT_4(c)  ITLWM_IONC_UNUSED(c, 4)
#define ITLWM_IONC_SLOT_5(c)  ITLWM_IONC_UNUSED(c, 5)
#define ITLWM_IONC_SLOT_6(c)  public: virtual mbuf_t allocatePacketNoWait(UInt32 size)
#define ITLWM_IONC_SLOT_7(c)  public: virtual IOReturn setHardwareAssists(UInt32 hardwareAssists, UInt32 hardwareAssistsMask)
#define ITLWM_IONC_SLOT_8(c)  ITLWM_IONC_UNUSED(c, 8)
#define ITLWM_IONC_SLOT_9(c)  ITLWM_IONC_UNUSED(c, 9)
#define ITLWM_IONC_SLOT_10(c) ITLWM_IONC_UNUSED(c, 10)
#define ITLWM_IONC_SLOT_11(c) ITLWM_IONC_UNUSED(c, 11)
#define ITLWM_IONC_SLOT_12(c) ITLWM_IONC_UNUSED(c, 12)
#define ITLWM_IONC_SLOT_13(c) ITLWM_IONC_UNUSED(c, 13)
#define ITLWM_IONC_SLOT_14(c) ITLWM_IONC_UNUSED(c, 14)
#define ITLWM_IONC_SLOT_15(c) ITLWM_IONC_UNUSED(c, 15)
#define ITLWM_IONC_SLOT_16(c) ITLWM_IONC_UNUSED(c, 16)
#define ITLWM_IONC_SLOT_17(c) ITLWM_IONC_UNUSED(c, 17)
#define ITLWM_IONC_SLOT_18(c) ITLWM_IONC_UNUSED(c, 18)
#define ITLWM_IONC_SLOT_19(c) ITLWM_IONC_UNUSED(c, 19)
#define ITLWM_IONC_SLOT_20(c) ITLWM_IONC_UNUSED(c, 20)
#define ITLWM_IONC_SLOT_21(c) ITLWM_IONC_UNUSED(c, 21)
#define ITLWM_IONC_SLOT_22(c) ITLWM_IONC_UNUSED(c, 22)
#define ITLWM_IONC_SLOT_23(c) ITLWM_IONC_UNUSED(c, 23)
#define ITLWM_IONC_SLOT_24(c) ITLWM_IONC_UNUSED(c, 24)
#define ITLWM_IONC_SLOT_25(c) ITLWM_IONC_UNUSED(c, 25)
#define ITLWM_IONC_SLOT_26(c) ITLWM_IONC_UNUSED(c, 26)
#define ITLWM_IONC_SLOT_27(c) ITLWM_IONC_UNUSED(c, 27)
#define ITLWM_IONC_SLOT_28(c) ITLWM_IONC_UNUSED(c, 28)
#define ITLWM_IONC_SLOT_29(c) ITLWM_IONC_UNUSED(c, 29)
#define ITLWM_IONC_SLOT_30(c) ITLWM_IONC_UNUSED(c, 30)
#define ITLWM_IONC_SLOT_31(c) ITLWM_IONC_UNUSED(c, 31)

#define ITLWM_IONC_DISPATCH_I(c, i) ITLWM_IONC_SLOT_ ## i(c)
#define ITLWM_IONC_DISPATCH(c, i) ITLWM_IONC_DISPATCH_I(c, i)
#define OSMetaClassDeclareReservedUnused(c, i) ITLWM_IONC_DISPATCH(c, i)

// Use the repository-pinned SDK explicitly so header-search reordering cannot
// make #include_next resolve this overlay twice.
#include "../../../MacKernelSDK/Headers/IOKit/network/IONetworkController.h"

#pragma pop_macro("OSMetaClassDeclareReservedUnused")
#undef ITLWM_IONC_DISPATCH
#undef ITLWM_IONC_DISPATCH_I
#undef ITLWM_IONC_UNUSED
#undef ITLWM_IONC_SLOT_0
#undef ITLWM_IONC_SLOT_1
#undef ITLWM_IONC_SLOT_2
#undef ITLWM_IONC_SLOT_3
#undef ITLWM_IONC_SLOT_4
#undef ITLWM_IONC_SLOT_5
#undef ITLWM_IONC_SLOT_6
#undef ITLWM_IONC_SLOT_7
#undef ITLWM_IONC_SLOT_8
#undef ITLWM_IONC_SLOT_9
#undef ITLWM_IONC_SLOT_10
#undef ITLWM_IONC_SLOT_11
#undef ITLWM_IONC_SLOT_12
#undef ITLWM_IONC_SLOT_13
#undef ITLWM_IONC_SLOT_14
#undef ITLWM_IONC_SLOT_15
#undef ITLWM_IONC_SLOT_16
#undef ITLWM_IONC_SLOT_17
#undef ITLWM_IONC_SLOT_18
#undef ITLWM_IONC_SLOT_19
#undef ITLWM_IONC_SLOT_20
#undef ITLWM_IONC_SLOT_21
#undef ITLWM_IONC_SLOT_22
#undef ITLWM_IONC_SLOT_23
#undef ITLWM_IONC_SLOT_24
#undef ITLWM_IONC_SLOT_25
#undef ITLWM_IONC_SLOT_26
#undef ITLWM_IONC_SLOT_27
#undef ITLWM_IONC_SLOT_28
#undef ITLWM_IONC_SLOT_29
#undef ITLWM_IONC_SLOT_30
#undef ITLWM_IONC_SLOT_31

#endif /* ITLWM_SEQUOIA_ABI */

#endif
