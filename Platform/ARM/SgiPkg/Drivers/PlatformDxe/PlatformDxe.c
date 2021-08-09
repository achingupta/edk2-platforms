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
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PL011UartLib.h>

#include <Protocol/AcpiTable.h>
#include <Protocol/AcpiSystemDescriptionTable.h>
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

/**
  Initialize UART controllers connected to IO Virtualization block.

  Use PL011UartLib Library to initialize UART controllers connected
  to X4_0 and X8 port of the IO Virtualization block.

  @retval Nothing
**/
STATIC
VOID
InitIoVirtBlkUartControllers (VOID)
{
  EFI_STATUS Status;
  UINT64 BaudRate;
  UINT32 ReceiveFifoDepth;
  EFI_PARITY_TYPE Parity;
  UINT8 DataBits;
  EFI_STOP_BITS_TYPE StopBits;

  if (!FeaturePcdGet (PcdIoVirtBlkNonDiscoverable))
    return;

  ReceiveFifoDepth = 0;
  Parity = 1;
  DataBits = 8;
  StopBits = 1;
  BaudRate = 115200;

  // Use PL011Uart Library to initialize the X4: PL011_UART0
  Status = PL011UartInitializePort (
             (UINTN)FixedPcdGet64 (PcdIoVirtBlkUart0Base),
             FixedPcdGet32 (PcdSerialDbgUartClkInHz),
             &BaudRate,
             &ReceiveFifoDepth,
             &Parity,
             &DataBits,
             &StopBits
             );

  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "Failed to initialize IO Virt Block X4_0: PL011_UART0, status: %r\n",
      Status
      ));
  }

  // Use PL011Uart Library to initialize the X8: PL011_UART1
  Status = PL011UartInitializePort (
             (UINTN)FixedPcdGet64 (PcdIoVirtBlkUart1Base),
             FixedPcdGet32 (PcdSerialDbgUartClkInHz),
             &BaudRate,
             &ReceiveFifoDepth,
             &Parity,
             &DataBits,
             &StopBits
             );

  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "Failed to initialize IO Virt Block X8: PL011_UART1, status: %r\n",
      Status
      ));
  }

}

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

  InitIoVirtBlkUartControllers ();

  /* ToDo: Cleanup the code below */
  if ((SgiGetProductId () != Sgi575) && (SgiGetProductId () != RdN1Edge))
    return EFI_SUCCESS;

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
  else {
    EFI_ACPI_SDT_PROTOCOL   *AcpiSdtProtocol = NULL;
    EFI_ACPI_TABLE_PROTOCOL *AcpiTableProtocol = NULL;
    EFI_ACPI_TABLE_VERSION  TableVersion;
    UINTN                   TableIndex = 0;
    UINTN                   TableKey = 0;
    VOID                    *TableHeader = NULL;

    Status = gBS->LocateProtocol(
                    &gEfiAcpiSdtProtocolGuid,
                    NULL,
                    (VOID**) &AcpiSdtProtocol
                    );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Status = gBS->LocateProtocol(
                    &gEfiAcpiTableProtocolGuid,
                    NULL,
                    (VOID**) &AcpiTableProtocol
                    );
    if (EFI_ERROR (Status)) {
      return Status;
    }

    while (!EFI_ERROR (Status)) {
      Status = AcpiSdtProtocol->GetAcpiTable (
                                   TableIndex,
                                   (EFI_ACPI_SDT_HEADER **)&TableHeader,
                                   &TableVersion,
                                   &TableKey
                                   );
      if (!EFI_ERROR (Status)) {
        TableIndex++;

      if (((EFI_ACPI_SDT_HEADER *)TableHeader)->Signature ==
            EFI_ACPI_6_3_ERROR_INJECTION_TABLE_SIGNATURE) {
          Status = AcpiTableProtocol->UninstallAcpiTable (
                                         AcpiTableProtocol,
                                         TableKey
                                         );
          if (EFI_ERROR (Status)) {
            return Status;
          }

	  break;
        }
      }
    }
  }

  return Status;
}
