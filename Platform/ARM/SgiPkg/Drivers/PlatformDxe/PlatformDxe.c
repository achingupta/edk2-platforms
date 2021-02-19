/** @file
*
*  Copyright (c) 2018, ARM Limited. All rights reserved.
*
*  SPDX-License-Identifier: BSD-2-Clause-Patent
*
**/
#include <IndustryStandard/Acpi.h>

#include <Library/AcpiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <SgiPlatform.h>

#define EINJ_TRIGGER_ERROR_ACTION_NO    2

typedef struct {
  EFI_ACPI_6_3_EINJ_TRIGGER_ACTION_TABLE        TriggerErrorHeader;
  EFI_ACPI_6_3_EINJ_INJECTION_INSTRUCTION_ENTRY ErrorInstructionEntry[EINJ_TRIGGER_ERROR_ACTION_NO];
} EINJ_TRIGGER_ERROR_ACTION;

VOID
InitVirtioDevices (
  VOID
  );

EFI_STATUS
EFIAPI
ArmSgiPkgEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS              Status;

  Status = LocateAndInstallAcpiFromFv (&gArmSgiAcpiTablesGuid);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Failed to install ACPI tables\n", __FUNCTION__));
    return Status;
  }

  InitVirtioDevices ();

  if (FeaturePcdGet (PcdEinjSupported)) {
    // Initialize the EINJ Instruction Buffer memory space
    ZeroMem (
      (VOID *)(PcdGet64 (PcdEinjInstBufferBase)),
      PcdGet64 (PcdEinjInstBufferSize)
    );

    // Populate Trigger Action table at address 0xFF631000 which is pointed by
    // Get Trigger Error Action instruction.
    EINJ_TRIGGER_ERROR_ACTION *mEinjTriggerErrorAction =
                                (EINJ_TRIGGER_ERROR_ACTION*) 0xFF631000;

    // Trigger Error Action Table.
    *mEinjTriggerErrorAction = (EINJ_TRIGGER_ERROR_ACTION) {
      {
        sizeof (EFI_ACPI_6_3_EINJ_TRIGGER_ACTION_TABLE),   // Header Size
        0,   // Revision
        sizeof (EINJ_TRIGGER_ERROR_ACTION),   // Table Size
        EINJ_TRIGGER_ERROR_ACTION_NO   // Entry Count
      },
      {
        // Trigger Error Instruction 1
        // Populates DMC error injection payload field(information).
        {
          EFI_ACPI_6_3_EINJ_TRIGGER_ERROR,
          EFI_ACPI_6_3_EINJ_WRITE_REGISTER_VALUE,
          0,   // Flags
          0,   // Reserved
          {
            EFI_ACPI_6_3_SYSTEM_MEMORY,
            32,
            0,
            EFI_ACPI_6_3_DWORD,
            0x0600051C,   // base address of SCP payload structure
          },
          0x0000000000000000,   // Inject 1 bit error into DMC0 instance.
          0xffffffffffffffff    // Mask
        },
        // Trigger Error Instruction 2
        // MHU programming for doorbell event to SCP
        {
          EFI_ACPI_6_3_EINJ_TRIGGER_ERROR,
          EFI_ACPI_6_3_EINJ_WRITE_REGISTER_VALUE,
          0,   // Flag
          0,   // Reserved
          {
            EFI_ACPI_6_3_SYSTEM_MEMORY,
            32,
            0,
            EFI_ACPI_6_3_DWORD,
            (FixedPcdGet64 (PcdMhuBaseAddress) +
              FixedPcdGet32 (PcdMhuChannelSetOffset))
          },
          0x0000000000000002,   // Value indicating DMC error injection
          0xffffffffffffffff    // Register region Mask
        },
      }
    };
  }
  return Status;
}
