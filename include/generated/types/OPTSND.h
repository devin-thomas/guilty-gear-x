/* GGX US layout inferred from sound-menu code; X Plus STABS supplies the base type. */
#ifndef GGX_GENERATED_TYPE_OPTSND_H
#define GGX_GENERATED_TYPE_OPTSND_H

#include "../enums.h"
#include "../forward/OPTSND.h"

/* GGX US size: 48 bytes (X Plus debug size: 72). */
struct OPTSND {
    int wakupos; /* +0x0 */
    int bgmpos; /* +0x4 */
    int pos[3]; /* +0x8: voice pack, SE pack, shared sound number */
    char *profile; /* +0x14 */
    int loaddat[2]; /* +0x18 */
    int maxno; /* +0x20 */
    int playno; /* +0x24 */
    int id; /* +0x28 */
    int repeat; /* +0x2c */
};

#endif
