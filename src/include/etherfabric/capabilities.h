/* SPDX-License-Identifier: BSD-2-Clause */
/* X-SPDX-Copyright-Text: (c) Copyright 2016-2020 Xilinx, Inc. */

/**************************************************************************\
*//*! \file
** \author    Solarflare Communications, Inc.
** \brief     Capabilities API for EtherFabric Virtual Interface HAL.
** \date      2018/11/06
** \copyright Copyright &copy; 2018 Solarflare Communications, Inc. All
**            rights reserved. Solarflare, OpenOnload and EnterpriseOnload
**            are trademarks of Solarflare Communications, Inc.
*//*
\**************************************************************************/

#ifndef __EFAB_CAPABILITIES_H__
#define __EFAB_CAPABILITIES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <etherfabric/base.h>
#include <etherfabric/pd.h>

/*
 * List of capabilities that can be queried.
 *
 * These describe the theoretical availability of a resource in the
 * hardware's current configuration, rather than the current
 * availability of that resource, or if the hardware would be capable
 * in a different configuration.  However, for those capabilities where
 * permissions are relevant, the reported value does reflects whether
 * the caller is permitted to access that resource, either due to
 * licences, NIC configuration, or user privileges
 */

/* NB.  Do NOT remove, reorder or add to the middle of this
   enumeration. */

/*! \brief Possible capabilities */
enum ef_vi_capability {
  /** Hardware capable of PIO */
  EF_VI_CAP_PIO = 0,
  /** PIO buffer size supplied to each VI */
  EF_VI_CAP_PIO_BUFFER_SIZE,
  /** Total number of PIO buffers */
  EF_VI_CAP_PIO_BUFFER_COUNT,

  /** Can packets be looped back by hardware */
  EF_VI_CAP_HW_MULTICAST_LOOPBACK,
  /** Can mcast be delivered to many VIs */
  EF_VI_CAP_HW_MULTICAST_REPLICATION,

  /** Hardware timestamping of received packets */
  EF_VI_CAP_HW_RX_TIMESTAMPING,
  /** Hardware timestamping of transmitted packets */
  EF_VI_CAP_HW_TX_TIMESTAMPING,

  /** Is firmware capable of packed stream mode */
  EF_VI_CAP_PACKED_STREAM,
  /** Packed stream buffer sizes supported in kB, bitmask */
  EF_VI_CAP_PACKED_STREAM_BUFFER_SIZES,
  /** NIC switching, ef_pd_alloc_with_vport */
  EF_VI_CAP_VPORTS,

  /** Is physical addressing mode supported? */
  EF_VI_CAP_PHYS_MODE,
  /** Is buffer addressing mode (NIC IOMMU) supported */
  EF_VI_CAP_BUFFER_MODE,

  /** Chaining of multicast filters */
  EF_VI_CAP_MULTICAST_FILTER_CHAINING,
  /** Can functions create filters for 'wrong' MAC addr */
  EF_VI_CAP_MAC_SPOOFING,

  /** Filter on local IP + UDP port */
  EF_VI_CAP_RX_FILTER_TYPE_UDP_LOCAL,
  /** Filter on local IP + TCP port */
  EF_VI_CAP_RX_FILTER_TYPE_TCP_LOCAL,
  /** Filter on local and remote IP + UDP port */
  EF_VI_CAP_RX_FILTER_TYPE_UDP_FULL,
  /** Filter on local and remote IP + TCP port */
  EF_VI_CAP_RX_FILTER_TYPE_TCP_FULL,
  /** Filter on any of above four types with addition of VLAN */
  EF_VI_CAP_RX_FILTER_TYPE_IP_VLAN,

  /** Filter on local IP + UDP port */
  EF_VI_CAP_RX_FILTER_TYPE_UDP6_LOCAL,
  /** Filter on local IP + TCP port */
  EF_VI_CAP_RX_FILTER_TYPE_TCP6_LOCAL,
  /** Filter on local and remote IP + UDP port */
  EF_VI_CAP_RX_FILTER_TYPE_UDP6_FULL,
  /** Filter on local and remote IP + TCP port */
  EF_VI_CAP_RX_FILTER_TYPE_TCP6_FULL,
  /** Filter on any of above four types with addition of VLAN */
  EF_VI_CAP_RX_FILTER_TYPE_IP6_VLAN,

