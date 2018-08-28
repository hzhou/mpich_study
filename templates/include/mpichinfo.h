/* -*- Mode: C; c-basic-offset:4 ; -*- */
/*
 *  (C) 2007 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

/* This file creates strings for the most important configuration options.
   These are then used in the file src/mpi/init/initthread.c to initialize
   global variables that will then be included in both the library and
   executables, providing a way to determine what version and features of
   MPICH were used with a particular library or executable.
*/
#ifndef MPICHINFO_H_INCLUDED
#define MPICHINFO_H_INCLUDED

#define MPICH_CONFIGURE_ARGS_CLEAN ""
#define MPICH_VERSION_DATE "Mon Apr  9 17:58:42 CDT 2018"
#define MPICH_DEVICE "ch3:nemesis"
#define MPICH_COMPILER_CC "gcc -std=c99 -O2"
#define MPICH_COMPILER_CXX "g++ -O2"
#define MPICH_COMPILER_F77 "gfortran -O2"
#define MPICH_COMPILER_FC "gfortran "
#define MPICH_CUSTOM_STRING ""
#define MPICH_ABIVERSION "0:0:0"

#endif
