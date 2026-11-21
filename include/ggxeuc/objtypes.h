/* Object-work types for the TUs that declare their own externs and prototypes
 * (act, colli, cpu, key, tension, throw). Every type comes from include/generated/
 * (the Korean X Plus disc's STABS, with the GGX US layouts where they differ);
 * this file only selects them, so those TUs see the same layouts as the rest of
 * the tree. Other TUs include generated/<tu>.h instead. */
#ifndef GGX_OBJTYPES_H
#define GGX_OBJTYPES_H

#define NULL 0

#include "generated/types/u_char.h"
#include "generated/types/u_short.h"
#include "generated/types/u_int.h"
#include "generated/types/_BYTE_.h"
#include "generated/types/_WORD_.h"

#include "generated/types/COLLISION.h"
#include "generated/types/COLLIPOS.h"
#include "generated/types/TACTNORMAL.h"
#include "generated/types/TACTHEADER.h"
#include "generated/types/ACT_FUNC_SIZE.h"
#include "generated/types/TINITINSTANCE.h"
#include "generated/types/PERSONALWORK.h"
#include "generated/types/PLAYER_ENTRY.h"
#include "generated/types/HITSEPARAM.h"
#include "generated/types/HITSEUNION.h"
#include "generated/types/DAMAGEPARAM.h"
#include "generated/types/ACTWORK.h"
#include "generated/types/CHARACTER_WORK.h"

#endif /* GGX_OBJTYPES_H */
