/** @file
*  Secondary System Description Table (SSDT)
*
*  Copyright (c) 2021, ARM Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/

#include "SgiAcpiHeader.h"

DefinitionBlock("SsdtPci.aml", "SSDT", 2, "ARMLTD", "ARMSGI", EFI_ACPI_ARM_OEM_REVISION) {
  Scope (_SB) {
    // PCI Root Complexes
    EFI_ACPI_PCI_RC_INIT(0, 0, 0, 255, 256, 0x60000000, 0x7FFFFFFF, 0x20000000, 0x4000000000, 0x7FFFFFFFFF, 0x4000000000, 0x1010000000, 0x101FFFFFFF, 0x10000000)
  }
}
