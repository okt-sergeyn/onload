/* SPDX-License-Identifier: GPL-2.0 */
/* X-SPDX-Copyright-Text: (c) Copyright 2018-2020 Xilinx, Inc. */
#ifndef __CI_DRIVER_DRIVERLINK_API__
#define __CI_DRIVER_DRIVERLINK_API__

#if !defined(struct_group)
#define __struct_group(TAG, NAME, ATTRS, MEMBERS...) \
        union { \
                struct { MEMBERS } ATTRS; \
                struct TAG { MEMBERS } ATTRS NAME; \
        }
#define struct_group(NAME, MEMBERS...)  \
        __struct_group(/* no tag */, NAME, /* no attrs */, MEMBERS)
#endif

#include <../driver/linux_net/drivers/net/ethernet/sfc/filter.h>

#endif  /* __CI_DRIVER_DRIVERLINK_API__ */
