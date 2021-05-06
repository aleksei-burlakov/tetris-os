#ifndef PCI_H
#define PCI_H

void pci_init();
void pci_enumerate();
u16 pci_get_vendor(u8 bus, u8 device);
u16 pci_config_readword(u8 bus, u8 device, u8 func, u8 offset);

#endif PCI_H