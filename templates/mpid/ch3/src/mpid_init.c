/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */
/*
 *  (C) 2001 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#include "mpidimpl.h"

#define MAX_JOBID_LEN 1024

#if defined(HAVE_LIMITS_H)
#include <limits.h>
#endif
#if defined(HAVE_UNISTD_H)
#include <unistd.h>
#endif

/* FIXME: This does not belong here */
#ifdef USE_MPIDI_DBG_PRINT_VC
char *MPIDI_DBG_parent_str = "?";
#endif

/* FIXME: the PMI init function should ONLY do the PMI operations, not the 
   process group or bc operations.  These should be in a separate routine */
#ifdef USE_PMI2_API
#include "pmi2.h"
#else
#include "pmi.h"
#endif

#include "datatype.h"

int MPIDI_Use_pmi2_api = 0;

static int pg_compare_ids(void * id1, void * id2);
static int pg_destroy(MPIDI_PG_t * pg );
static int set_eager_threshold(MPIR_Comm *comm_ptr, MPIR_Info *info, void *state);

MPIDI_Process_t MPIDI_Process = { NULL };
MPIDI_CH3U_SRBuf_element_t * MPIDI_CH3U_SRBuf_pool = NULL;
MPIDI_CH3U_Win_fns_t MPIDI_CH3U_Win_fns = { NULL };
MPIDI_CH3U_Win_hooks_t MPIDI_CH3U_Win_hooks = { NULL };
MPIDI_CH3U_Win_pkt_ordering_t MPIDI_CH3U_Win_pkt_orderings = { 0 };

DUMP_STUB global_init

#undef FUNCNAME
#define FUNCNAME finalize_failed_procs_group
#undef FCNAME
#define FCNAME MPL_QUOTE(FUNCNAME)
static int finalize_failed_procs_group(void *param)
{
    int mpi_errno = MPI_SUCCESS;
    if (MPIDI_Failed_procs_group != MPIR_Group_empty) {
        mpi_errno = MPIR_Group_free_impl(MPIDI_Failed_procs_group);
        if (mpi_errno) MPIR_ERR_POP(mpi_errno);
    }
    
 fn_fail:
    return mpi_errno;
}

#undef FUNCNAME
#define FUNCNAME set_eager_threshold
#undef FCNAME
#define FCNAME MPL_QUOTE(FUNCNAME)
static int set_eager_threshold(MPIR_Comm *comm_ptr, MPIR_Info *info, void *state)
{
    int mpi_errno = MPI_SUCCESS;
    char *endptr;
    MPIR_FUNC_TERSE_STATE_DECL(MPID_STATE_MPIDI_CH3_SET_EAGER_THRESHOLD);

    MPIR_FUNC_TERSE_ENTER(MPID_STATE_MPIDI_CH3_SET_EAGER_THRESHOLD);

    comm_ptr->dev.eager_max_msg_sz = strtol(info->value, &endptr, 0);

    MPIR_ERR_CHKANDJUMP1(*endptr, mpi_errno, MPI_ERR_ARG,
                         "**infohintparse", "**infohintparse %s",
                         info->key);

 fn_exit:
    MPIR_FUNC_TERSE_EXIT(MPID_STATE_MPIDI_CH3_SET_EAGER_THRESHOLD);
    return mpi_errno;
 fn_fail:
    goto fn_exit;
}


#undef FUNCNAME
#define FUNCNAME MPID_Init
#undef FCNAME
#define FCNAME MPL_QUOTE(FUNCNAME)
int MPID_Init(int* p_argc, char*** p_argv, int n_requested, int* pn_provided, int* p_has_args, int* p_has_env)
{
    $call fn_, MPID_Init
}

/* This allows each channel to perform final initialization after the
 rest of MPI_Init completes.  */
int MPID_InitCompleted( void )
{
    int mpi_errno;
    mpi_errno = MPIDI_CH3_InitCompleted();
    return mpi_errno;
}

/*
 * Create the storage for the business card. 
 *
 * The routine MPIDI_CH3I_BCFree should be called with the original 
 * value *bc_val_p .  Note that the routines that set the value 
 * of the businesscard return a pointer to the first free location,
 * so you need to remember the original location in order to free 
 * it later.
 *
 */
int MPIDI_CH3I_BCInit( char **bc_val_p, int *val_max_sz_p )
{
    int pmi_errno;
    int mpi_errno = MPI_SUCCESS;
#ifdef USE_PMI2_API
    *val_max_sz_p = PMI2_MAX_VALLEN;
#else
    pmi_errno = PMI_KVS_Get_value_length_max(val_max_sz_p);
    if (pmi_errno != PMI_SUCCESS)
    {
        MPIR_ERR_SETANDJUMP1(mpi_errno,MPI_ERR_OTHER,
                             "**pmi_kvs_get_value_length_max",
                             "**pmi_kvs_get_value_length_max %d", pmi_errno);
    }
#endif
    /* This memroy is returned by this routine */
    *bc_val_p = MPL_malloc(*val_max_sz_p, MPL_MEM_ADDRESS);
    if (*bc_val_p == NULL) {
	MPIR_ERR_SETANDJUMP1(mpi_errno,MPI_ERR_OTHER, "**nomem","**nomem %d",
			     *val_max_sz_p);
    }
    
    /* Add a null to simplify looking at the bc */
    **bc_val_p = 0;

  fn_exit:
    return mpi_errno;

  fn_fail:
    goto fn_exit;
}

/* Free the business card.  This routine should be called once the business
   card is published. */
int MPIDI_CH3I_BCFree( char *bc_val )
{
    /* */
    if (bc_val) {
	MPL_free( bc_val );
    }
    
    return 0;
}

/* FIXME: The PG code should supply these, since it knows how the 
   pg_ids and other data are represented */
static int pg_compare_ids(void * id1, void * id2)
{
    return (strcmp((char *) id1, (char *) id2) == 0) ? TRUE : FALSE;
}


static int pg_destroy(MPIDI_PG_t * pg)
{
    if (pg->id != NULL)
    { 
	MPL_free(pg->id);
    }
    
    return MPI_SUCCESS;
}

