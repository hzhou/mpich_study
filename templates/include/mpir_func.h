/* -*- Mode: C; c-basic-offset:4 ; indent-tabs-mode:nil ; -*- */
/*
 *  (C) 2008 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#ifndef MPIR_FUNC_H_INCLUDED
#define MPIR_FUNC_H_INCLUDED

/* In MPICH, each function has an "enter" and "exit" macro.  These can be
 * used to add various features to each function at compile time, or they
 * can be set to empty to provide the fastest possible production version.
 *
 * There are at this time three choices of features (beyond the empty choice)
 * 1. timing
 *    These collect data on when each function began and finished; the
 *    resulting data can be displayed using special programs
 * 2. Debug logging (selected with --enable-g=log)
 *    Invokes MPL_DBG_MSG at the entry and exit for each routine
 * 3. Additional memory validation of the memory arena (--enable-g=memarena)
 */

/* function enter and exit macros */
$call define_enter_exit_macros
#endif /* MPIR_FUNC_H_INCLUDED */
