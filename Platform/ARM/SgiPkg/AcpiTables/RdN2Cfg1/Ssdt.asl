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
    EFI_ACPI_PCI_RC_INIT(0, 0, 0, 127, 128, 0x60000000, 0x6FFFFFFF, 0x10000000, 0x4000000000, 0x5FFFFFFFFF, 0x2000000000, 0x1010000000, 0x1017FFFFFF, 0x8000000)
  }
}
