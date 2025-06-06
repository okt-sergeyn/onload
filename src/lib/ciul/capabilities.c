/* SPDX-License-Identifier: BSD-2-Clause */
/* X-SPDX-Copyright-Text: (c) Copyright 2016-2020 Xilinx, Inc. */
#include <errno.h>
#include <stdio.h>

#include <etherfabric/capabilities.h>
#include <ci/efch/op_types.h>

#include "ef_vi_internal.h"
#include "driver_access.h"

  
static const char* names[EF_VI_CAP_MAX + 1] = {
  [EF_VI_CAP_PIO] = "PIO",
  [EF_VI_CAP_PIO_BUFFER_SIZE] = "PIO buffer size",
  [EF_VI_CAP_PIO_BUFFER_COUNT] = "PIO buffer count",
  [EF_VI_CAP_HW_MULTICAST_LOOPBACK] = "Hardware multicast loopback",
  [EF_VI_CAP_HW_MULTICAST_REPLICATION] = "Hardware multicast replication",
  [EF_VI_CAP_HW_RX_TIMESTAMPING] = "Hardware RX timestamping",
  [EF_VI_CAP_HW_TX_TIMESTAMPING] = "Hardware TX timestamping",
  [EF_VI_CAP_PACKED_STREAM] = "Packed stream",
  [EF_VI_CAP_PACKED_STREAM_BUFFER_SIZES] = "Packed stream buffer sizes",
  [EF_VI_CAP_VPORTS] = "VPorts",
  [EF_VI_CAP_PHYS_MODE] = "Physical addressing mode",
  [EF_VI_CAP_BUFFER_MODE] = "Buffer addressing mode",
  [EF_VI_CAP_MULTICAST_FILTER_CHAINING] = "Multicast filter chaining",
  [EF_VI_CAP_MAC_SPOOFING] = "MAC spoofing",
  [EF_VI_CAP_RX_FILTER_TYPE_UDP_LOCAL] = "IPv4 UDP local RX filters",
  [EF_VI_CAP_RX_FILTER_TYPE_TCP_LOCAL] = "IPv4 TCP local RX filters",
  [EF_VI_CAP_RX_FILTER_TYPE_UDP_FULL] = "IPv4 UDP full RX filters",
  [EF_VI_CAP_RX_FILTER_TYPE_TCP_FULL] = "IPv4 TCP full RX filters",
  [EF_VI_CAP_RX_FILTER_TYPE_IP_VLAN] = "IPv4 VLAN RX filters",
  [EF_VI_CAP_RX_FILTER_TYPE_UDP6_LOCAL] = "IPv6 UDP local RX filters",
  [EF_VI_CAP_RX_FILTER_TYPE_TCP6_LOCAL] = "IPv6 TCP local RX filters",
  [EF_VI_CAP_RX_FILTER_TYPE_UDP6_FULL] = "IPv6 UDP full RX filters",
  [EF_VI_CAP_RX_FILTER_TYPE_TCP6_FULL] = "IPv6 TCP full RX filters",
  [EF_VI_CAP_RX_FILTER_TYPE_IP6_VLAN] = "IPv6 VLAN RX filters",
  [EF_VI_CAP_RX_FILTER_TYPE_ETH_LOCAL] = "Ethernet local RX filters",
  [EF_VI_CAP_RX_FILTER_TYPE_ETH_LOCAL_VLAN] ="Ethernet local + VLAN RX filters",
  [EF_VI_CAP_RX_FILTER_TYPE_UCAST_ALL] = "Unicast all RX filters",
  [EF_VI_CAP_RX_FILTER_TYPE_MCAST_ALL] = "Multicast all RX filters",
  [EF_VI_CAP_RX_FILTER_TYPE_UCAST_MISMATCH] = "Unicast mismatch RX filters",
  [EF_VI_CAP_RX_FILTER_TYPE_MCAST_MISMATCH] = "Multicast mismatch RX filters",
  [EF_VI_CAP_RX_FILTER_TYPE_SNIFF] = "RX sniff",
  [EF_VI_CAP_TX_FILTER_TYPE_SNIFF] = "TX sniff",
  [EF_VI_CAP_RX_FILTER_IP4_PROTO] = "IPv4 protocol RX filters",
  [EF_VI_CAP_RX_FILTER_ETHERTYPE] = "Ethertype RX filters",
  [EF_VI_CAP_RXQ_SIZES] = "RXQ sizes",
  [EF_VI_CAP_TXQ_SIZES] = "TXQ sizes",
  [EF_VI_CAP_EVQ_SIZES] = "EVQ sizes",
  [EF_VI_CAP_ZERO_RX_PREFIX] = "Zero RX packet prefix",
  [EF_VI_CAP_TX_PUSH_ALWAYS] = "Always TX push",
  [EF_VI_CAP_NIC_PACE] = "NIC pacing",
  [EF_VI_CAP_RX_MERGE] = "RX event merge mode",
  [EF_VI_CAP_TX_ALTERNATIVES] = "TX alternatives",
  [EF_VI_CAP_TX_ALTERNATIVES_VFIFOS] = "TX alternatives vFIFOs",
  [EF_VI_CAP_TX_ALTERNATIVES_CP_BUFFERS] = "TX alternatives common pool buffers",
  [EF_VI_CAP_TX_ALTERNATIVES_CP_BUFFER_SIZE] =
    "TX alternatives common pool buffer size",
  [EF_VI_CAP_RX_FW_VARIANT] = "RX firmware variant",
  [EF_VI_CAP_TX_FW_VARIANT] = "TX firmware variant",
  [EF_VI_CAP_CTPIO] = "CTPIO",
  [EF_VI_CAP_RX_FORCE_EVENT_MERGING] = "RX queue is configured to force event merging",
  [EF_VI_CAP_MIN_BUFFER_MODE_SIZE] = "Buffer mode smallest page size",
  [EF_VI_CAP_CTPIO_ONLY] = "TX exclusively supports CTPIO transmit method",
  [EF_VI_CAP_RX_SHARED] = "VI may report packets destined to other VIs sharing RX queue",
  [EF_VI_CAP_RX_FILTER_SET_DEST] = "Set destination for filter",
  [EF_VI_CAP_RX_FILTER_MAC_IP4_PROTO] = "MAC + IPv4 protocol RX filters",
  [EF_VI_CAP_RX_POLL] = "Ability to independently poll for RX",
  [EF_VI_CAP_RX_REF] = "VI uses RX_REF events",
  [EF_VI_CAP_EXTRA_DATAPATHS] = "Supported extra datapaths (see enum ef_vi_extra_datapaths)",
  [EF_VI_CAP_MAX] = "Unknown",
};