  /** Filter on local MAC address */
  EF_VI_CAP_RX_FILTER_TYPE_ETH_LOCAL,
  /** Filter on local MAC+VLAN */
  EF_VI_CAP_RX_FILTER_TYPE_ETH_LOCAL_VLAN,

  /** Filter on "all unicast" */
  EF_VI_CAP_RX_FILTER_TYPE_UCAST_ALL,
  /** Filter on "all multicast" */
  EF_VI_CAP_RX_FILTER_TYPE_MCAST_ALL,
  /** Filter on "unicast mismatch" */
  EF_VI_CAP_RX_FILTER_TYPE_UCAST_MISMATCH,
  /** Filter on "multicast mismatch" */
  EF_VI_CAP_RX_FILTER_TYPE_MCAST_MISMATCH,

  /** Availability of RX sniff filters */
  EF_VI_CAP_RX_FILTER_TYPE_SNIFF,
  /** Availability of TX sniff filters */
  EF_VI_CAP_TX_FILTER_TYPE_SNIFF,

  /** Filter on IPv4 protocol */
  EF_VI_CAP_RX_FILTER_IP4_PROTO,
  /** Filter on ethertype */
  EF_VI_CAP_RX_FILTER_ETHERTYPE,

  /** Available RX queue sizes, bitmask */
  EF_VI_CAP_RXQ_SIZES,
  /** Available TX queue sizes, bitmask */
  EF_VI_CAP_TXQ_SIZES,
  /** Available event queue sizes, bitmask */
  EF_VI_CAP_EVQ_SIZES,

  /** Availability of zero length RX packet prefix */
  EF_VI_CAP_ZERO_RX_PREFIX,

  /** Is always enabling TX push supported? */
  EF_VI_CAP_TX_PUSH_ALWAYS,

  /** Availability of NIC pace feature */
  EF_VI_CAP_NIC_PACE,

  /** Availability of RX event merging mode */
  EF_VI_CAP_RX_MERGE,

  /** Availability of TX alternatives */
  EF_VI_CAP_TX_ALTERNATIVES,

  /** Number of TX alternatives vFIFOs */
  EF_VI_CAP_TX_ALTERNATIVES_VFIFOS,

  /** Number of TX alternatives common pool buffers */
  EF_VI_CAP_TX_ALTERNATIVES_CP_BUFFERS,

  /** RX firmware variant */
  EF_VI_CAP_RX_FW_VARIANT,

  /** TX firmware variant */
  EF_VI_CAP_TX_FW_VARIANT,

  /** Availability of CTPIO */
  EF_VI_CAP_CTPIO,

  /** Size of TX alternatives common pool buffers **/
  EF_VI_CAP_TX_ALTERNATIVES_CP_BUFFER_SIZE,

  /** RX queue is configured to force event merging **/
  EF_VI_CAP_RX_FORCE_EVENT_MERGING,

  /** Smallest supported page size/alignment when using buffer mode */
  EF_VI_CAP_MIN_BUFFER_MODE_SIZE,

  /** Hardware supports only CTPIO sending method */
  EF_VI_CAP_CTPIO_ONLY,

  /** Packets captured on a VI might belong to other instance */
  EF_VI_CAP_RX_SHARED,

  /** Ability to specify a destination for a filter */
  EF_VI_CAP_RX_FILTER_SET_DEST,

  /** Filter on MAC + IPv4 protocol */
  EF_VI_CAP_RX_FILTER_MAC_IP4_PROTO,

  /** Ability to independently poll for RX */
  EF_VI_CAP_RX_POLL,

  /** RX uses RX_REF events */
  EF_VI_CAP_RX_REF,

  /** Supported extra datapaths (see enum ef_vi_extra_datapaths) */
  EF_VI_CAP_EXTRA_DATAPATHS,

  /** Maximum value of capabilities enumeration */
  EF_VI_CAP_MAX, /* Keep this last */
};


/*! \brief Flags to modify a capability lookup */
enum ef_vi_capability_flags {
  /** Lookup the capability of the LLCT datapath, valid for ifindex lookups */
  EF_VI_CAP_F_LLCT = 0x80000000,

