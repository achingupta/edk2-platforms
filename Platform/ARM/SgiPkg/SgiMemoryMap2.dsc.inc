#
#  Copyright (c) 2020, ARM Limited. All rights reserved.
#
#  SPDX-License-Identifier: BSD-2-Clause-Patent
#

[PcdsFixedAtBuild.common]
  # System Peripherals
  gArmSgiTokenSpaceGuid.PcdSmcCs0Base|0x08000000
  gArmSgiTokenSpaceGuid.PcdSmcCs1Base|0x1050000000
  gArmSgiTokenSpaceGuid.PcdSysPeriphBase|0x0C000000
  gArmSgiTokenSpaceGuid.PcdSysPeriphSysRegBase|0x0C010000

  # Non-Volatile variable storage
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageVariableBase64|0x1050000000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwWorkingBase64|0x1051400000
  gEfiMdeModulePkgTokenSpaceGuid.PcdFlashNvStorageFtwSpareBase64|0x1052800000

  # PL011 - Serial Terminal
  gEfiMdeModulePkgTokenSpaceGuid.PcdSerialRegisterBase|0x0EF70000

  # PL370 - HDLCD1
  gArmPlatformTokenSpaceGuid.PcdArmHdLcdBase|0x0EF60000

  # PL011 - Serial Debug UART
  gArmPlatformTokenSpaceGuid.PcdSerialDbgRegisterBase|0x0EF70000
  gArmPlatformTokenSpaceGuid.PL011UartInterrupt|403

  # PL031 RealTimeClock
  gArmPlatformTokenSpaceGuid.PcdPL031RtcBase|0x0C170000

  # Virtio Disk
  gArmSgiTokenSpaceGuid.PcdVirtioBlkBaseAddress|0x0C130000
  gArmSgiTokenSpaceGuid.PcdVirtioBlkSize|0x10000
  gArmSgiTokenSpaceGuid.PcdVirtioBlkInterrupt|458

  # Ethernet
  gArmSgiTokenSpaceGuid.PcdVirtioNetBaseAddress|0x0C150000
  gArmSgiTokenSpaceGuid.PcdVirtioNetInterrupt|460

  # PCIe
  gArmTokenSpaceGuid.PcdPciMmio32Base|0x60000000
  gArmTokenSpaceGuid.PcdPciMmio32Size|0x10000000
  gArmTokenSpaceGuid.PcdPciMmio64Base|0x4000000000
  gArmTokenSpaceGuid.PcdPciMmio64Size|0x4000000000
  gEfiMdePkgTokenSpaceGuid.PcdPciExpressBaseAddress|0x1010000000

  # Timer & Watchdog interrupts
  gArmSgiTokenSpaceGuid.PcdGtFrame0Gsiv|109
  gArmSgiTokenSpaceGuid.PcdGtFrame1Gsiv|108
  gArmSgiTokenSpaceGuid.PcdWdogWS0Gsiv|110
  gArmSgiTokenSpaceGuid.PcdWdogWS1Gsiv|111

  # SP804 dual timer
  gArmSgiTokenSpaceGuid.PcdSp804DualTimerBaseAddress|0x0C110000
  gArmSgiTokenSpaceGuid.PcdSp804DualTimerSize|0x00010000
  gArmSgiTokenSpaceGuid.PcdSp804DualTimerInterrupt|486

  # SMMU
  gArmSgiTokenSpaceGuid.PcdSmmuBase|0x40000000
  gArmSgiTokenSpaceGuid.PcdSmmuSize|0x10000000
  gArmSgiTokenSpaceGuid.PcdSmmuDevIDBase|0x80000

  # GPIO controller
  gArmSgiTokenSpaceGuid.PcdGpioController0BaseAddress|0x0C1D0000
  gArmSgiTokenSpaceGuid.PcdGpioController0Size|0x00010000
  gArmSgiTokenSpaceGuid.PcdGpioController0Interrupt|392

  # IO virtualization block UARTs
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkUart0Base|0xC00000000000
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkUart0End|0xC0000000FFFF
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkUart0Size|0x10000
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkUart0Interrupt|492

  gArmSgiTokenSpaceGuid.PcdIoVirtBlkUart1Base|0xC00020000000
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkUart1End|0xC0002000FFFF
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkUart1Size|0x10000
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkUart1Interrupt|502

  # IO virtualization block PL330 DMA controllers
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkDma0BaseAddress|0xC00010000000
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkDma0EndAddress|0xC0001000FFFF
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkDma0Size|0x10000
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkDma0NumCh|8
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkDma0Ch0Gsiv|493
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkDma0Ch1Gsiv|494
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkDma0Ch2Gsiv|495
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkDma0Ch3Gsiv|496
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkDma0Ch4Gsiv|497
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkDma0Ch5Gsiv|498
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkDma0Ch6Gsiv|499
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkDma0Ch7Gsiv|500
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkDma0AbortGsiv|501

  gArmSgiTokenSpaceGuid.PcdIoVirtBlkDma1BaseAddress|0xC00030000000
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkDma1EndAddress|0xC0003000FFFF
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkDma1Size|0x10000
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkDma1NumCh|8
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkDma1Ch0Gsiv|503
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkDma1Ch1Gsiv|504
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkDma1Ch2Gsiv|505
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkDma1Ch3Gsiv|506
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkDma1Ch4Gsiv|507
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkDma1Ch5Gsiv|508
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkDma1Ch6Gsiv|509
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkDma1Ch7Gsiv|510
  gArmSgiTokenSpaceGuid.PcdIoVirtBlkDma1AbortGsiv|511
