/***************************************************************************//**
 * @file
 * @brief System Peripheral API
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/
// NOLINTBEGIN
#pragma GCC diagnostic ignored "-Wsign-conversion"

#include "em_system.h"
#include "sl_assert.h"
#include <stddef.h>
#if defined(SYSCFG_PRESENT)
#include "em_syscfg.h"
#endif
/***************************************************************************//**
 * @addtogroup system
 * @{
 ******************************************************************************/

/*******************************************************************************
 *********************************   DEFINES   *********************************
 ******************************************************************************/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/* Bit mask used to extract the part number value without the new naming
 * bitfield. */
#define SYSCFG_CHIPREV_PARTNUMBER1  0xFE0
#define SYSCFG_CHIPREV_PARTNUMBER0  0xF

/* Bit mask to convert NON-SECURE to SECURE */
#define CONVERT_NS_TO_S (~(1 << 28U))

/** @endcond */

/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Get a chip major/minor revision.
 *
 * @param[out] rev
 *   A location to place the chip revision information.
 ******************************************************************************/
void SYSTEM_ChipRevisionGet(SYSTEM_ChipRevision_TypeDef *rev)
{
#if defined(_SYSCFG_CHIPREV_FAMILY_MASK) || defined(_SYSCFG_CHIPREV_PARTNUMBER_MASK)
  /* On series-2 (and higher) the revision info is in the SYSCFG->CHIPREV register. */
#if defined(CMU_CLKEN0_SYSCFG)
  CMU->CLKEN0_SET = CMU_CLKEN0_SYSCFG;
#endif
  uint32_t chiprev = SYSCFG_readChipRev();
#if defined(_SYSCFG_CHIPREV_PARTNUMBER_MASK)
  rev->partNumber = ((chiprev & SYSCFG_CHIPREV_PARTNUMBER1) >> 5) | (chiprev & SYSCFG_CHIPREV_PARTNUMBER0);
#else
  rev->family = (chiprev & _SYSCFG_CHIPREV_FAMILY_MASK) >> _SYSCFG_CHIPREV_FAMILY_SHIFT;
#endif
  rev->major  = (chiprev & _SYSCFG_CHIPREV_MAJOR_MASK)  >> _SYSCFG_CHIPREV_MAJOR_SHIFT;
  rev->minor  = (chiprev & _SYSCFG_CHIPREV_MINOR_MASK)  >> _SYSCFG_CHIPREV_MINOR_SHIFT;
#else
  uint8_t tmp;

  EFM_ASSERT(rev);

  /* CHIP FAMILY bit [5:2] */
  tmp  = (uint8_t)(((ROMTABLE->PID1 & _ROMTABLE_PID1_FAMILYMSB_MASK)
                    >> _ROMTABLE_PID1_FAMILYMSB_SHIFT) << 2);
  /* CHIP FAMILY bit [1:0] */
  tmp |=  (uint8_t)((ROMTABLE->PID0 & _ROMTABLE_PID0_FAMILYLSB_MASK)
                    >> _ROMTABLE_PID0_FAMILYLSB_SHIFT);
  rev->family = tmp;

  /* CHIP MAJOR bit [3:0] */
  rev->major = (uint8_t)((ROMTABLE->PID0 & _ROMTABLE_PID0_REVMAJOR_MASK)
                         >> _ROMTABLE_PID0_REVMAJOR_SHIFT);

  /* CHIP MINOR bit [7:4] */
  tmp  = (uint8_t)(((ROMTABLE->PID2 & _ROMTABLE_PID2_REVMINORMSB_MASK)
                    >> _ROMTABLE_PID2_REVMINORMSB_SHIFT) << 4);
  /* CHIP MINOR bit [3:0] */
  tmp |= (uint8_t)((ROMTABLE->PID3 & _ROMTABLE_PID3_REVMINORLSB_MASK)
                   >> _ROMTABLE_PID3_REVMINORLSB_SHIFT);
  rev->minor = tmp;
#endif
}

/***************************************************************************//**
 * @brief
 *    Get a factory calibration value for a given peripheral register.
 *
 * @param[in] regAddress
 *    The peripheral calibration register address to get a calibration value for. If
 *    the calibration value is found, this register is updated with the
 *    calibration value.
 *
 * @return
 *    True if a calibration value exists, false otherwise.
 ******************************************************************************/
bool SYSTEM_GetCalibrationValue(volatile uint32_t *regAddress)
{
  SYSTEM_CalAddrVal_TypeDef * p, * end;

  uint32_t s_regAddress = (uint32_t)regAddress;
  s_regAddress = s_regAddress & CONVERT_NS_TO_S;

#if defined(MSC_FLASH_CHIPCONFIG_MEM_BASE)
  p   = (SYSTEM_CalAddrVal_TypeDef *)MSC_FLASH_CHIPCONFIG_MEM_BASE;
  end = (SYSTEM_CalAddrVal_TypeDef *)MSC_FLASH_CHIPCONFIG_MEM_END;
#else
  p   = (SYSTEM_CalAddrVal_TypeDef *)(DEVINFO_BASE & 0xFFFFF000U);
  end = (SYSTEM_CalAddrVal_TypeDef *)DEVINFO_BASE;
#endif

  for (; p < end; p++) {
    if (p->address == 0) {
      /* p->address == 0 marks the end of the table */
      return false;
    }
    if (p->address == s_regAddress) {
      *regAddress = p->calValue;
      return true;
    }
  }
  /* Nothing found for regAddress. */
  return false;
}

/***************************************************************************//**
 * @brief
 *   Get family security capability.
 *
 * @note
 *   This function retrieves the family security capability based on the
 *   device number. The device number is one letter and 3 digits:
 *   DEVICENUMBER = (alpha-'A')*1000 + numeric. i.e. 0d = "A000"; 1123d = "B123".
 *   The security capabilities are represented by ::SYSTEM_SecurityCapability_TypeDef.
 *
 * @return
 *   Security capability of MCU.
 ******************************************************************************/
SYSTEM_SecurityCapability_TypeDef SYSTEM_GetSecurityCapability(void)
{
  SYSTEM_SecurityCapability_TypeDef sc;

#if (_SILICON_LABS_32B_SERIES == 0)
  sc = securityCapabilityNA;
#elif (_SILICON_LABS_32B_SERIES == 1)
  sc = securityCapabilityBasic;
#else
  sc = securityCapabilityUnknown;
#endif

#if (_SILICON_LABS_32B_SERIES == 2)
  uint16_t mcuFeatureSetMajor;
  uint16_t deviceNumber;
  deviceNumber = SYSTEM_GetPartNumber();
  mcuFeatureSetMajor = 'A' + (deviceNumber / 1000);
#if defined(_SILICON_LABS_32B_SERIES_2_CONFIG_2)
  // override feature set since BRD4182A Rev A00 -> rev B02 are marked "A"
  mcuFeatureSetMajor = 'C';
#endif

  switch (mcuFeatureSetMajor) {
    case 'A':
      sc = securityCapabilitySE;
      break;

    case 'B':
      sc = securityCapabilityVault;
      break;

    case 'C':
      sc = securityCapabilityRoT;
      break;

    default:
      sc = securityCapabilityUnknown;
      break;
  }
#endif

  return sc;
}

/** @} (end addtogroup system) */
// NOLINTEND
