/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */
/*
 *  (C) 2010 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#include "hydra.h"
#include "pmip.h"
#include "demux.h"
#include "bsci.h"
#include "topo.h"

struct HYD_pmcd_pmip_s HYD_pmcd_pmip;

static HYD_status init_params(void)
{
    HYD_status status = HYD_SUCCESS;

    HYDU_init_user_global(&HYD_pmcd_pmip.user_global);

    HYD_pmcd_pmip.system_global.global_core_map.local_filler = -1;
    HYD_pmcd_pmip.system_global.global_core_map.local_count = -1;
    HYD_pmcd_pmip.system_global.global_core_map.global_count = -1;
    HYD_pmcd_pmip.system_global.pmi_id_map.filler_start = -1;
    HYD_pmcd_pmip.system_global.pmi_id_map.non_filler_start = -1;

    HYD_pmcd_pmip.system_global.global_process_count = -1;
    HYD_pmcd_pmip.system_global.pmi_fd = NULL;
    HYD_pmcd_pmip.system_global.pmi_rank = -1;
    HYD_pmcd_pmip.system_global.pmi_process_mapping = NULL;

    HYD_pmcd_pmip.upstream.server_name = NULL;
    HYD_pmcd_pmip.upstream.server_port = -1;
    HYD_pmcd_pmip.upstream.control = HYD_FD_UNSET;

    HYD_pmcd_pmip.downstream.out = NULL;
    HYD_pmcd_pmip.downstream.err = NULL;
    HYD_pmcd_pmip.downstream.in = HYD_FD_UNSET;
    HYD_pmcd_pmip.downstream.pid = NULL;
    HYD_pmcd_pmip.downstream.exit_status = NULL;
    HYD_pmcd_pmip.downstream.pmi_rank = NULL;
    HYD_pmcd_pmip.downstream.pmi_fd = NULL;
    HYD_pmcd_pmip.downstream.forced_cleanup = 0;

    HYD_pmcd_pmip.local.id = -1;
    HYD_pmcd_pmip.local.pgid = -1;
    HYD_pmcd_pmip.local.iface_ip_env_name = NULL;
    HYD_pmcd_pmip.local.hostname = NULL;
    HYD_pmcd_pmip.local.spawner_kvsname = NULL;
    HYD_pmcd_pmip.local.proxy_core_count = -1;
    HYD_pmcd_pmip.local.proxy_process_count = -1;
    HYD_pmcd_pmip.local.ckpoint_prefix_list = NULL;
    HYD_pmcd_pmip.local.retries = -1;

    HYD_pmcd_pmip.exec_list = NULL;

    status = HYD_pmcd_pmi_allocate_kvs(&HYD_pmcd_pmip.local.kvs, -1);

    return status;
}

static void cleanup_params(void)
{
    int i;

    HYDU_finalize_user_global(&HYD_pmcd_pmip.user_global);

    /* System global */
    if (HYD_pmcd_pmip.system_global.pmi_fd)
        MPL_free(HYD_pmcd_pmip.system_global.pmi_fd);

    if (HYD_pmcd_pmip.system_global.pmi_process_mapping)
        MPL_free(HYD_pmcd_pmip.system_global.pmi_process_mapping);


    /* Upstream */
    if (HYD_pmcd_pmip.upstream.server_name)
        MPL_free(HYD_pmcd_pmip.upstream.server_name);


    /* Downstream */
    if (HYD_pmcd_pmip.downstream.out)
        MPL_free(HYD_pmcd_pmip.downstream.out);

    if (HYD_pmcd_pmip.downstream.err)
        MPL_free(HYD_pmcd_pmip.downstream.err);

    if (HYD_pmcd_pmip.downstream.pid)
        MPL_free(HYD_pmcd_pmip.downstream.pid);

    if (HYD_pmcd_pmip.downstream.exit_status)
        MPL_free(HYD_pmcd_pmip.downstream.exit_status);

    if (HYD_pmcd_pmip.downstream.pmi_rank)
        MPL_free(HYD_pmcd_pmip.downstream.pmi_rank);

    if (HYD_pmcd_pmip.downstream.pmi_fd)
        MPL_free(HYD_pmcd_pmip.downstream.pmi_fd);

    if (HYD_pmcd_pmip.downstream.pmi_fd_active)
        MPL_free(HYD_pmcd_pmip.downstream.pmi_fd_active);


    /* Local */
    if (HYD_pmcd_pmip.local.iface_ip_env_name)
        MPL_free(HYD_pmcd_pmip.local.iface_ip_env_name);

    if (HYD_pmcd_pmip.local.hostname)
        MPL_free(HYD_pmcd_pmip.local.hostname);

    if (HYD_pmcd_pmip.local.spawner_kvsname)
        MPL_free(HYD_pmcd_pmip.local.spawner_kvsname);

    if (HYD_pmcd_pmip.local.ckpoint_prefix_list) {
        for (i = 0; HYD_pmcd_pmip.local.ckpoint_prefix_list[i]; i++)
            MPL_free(HYD_pmcd_pmip.local.ckpoint_prefix_list[i]);
        MPL_free(HYD_pmcd_pmip.local.ckpoint_prefix_list);
    }

    HYD_pmcd_free_pmi_kvs_list(HYD_pmcd_pmip.local.kvs);


    /* Exec list */
    HYDU_free_exec_list(HYD_pmcd_pmip.exec_list);

    HYDT_topo_finalize();
}

int main(int argc, char **argv)
{
}
