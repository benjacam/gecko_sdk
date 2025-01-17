/******************************************************************************
*
* @file   sl_btctrl_hci_packet.h
* @brief  Data structures and constants for HCI data packets
*
*******************************************************************************
* # License
* <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
*******************************************************************************
*
* The licensor of this software is Silicon Laboratories Inc. Your use of this
* software is governed by the terms of Silicon Labs Master Software License
* Agreement (MSLA) available at
* www.silabs.com/about-us/legal/master-software-license-agreement. This
* software is distributed to you in Source Code format and is governed by the
* sections of the MSLA applicable to Source Code.
*
******************************************************************************/
#ifndef SL_BTCTRL_HCI_PACKET_H
#define SL_BTCTRL_HCI_PACKET_H

#include "sl_common.h"

#define hci_command_header_size     3   // opcode (2 bytes), length (1 byte)
#define hci_acl_data_header_size    4   // handle (2 bytes), length (2 bytes)

SL_PACK_START(1)
typedef struct {
  uint16_t opcode; /* HCI command opcode */
  uint8_t param_len; /* command parameter length */
} SL_ATTRIBUTE_PACKED hci_command_t;
SL_PACK_END()

SL_PACK_START(1)
typedef struct {
  uint16_t conn_handle; /* ACL connection handle */
  uint16_t length; /* Length of packet */
} SL_ATTRIBUTE_PACKED acl_packet_t;
SL_PACK_END()

enum hci_packet_type {
  hci_packet_type_ignore = 0, /* 0 used to trigger wakeup interrupt */
  hci_packet_type_command = 1,
  hci_packet_type_acl_data = 2,
  hci_packet_type_sync_data = 3,
  hci_packet_type_event = 4,
  hci_packet_type_iso_data = 5
};

enum hci_packet_state {
  hci_packet_state_read_packet_type = 0,
  hci_packet_state_read_header = 1,
  hci_packet_state_read_data = 2
};

SL_PACK_START(1)
typedef struct {
  enum hci_packet_type packet_type : 8;
  union {
    hci_command_t hci_cmd;
    acl_packet_t acl_pkt;
  };
} SL_ATTRIBUTE_PACKED hci_packet_t;
SL_PACK_END()

void sl_btctrl_hci_packet_step(void);

#endif // SL_BTCTRL_HCI_PACKET_H