int
__ef_vi_capabilities_get_hw(ef_driver_handle handle, int ifindex, int pd_id,
                            ef_driver_handle pd_dh, enum ef_vi_capability cap,
                            unsigned long* value)
{
  int rc;
  ci_capabilities_op_t op = (ci_capabilities_op_t) { 0 };

  /* The capability flags steal the top bits of the capability number, so there
   * must not be any overlap. */
  EF_VI_BUILD_ASSERT((EF_VI_CAP_MAX & EF_VI_CAP_F_ALL) == 0);

  /* Can't specify a PD and an ifindex. */
  EF_VI_ASSERT((ifindex >= 0 && pd_id < 0) || (pd_id >= 0 && ifindex < 0));

  if( (cap & ~EF_VI_CAP_F_ALL) < EF_VI_CAP_MAX ) {
    op.cap_in.ifindex = ifindex;
    if( ifindex < 0 ) {
      op.cap_in.pd_id = efch_make_resource_id(pd_id);
      op.cap_in.pd_fd = pd_dh;
    }
    op.cap_in.cap = cap;
    rc = ci_capabilities_op(handle, &op);

    if( rc == 0 ) {
      *value = op.cap_out.val;
      return op.cap_out.support_rc;
    }
    else {
      return rc;
    }
  }
  else {
    return -ENOSYS;
  }
}


int
__ef_vi_capabilities_get(ef_driver_handle handle, int ifindex, int pd_id,
                         ef_driver_handle pd_dh, enum ef_vi_capability cap,
                         unsigned long* value)
{
  int rc;

  rc = ef_vi_compat_capability_get(cap, value);
  if( rc != 0 && rc != -EOPNOTSUPP )
    rc = __ef_vi_capabilities_get_hw(handle, ifindex, pd_id, pd_dh, cap, value);

  return rc;
}


int ef_vi_capabilities_get_from_pd_flags(ef_driver_handle handle,
                                         int ifindex,
                                         enum ef_pd_flags pd_flags,
                                         enum ef_vi_capability cap,
                                         unsigned long* value)
{
  pd_flags = ef_pd_flags_from_env(pd_flags, handle, ifindex);

  if( pd_flags & EF_PD_EXPRESS )
    cap |= EF_VI_CAP_F_LLCT;

  return __ef_vi_capabilities_get(handle, ifindex, -1, -1, cap, value);
}


int ef_vi_capabilities_get(ef_driver_handle handle, int ifindex,
                           enum ef_vi_capability cap, unsigned long* value)
{
  return ef_vi_capabilities_get_from_pd_flags(handle, ifindex, EF_PD_DEFAULT,
                                              cap, value);
}


int ef_pd_capabilities_get(ef_driver_handle handle, ef_pd* pd,
                           ef_driver_handle pd_dh, enum ef_vi_capability cap,
                           unsigned long* value)
{
  return __ef_vi_capabilities_get(handle, -1, pd->pd_resource_id, pd_dh, cap,
                                  value);
}


int ef_vi_capabilities_max(void)
{
  return EF_VI_CAP_MAX - 1;
}


const char* ef_vi_capabilities_name(enum ef_vi_capability cap)
{
  if( cap < EF_VI_CAP_MAX )
    return names[cap];
  else
    return names[EF_VI_CAP_MAX];
}

