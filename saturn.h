#ifndef _Saturn_
#define _Saturn_

#include <string.h>
#include <stdint.h>

#define u8 unsigned char
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

#include "bitfield.h"

#include "vdp1.h"
#include "vdp2.h"
#include "smpc.h"
#include "m68k.h"

/* smpc */

#define PARALLELIO (*(u8*)0x2010007)
#define PARALLELIODDR1 (*(u8*)0x20100079)
#define PARALLELIODDR2 (*(u8*)0x2010007B)
#define PARALLELIOPDR1 (*(u8*)0x20100075)
#define PARALLELIOPDR2 (*(u8*)0x20100077)

#define VBLANK ((SCRSTATE>>3)&1)
#define VBLANKCPT (((*(u16*)(VDP2REG+0x0A))&0x1FF)>>1)

#define BGCLR 0x00

#define SETB(a,b) a|=(1<<b)
#define USETB(a,b) a&=~(1<<b)

#define C_RGB(r,g,b) (0x8000 | ((b >> 3) << 10) | ((g >> 3) << 5) | (r >> 3) )

#define scrULock USETB(TVMR2,15)
#define scrLock   SETB(TVMR2,15)

#endif
