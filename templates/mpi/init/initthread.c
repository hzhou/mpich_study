/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */
/*
 *  (C) 2001 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 *
 * Portions of this code were written by Microsoft. Those portions are
 * Copyright (c) 2007 Microsoft Corporation. Microsoft grants
 * permission to use, reproduce, prepare derivative works, and to
 * redistribute to others. The code is licensed "as is." The User
 * bears the risk of using it. Microsoft gives no express warranties,
 * guarantees or conditions. To the extent permitted by law, Microsoft
 * excludes the implied warranties of merchantability, fitness for a
 * particular purpose and non-infringement.
 */

#include "mpiimpl.h"
#include "mpir_info.h"
#include "datatype.h"
#include "mpi_init.h"
#ifdef HAVE_CRTDBG_H
#include <crtdbg.h>
#endif
#ifdef HAVE_USLEEP
#include <unistd.h>
#endif

INCLUDE_FILE docs/cvars_debug_hold.txt

/* Any internal routines can go here.  Make them static if possible */

/* Global variables can be initialized here */
MPIR_Process_t MPIR_Process = { OPA_INT_T_INITIALIZER(MPICH_MPI_STATE__PRE_INIT) };

     /* all other fields in MPIR_Process are irrelevant */
MPIR_Thread_info_t MPIR_ThreadInfo = { 0 };

#if defined(MPICH_IS_THREADED) && defined(MPL_TLS_SPECIFIER)
MPL_TLS_SPECIFIER MPIR_Per_thread_t MPIR_Per_thread = { 0 };
#else
MPIR_Per_thread_t MPIR_Per_thread = { 0 };
#endif

MPID_Thread_tls_t MPIR_Per_thread_key;

/* These are initialized as null (avoids making these into common symbols).
   If the Fortran binding is supported, these can be initialized to
   their Fortran values (MPI only requires that they be valid between
   MPI_Init and MPI_Finalize) */
MPIU_DLL_SPEC MPI_Fint *MPI_F_STATUS_IGNORE ATTRIBUTE((used)) = 0;
MPIU_DLL_SPEC MPI_Fint *MPI_F_STATUSES_IGNORE ATTRIBUTE((used)) = 0;

/* This will help force the load of initinfo.o, which contains data about
   how MPICH was configured. */
extern const char MPII_Version_device[];

$call @init_fortran_mpirinitf_
$call @init_windows
#if defined(MPICH_IS_THREADED)

DUMP_STUB global_init

/* These routine handle any thread initialization that my be required */
#undef FUNCNAME
#define FUNCNAME thread_cs_init
#undef FCNAME
#define FCNAME MPL_QUOTE(FUNCNAME)
static int thread_cs_init(void)
{
    $call fn_, thread_cs_init
    int err;

}

#undef FUNCNAME
#define FUNCNAME MPIR_Thread_CS_Finalize
#undef FCNAME
#define FCNAME MPL_QUOTE(FUNCNAME)
int MPIR_Thread_CS_Finalize(void)
{
    $call fn_, MPIR_Thread_CS_Finalize
}
#endif /* MPICH_IS_THREADED */

