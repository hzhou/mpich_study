/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */
/*
 *  (C) 2008 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#include "hydra_server.h"
#include "hydra.h"
#include "mpiexec.h"
#include "pmci.h"
#include "bsci.h"
#include "demux.h"
#include "ui.h"
#include "uiu.h"

struct HYD_server_info_s HYD_server_info = { {0} };

struct HYD_exec *HYD_uii_mpx_exec_list = NULL;
struct HYD_ui_info_s HYD_ui_info;
struct HYD_ui_mpich_info_s HYD_ui_mpich_info;

DUMP_STUB global_init

#define ordered(n1, n2) \
    (((HYD_ui_mpich_info.sort_order == ASCENDING) && ((n1)->core_count <= (n2)->core_count)) || \
     ((HYD_ui_mpich_info.sort_order == DESCENDING) && ((n1)->core_count >= (n2)->core_count)))

static int compar(const void *_n1, const void *_n2)
{
    struct HYD_node *n1, *n2;
    int ret;

    n1 = *((struct HYD_node **) _n1);
    n2 = *((struct HYD_node **) _n2);

    if (n1->core_count == n2->core_count)
        ret = 0;
    else if (n1->core_count < n2->core_count)
        ret = -1;
    else
        ret = 1;

    if (HYD_ui_mpich_info.sort_order == DESCENDING)
        ret *= -1;

    return ret;
}

static HYD_status qsort_node_list(void)
{
    struct HYD_node **node_list, *node, *new_list, *r1;
    int count, i;
    HYD_status status = HYD_SUCCESS;

    for (count = 0, node = HYD_server_info.node_list; node; node = node->next, count++)
        /* skip */ ;

    HYDU_MALLOC_OR_JUMP(node_list, struct HYD_node **, count * sizeof(struct HYD_node *), status);
    for (i = 0, node = HYD_server_info.node_list; node; node = node->next, i++)
        node_list[i] = node;

    qsort((void *) node_list, (size_t) count, sizeof(struct HYD_node *), compar);

    r1 = new_list = node_list[0];
    for (i = 1; i < count; i++) {
        r1->next = node_list[i];
        r1 = r1->next;
        r1->next = NULL;
    }
    HYD_server_info.node_list = new_list;

    MPL_free(node_list);

  fn_exit:
    return status;

  fn_fail:
    goto fn_exit;
}

