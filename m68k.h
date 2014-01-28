#ifndef _sat_m68k_
#define _sat_m68k_

using namespace std;

#include "bitfield.h"

#pragma pack(push, 1)

namespace M68K {
  	union m68k {
		volatile u8 raw[ (512 + 4)*1024 ];
		
		volatile struct {
			union { /* dram, 512KB */
				u8 dramraw[512*1024];
				struct {
					union {
						u32 address;
						struct {
							u16 upper;
							u16 lower;
						} addressWord;
					} stack;
					
					union {
						u32 address;
						struct {
							u16 upper;
							u16 lower;
						} addressWord;
					} pc;
					
					u8 dram[512*1024 - 2*4];
				};
			};
			
			union { /* registers, 4KB */
				u8 raw[4096];
				struct {
					/* 1024 */
					r16 individualSlotControl[508];
					u8 junk0[1024 - 508*2];
					/* 512 */
					union {
						r16 reg[24];
						struct {// 400
							u16 : 6;
							u16 M4 : 1;
							u16 DB : 1;
							u16 VER: 4;
							u16 MVOL : 4;
							// 402
							u16 : 7;
							u16 RBL : 2;
							u16 RBP : 7;
							// 404
							u16 : 3;
							u16 OF : 1;
							u16 OE : 1;
							u16 IO : 1;
							u16 IF : 1;
							u16 IE : 1;
							u16 MIBUF : 8;
							// 406
							u16 : 8;
							u16 MOBUF : 8;
							// 408
							u16 MSLC : 5;
							u16 CA : 4;
							u16 : 7;
							// 40A
							u16 junk[8];
							// 412
							u16 DMEAL : 15;
							u16 : 1;
							// 414
							u16 DMEAH : 4;
							u16 DRGA : 11;
							u16 : 1;
							// 416
							u16 : 1;
							u16 GA : 1;
							u16 DI : 1;
							u16 EX : 1;
							u16 DTLG : 11;
							u16 : 1;
							// 418
							u16 : 5;
							u16 TACTL : 3;
							u16 TIMA : 8;
							// 41A
							u16 : 5;
							u16 TBCTL : 3;
							u16 TIMB : 8;
							// 41C
							u16 : 5;
							u16 TCCTL : 3;
							u16 TIMC : 8;
							// 41E
							u16 : 5;
							u16 SCIEB : 11;
							// 420
							u16 : 5;
							u16 SCIPD : 11;
							// 422
							u16 : 5;
							u16 SCIRE : 11;
							// 424
							u16 : 8;
							u16 SCILV0 : 8;
							// 426
							u16 : 8;
							u16 SCILV1 : 8;
							// 428
							u16 : 8;
							u16 SCILV2 : 8;
							// 42A
							u16 : 5;
							u16 MCIEB : 11;
							// 42C
							u16 : 5;
							u16 MCIPD : 11;
							// 42E
							u16 : 5;
							u16 MCIRE : 11;
						};
					} scsp;
					u8 junk1[512 - 24*2];
					/* 256 */
					u16 soundDataTrack[64];
					u8 junk2[256 - 64*2];
					/* 2048 */
					struct {
						r16 control[1010];
					} dsp;
					u8 junk3[2048 - 1010*2];
					/* 256 for 4k */
					u8 unknow[256];
				};
			} registers;
		};
		
	}; /* /union m68k */
	
} /* /namespace M68K */

#pragma pack(pop)

#ifdef __m68k__
	#define M68K_ADDRESS 0x0
#elif defined( __sh__ )
	#define M68K_ADDRESS 0x05A00000
#else
	#error "bad arch"
#endif

#define M68K_DRAM_SIZE (512*1024)

union M68K::m68k *m68k = (union M68K::m68k*)M68K_ADDRESS;

//void m68kHello(void) { *(u32*)&m68k->dram[ sizeof(m68k->dram) - 4 ] = 0xaabbccdd; }

void m68kUpload(u8*bin,u32 size){
	
	// enable the full 512KB dram
	m68k->registers.scsp.M4 = 1;
	// 16b dsp
	m68k->registers.scsp.DB = 0;
	// wait init
	//sleep(50);
	// upload bin
	memcpy((void*)m68k->dram,bin,size);
	// exec
	m68k->pc.address = (u32)m68k->dram;
}

#endif
