/* Test the `vbslQf32' ARM Neon intrinsic.  */
/* This file was autogenerated by neon-testgen.  */

/* { dg-do assemble } */
/* { dg-require-effective-target arm_neon_ok } */
/* { dg-options "-save-temps -O0" } */
/* { dg-add-options arm_neon } */

#include "arm_neon.h"

void test_vbslQf32 (void)
{
  float32x4_t out_float32x4_t;
  uint32x4_t arg0_uint32x4_t;
  float32x4_t arg1_float32x4_t;
  float32x4_t arg2_float32x4_t;

  out_float32x4_t = vbslq_f32 (arg0_uint32x4_t, arg1_float32x4_t, arg2_float32x4_t);
}

/* { dg-final { scan-assembler "((vbsl)|(vbit)|(vbif))\[ 	\]+\[qQ\]\[0-9\]+, \[qQ\]\[0-9\]+, \[qQ\]\[0-9\]+!?\(\[ 	\]+@.*\)?\n" } } */
/* { dg-final { cleanup-saved-temps } } */
