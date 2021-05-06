#include "timer.h"
#include "pci.h"
#include "util.h"

#define PCI_CONFIG_ADDRESS 0xCF8
#define PCI_CONFIG_DATA 0xCFC

char buf0[128], buf1[128];

void pci_init()
{
    pci_enumerate();
}

void pci_enumerate()
{
    textMode_print("PCI Devices\r\n");
    textMode_print("VEN_ID DEV_ID TYPE FUNCTION\r\n");


    for (u16 bus = 0; bus < 256; bus++)
    {
        for (u8 device = 0; device < 32; device++)
        {
            pci_check_vendor(bus, device);
        }
    }

    timer_waitTicks(TIMER_TPS * 1000);
}

void pci_check_vendor(u8 bus, u8 device) 
{
    u16 vendorId = pci_get_vendor(bus, device);
    if (vendorId != 0xffff)
    {
        u16 deviceId = pci_config_readword(bus, device, 0, 2);

        strlcat(buf0, " ", 128);
        itoa(vendorId, buf1, 16, 128);
        strlcat(buf0, buf1, 128);
        strlcat(buf0, "   ", 128);
        itoa(deviceId, buf1, 16, 128);
        strlcat(buf0, buf1, 128);
        strlcat(buf0, "\r\n", 128);
        textMode_print(buf0);
    }
}

u16 pci_config_readword(u8 bus, u8 device, u8 func, u8 offset)
{
    u32 lbus = (u32) bus;
    u32 ldevice = (u32) device;
    u32 lfunc = (u32) func;
    u16 tmp = 0;

    u32 address = (u32) ((lbus << 16) | (ldevice << 11) | (lfunc << 8) | (offset & 0xfc) | ((u32) 0x80000000));

    outportl(PCI_CONFIG_ADDRESS, address);
    tmp = (u16) ((inportl(PCI_CONFIG_DATA) >> ((offset & 2) << 3)) & 0xffff);
    return (tmp);
}

u16 pci_get_vendor(u8 bus, u8 device)
{
    return pci_config_readword(bus, device, 0, 0);
}