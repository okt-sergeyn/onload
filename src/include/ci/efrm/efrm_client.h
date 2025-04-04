/* SPDX-License-Identifier: GPL-2.0 */
/* X-SPDX-Copyright-Text: (c) Copyright 2008-2020 Xilinx, Inc. */
#ifndef __EFRM_CLIENT_H__
#define __EFRM_CLIENT_H__

#include <ci/efrm/resource.h>
#include <ci/internal/transport_config_opt.h>

struct efrm_client;
struct net_device;
struct pci_dev;
struct device;
struct efhw_nic;


struct efrm_client_callbacks {
        /* Called after NIC is back up and MCDI is possible. */
	void (*post_reset)(struct efrm_client *, void *user_data);
        /* Called when a reset has been notified. */
	void (*reset_suspend)(struct efrm_client *, void *user_data);
};


struct efrm_client_attr {
	/* Bitmask of supported ring sizes indexed by efhw_q_type. */
	unsigned vi_ring_sizes[3];
	unsigned vi_ring_doorbell_off[3];
};


typedef bool (*nic_match_func)(const struct efhw_nic *nic,
                               const void *opaque_data);


/* Selects an arbitrary interface. */
#define EFRM_IFINDEX_DEFAULT  -1


/* NB. Callbacks may be invoked even before this returns. */
extern int  efrm_client_get(int ifindex, uint64_t nic_flags,
                            uint64_t nic_flags_mask,
                            struct efrm_client_callbacks *,
                            void *user_data, struct efrm_client **client_out);
extern int  efrm_client_get_by_nic(const struct efhw_nic*,
                                   struct efrm_client_callbacks *,
                                   void *user_data,
                                   struct efrm_client **client_out);
extern void efrm_client_set_callbacks(struct efrm_client *client,
                                      struct efrm_client_callbacks *callbacks,
                                      void *user_data);
extern void efrm_client_put(struct efrm_client *);
extern void efrm_client_add_ref(struct efrm_client *);

extern
const struct efrm_client_attr *efrm_client_get_attr(struct efrm_client *);
extern struct efhw_nic *efrm_client_get_nic(struct efrm_client *);

/* This function is for logging/diagnostics only. ifindexes are not
 * netns-safe */
extern int efrm_client_get_ifindex(struct efrm_client *);

extern int efrm_client_accel_allowed(struct efrm_client *client);

extern struct efhw_nic* efhw_nic_find(const struct net_device *,
                                      uint64_t require, uint64_t reject);
extern struct efhw_nic* efhw_nic_find_by_dev(const struct device *dev);
extern struct efhw_nic* efhw_nic_find_by_foo(nic_match_func match,
                                             const void *match_data);

extern void efrm_client_disable_post_reset(struct efrm_client*);


#endif  /* __EFRM_CLIENT_H__ */
