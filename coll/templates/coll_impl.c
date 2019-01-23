/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */
/*
 *  (C) 2006 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 *
 *  Portions of this code were written by Intel Corporation.
 *  Copyright (C) 2011-2017 Intel Corporation.  Intel provides this material
 *  to Argonne National Laboratory subject to Software Grant and Corporate
 *  Contributor License Agreement dated February 8, 2012.
 */

#include "mpiimpl.h"
#include "coll_impl.h"
DUMP_STUB global_init

/*
=== BEGIN_MPI_T_CVAR_INFO_BLOCK ===

cvars:
    - name        : MPIR_CVAR_DEVICE_COLLECTIVES
      category    : COLLECTIVE
      type        : boolean
      default     : true
      class       : device
      verbosity   : MPI_T_VERBOSITY_USER_BASIC
      scope       : MPI_T_SCOPE_ALL_EQ
      description : >-
        If set to true, MPI collectives will allow the device to override
        the MPIR-level collective algorithms. The device still has the
        option to call the MPIR-level algorithms manually.  If set to false,
        the device-level collective function will not be called.

=== END_MPI_T_CVAR_INFO_BLOCK ===
*/

int MPIR_Nbc_progress_hook_id = 0;
/*
 * Initializing the below choice variables is not strictly necessary.
 * However, such uninitialized variables are common variables, which
 * some linkers treat as undefined references.  Initializing them to
 * AUTO keeps such linkers happy.
 *
 * More discussion on this can be found in the "--warn-common"
 * discussion of
 * https://sourceware.org/binutils/docs/ld/Options.html#Options
 */
$call global_coll_algo_vars

#undef FUNCNAME
#define FUNCNAME MPII_Coll_init
#undef FCNAME
#define FCNAME MPL_QUOTE(FUNCNAME)
int MPII_Coll_init(void)
{
    int mpi_errno = MPI_SUCCESS;

    /* =========================================================================== */
    /* | Check if the user manually selected any collective algorithms via CVARs | */
    /* =========================================================================== */

    $call check_cvar_algo_choices

    /* register non blocking collectives progress hook */
    mpi_errno = MPID_Progress_register_hook(MPIDU_Sched_progress, &MPIR_Nbc_progress_hook_id);
    if (mpi_errno)
        MPIR_ERR_POP(mpi_errno);

    /* initialize transports */
    mpi_errno = MPII_Stubtran_init();
    if (mpi_errno)
        MPIR_ERR_POP(mpi_errno);
    mpi_errno = MPII_Gentran_init();
    if (mpi_errno)
        MPIR_ERR_POP(mpi_errno);

    /* initialize algorithms */
    mpi_errno = MPII_Stubalgo_init();
    if (mpi_errno)
        MPIR_ERR_POP(mpi_errno);
    mpi_errno = MPII_Treealgo_init();
    if (mpi_errno)
        MPIR_ERR_POP(mpi_errno);
    mpi_errno = MPII_Recexchalgo_init();
    if (mpi_errno)
        MPIR_ERR_POP(mpi_errno);

  fn_exit:
    return mpi_errno;
  fn_fail:
    goto fn_exit;
}

#undef FUNCNAME
#define FUNCNAME MPII_Coll_finalize
#undef FCNAME
#define FCNAME MPL_QUOTE(FUNCNAME)
int MPII_Coll_finalize(void)
{
    int mpi_errno = MPI_SUCCESS;

    /* deregister non blocking collectives progress hook */
    MPID_Progress_deregister_hook(MPIR_Nbc_progress_hook_id);

    mpi_errno = MPII_Gentran_finalize();
    if (mpi_errno)
        MPIR_ERR_POP(mpi_errno);
    mpi_errno = MPII_Stubtran_finalize();
    if (mpi_errno)
        MPIR_ERR_POP(mpi_errno);

  fn_exit:
    return mpi_errno;
  fn_fail:
    goto fn_exit;
}

/* Function used by CH3 progress engine to decide whether to
 * block for a recv operation */
int MPIR_Coll_safe_to_block()
{
    return MPII_Gentran_scheds_are_pending() == false;
}

/* Function to initialze communicators for collectives */
int MPIR_Coll_comm_init(MPIR_Comm * comm)
{
    int mpi_errno = MPI_SUCCESS;

    /* initialize any stub algo related data structures */
    mpi_errno = MPII_Stubalgo_comm_init(comm);
    if (mpi_errno)
        MPIR_ERR_POP(mpi_errno);
    /* initialize any tree algo related data structures */
    mpi_errno = MPII_Treealgo_comm_init(comm);
    if (mpi_errno)
        MPIR_ERR_POP(mpi_errno);

    /* initialize any transport data structures */
    mpi_errno = MPII_Stubtran_comm_init(comm);
    if (mpi_errno)
        MPIR_ERR_POP(mpi_errno);
    mpi_errno = MPII_Gentran_comm_init(comm);
    if (mpi_errno)
        MPIR_ERR_POP(mpi_errno);

  fn_exit:
    return mpi_errno;
  fn_fail:
    goto fn_exit;
}

/* Function to cleanup any communicators for collectives */
int MPII_Coll_comm_cleanup(MPIR_Comm * comm)
{
    int mpi_errno = MPI_SUCCESS;

    /* cleanup all collective communicators */
    mpi_errno = MPII_Stubalgo_comm_cleanup(comm);
    if (mpi_errno)
        MPIR_ERR_POP(mpi_errno);
    mpi_errno = MPII_Treealgo_comm_cleanup(comm);
    if (mpi_errno)
        MPIR_ERR_POP(mpi_errno);

    /* cleanup transport data */
    mpi_errno = MPII_Stubtran_comm_cleanup(comm);
    if (mpi_errno)
        MPIR_ERR_POP(mpi_errno);
    mpi_errno = MPII_Gentran_comm_cleanup(comm);
    if (mpi_errno)
        MPIR_ERR_POP(mpi_errno);

  fn_exit:
    return mpi_errno;
  fn_fail:
    goto fn_exit;
}
