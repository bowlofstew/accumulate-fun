#pragma once

#include "accumulate.h"

#include "binsearch_ops.h"
#include "heap_ops.h"
#include "minmax_ops.h"
#include "modifying_seq_ops.h"
#include "non_modifying_seq_ops.h"
#include "numeric_ops.h"
#include "partitioning_ops.h"
#include "set_ops.h"
#include "sorting_ops.h"

//  1/ 1 accumulate itself
//
//  0/ 4 binary search ops
//  0/ 6 heap ops
//  6/10 min/max ops
// 24/27 modifying sequence ops
// 16/16 non-modifying sequence ops
//  4/ 4 numeric ops
//  0/ 5 partitioning ops
//  0/ 7 set ops
//  0/ 7 sorting ops
//
// 51/87 total

// not included in the 87:
// iter_swap, swap, random_shuffle
