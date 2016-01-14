/* Test the `vld1_laneu16' ARM Neon intrinsic.  */
/* This file was autogenerated by neon-testgen.  */

/* { dg-do assemble } */
/* { dg-require-effective-target arm_neon_ok } */
/* { dg-options "-save-temps -O0" } */
/* { dg-add-options arm_neon } */

#include "arm_neon.h"

void test_vld1_laneu16 (void)
{
  uint16x4_t out_uint16x4_t;
  uint16x4_t arg1_uint16x4_t;

  out_uint16x4_t = vld1_lane_u16 (0, arg1_uint16x4_t, 1);
}

/* { dg-final { scan-assembler "vld1\.16\[ 	\]+((\\\{\[dD\]\[0-9\]+\\\[\[0-9\]+\\\]\\\})|(\[dD\]\[0-9\]+\\\[\[0-9\]+\\\])), \\\[\[rR\]\[0-9\]+\\\]!?\(\[ 	\]+@.*\)?\n" } } */
/* { dg-final { cleanup-saved-temps } } */