  /** The bitset of all available flags */
  EF_VI_CAP_F_ALL = EF_VI_CAP_F_LLCT,
};


/*! \brief Values returned by EF_VI_CAP_EXTRA_DATAPATHS */
enum ef_vi_extra_datapaths {
  /** This interface supports the LLCT datapath, which should be selected by
  ** supplying the EF_PD_LLCT PD flag at PD allocation time. */
  EF_VI_EXTRA_DATAPATH_EXPRESS = (1 << 0),
};


/*! \brief Get the value of the given capability
**
** \param handle  The ef_driver_handle associated with the interface that you
**                wish to query.
** \param ifindex The index of the interface that you wish to query. You can
**                use if_nametoindex() to obtain this. This should be the
**                underlying physical interface, rather than a bond, VLAN, or
**                similar.
** \param cap     The capability to get.
** \param value   Pointer to location at which to store the value.
**
** \return 0 on success (capability is supported and value field is updated),
**         or a negative error code:\n
**         -EOPNOTSUPP if the capability is not supported\n
**         -ENOSYS if the API does not know how to retreive support for the
**                 supplied capability\n
**         other negative error if support could not be retrieved
**/
extern int
ef_vi_capabilities_get(ef_driver_handle handle, int ifindex,
                       enum ef_vi_capability cap, unsigned long* value);

/*! \brief Get the value of the given capability on a specific ef_pd
**
** \param handle  The ef_driver_handle associated with the interface that you
**                wish to query.
** \param pd      The ef_pd that you wish to query.
** \param pd_dh   The ef_driver_handle associated with the pd that you
**                wish to query.
** \param cap     The capability to get.
** \param value   Pointer to location at which to store the value.
**
** \return 0 on success (capability is supported and value field is updated),
**         or a negative error code:\n
**         -EOPNOTSUPP if the capability is not supported\n
**         -ENOSYS if the API does not know how to retreive support for the
**                 supplied capability\n
**         other negative error if support could not be retrieved
**/
extern int
ef_pd_capabilities_get(ef_driver_handle handle, ef_pd* pd,
                       ef_driver_handle pd_dh, enum ef_vi_capability cap,
                       unsigned long* value);

/*! \brief Get the value of the given capability for a specific ef_pd without
**         having to allocate the ef_pd.
**
** \param handle   The ef_driver_handle associated with the interface that you
**                 wish to query.
** \param ifindex  The index of the interface that you wish to query. You can
**                 use if_nametoindex() to obtain this. This should be the
**                 underlying physical interface, rather than a bond, VLAN, or
**                 similar.
** \param pd_flags The PD flags that would be used to allocate the ef_pd to
**                 lookup the capability on.
** \param cap      The capability to get.
** \param value    Pointer to location at which to store the value.
**
** \return 0 on success (capability is supported and value field is updated),
**         or a negative error code:\n
**         -EOPNOTSUPP if the capability is not supported\n
**         -ENOSYS if the API does not know how to retreive support for the
**                 supplied capability\n
**         other negative error if support could not be retrieved
**/
extern int
ef_vi_capabilities_get_from_pd_flags(ef_driver_handle handle,
                                     int ifindex,
                                     enum ef_pd_flags pd_flags,
                                     enum ef_vi_capability cap,
                                     unsigned long* value);


/*! \brief Gets the maximum supported value of \ref ef_vi_capability
**
** \return The maximum capability value, or a negative error code.
**
** This function returns the maximum supported value of \ref ef_vi_capability,
** so that all capabilities can be iterated. For example:
**
** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.c}
** max = ef_vi_capabilities_max();
** for( cap = 0; cap <= max; ++cap ) {
**   rc = ef_vi_capabilities_get(driver_handle, ifindex, cap, &val);
**   if( rc == 0 ) printf("%s %d\n", ef_vi_capabilities_name(cap), val);
** }
** ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
**/
extern int
ef_vi_capabilities_max(void);


/*! \brief Gets a human-readable string describing the given capability
**
** \param cap The capability for which to get the description.
**
** \return A string describing the capability, or a negative error code.
**/
extern const char*
ef_vi_capabilities_name(enum ef_vi_capability cap);


#ifdef __cplusplus
}
#endif

#endif /* __EFAB_CAPABILITIES_H__ */

