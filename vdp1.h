#ifndef _vdp1_
#define _vdp1_

/* vdp1 */
#define VDP1 0x5c00000

#define VDP1VRAM VDP1
#define VDP1VRAMSIZE (512*1024)

#define VDP1FB0 (VDP1+VDP1VRAMSIZE)
#define VDP1FB0SIZE (256*1024)

#define VDP1FB1 (VDP1FB1+VDP1FB0SIZE)
#define VDP1FB1SIZE VDP1FB0SIZE

#define VDP1REG (VDP1+0x100000)

#define TVMR (*(u16*)VDP1REG)
#define FBCR (*(u16*)(VDP1REG+2))
#define PTMR (*(u16*)(VDP1REG+4))
#define EWDR (*(u16*)(VDP1REG+6))
#define EWLR (*(u16*)(VDP1REG+8))
#define EWRR (*(u16*)(VDP1REG+10))
#define ENDR (*(u16*)(VDP1REG+12))
#define EDSR (*(u16*)(VDP1REG+14))
#define LOPR (*(u16*)(VDP1REG+16))
#define COPR (*(u16*)(VDP1REG+18))
#define MODR (*(u16*)(VDP1REG+20))

#pragma pack(push, 1)

/* registers */

/* TVMR */
enum vdp1_tvmr__tvm {
	    normal = 0b000,
	        hd = 0b100,
	rotation16 = 0b010,
	 rotation8 = 0b110,
	      hdtv = 0b001
};

union vdp1_tvmr {
	r16 reg;
	struct {
		vdp1_tvmr__tvm tvm : 3;
		u16 vbe : 1;
		u16 : 12;
	};
};

/* FBCR */
enum vdp1_fbcr__plot_lines { // dil, die
	both = 0b00, // no/single interlace
	even = 0b10, //double interlace
	 odd = 0b11, //double interlace
	 double_interlace = 0b11,
	 single_interlace = 0b00,
};

union vdp1_fbcr {
	r16 reg;
	struct {
		u16 fct : 1;
		u16 fcm : 1;
		u16 dil : 1;
		u16 die : 1;
		u16 eos : 1; // enum { even=0,odd=1 }
		u16 : 11;
	};
	struct {
		u16 : 2;
		vdp1_fbcr__plot_lines plot_line : 2;
		u16 : 12;
	};
	struct {
		u16 : 2;
		vdp1_fbcr__plot_lines interlace : 2;
		u16 : 12;
	};
};

/* TVMR + FBCR */

union vdp1_fbcmr {
	u32 raw;
	struct {
		u32 : 3;
		u32 vbe : 1;
		u32 : 12;
		u32 fct : 1;
		u32 fcm : 1;
		u32 : 14;
	} reg;
};

enum vdp1_fbcmr__mode {
	one_cycle = 0,
	manual_erase = ( 1<<17 ),
	manual_change = ( 1<<16 | 1<<17 ),
	manual_erase_change = ( 1<<3 | 1<<16 | 1<<17 )
};

/* PTMR */

enum vdp1_fbcr__ptm {
	idle = 0b00,
      manual = 0b10,
   automatic = 0b01
};

union vdp1_ptmr {
	r16 reg;
	struct {
		vdp1_fbcr__ptm ptm : 2;
		u16 : 14;
	};
};

/* EWDR */
union vdp1_ewdr {
	r16 reg;
	u16 raw;
	u16 color; // 16b
	struct {   // 8b
		u8 even;
		u8 odd;
	};
};

/* EWLR */
union vdp1_ewlr {
	r16 reg;
	struct {
		u16 ewY1 : 9;
		u16 ewX1 : 6;
		u16 : 1;
	};
};

/* EWRR */
union vdp1_ewrr {
	r16 reg;
	struct {
		u8 ewY3;
		u8 ewX3;
	};
};

/* EW* */
union vdp1_ew {
	r16 reg[3];
	struct {
		union {
			u16 color; // 16b
			struct {   // 8b
				u8 even;
				u8 odd;
			} colors;
		};
		
		struct {
			u16 y1 : 9;
			u16 x1 : 6;
			u16 : 1;
		};
		
		struct {
			u8 y3;
			u8 x3;
		};
	};
};

/* ENDR */
union vdp1_endr {
	r16 reg;
	u16 endr;
};

/* EDSR */
union vdp1_edsr {
	r16 reg;
	struct {
		u16 bef : 1;
		u16 cef : 1;
		u16 : 14;
	};
};

/* LOPR */
union vdp1_lopr {
	r16 reg;
	struct {
		u16 : 2;
		u16 lopr : 14;
	};
};

/* COPR */
union vdp1_copr {
	r16 reg;
	struct {
		u16 : 2;
		u16 copr : 14;
	};
};

/* MODR */
union vdp1_modr {
	r16 reg;
	struct {
		u16 tvm  : 3;
		u16 vbe  : 1;
		u16 fcm  : 1;
		u16 dil  : 1;
		u16 die  : 1;
		u16 eos  : 1;
		u16 ptm1 : 1;
		u16 : 3;
		u16 ver  : 4;
	};
};

/* /registers */

/* commands */

typedef union {
	r16 reg[16];
	
	struct {
		union {
			r16 reg;
			struct {
				u16 comm : 4;
				u16 dir  : 2;
				u16 : 2;
				u16 zp   : 4;
				u16 jp   : 3;
				u16 end  : 1;
			};
		} ctrl;
		
		r16 link;
		
		union {
			r16 reg;
			struct {
				union {
					struct {
						u8 calc : 3;
						u8 mode : 3;
						u8 : 2;
					} color;
					struct{
						u8 : 6;
						u8 spd : 1;
						u8 ecd : 1;
					};
				};

				u16 mesh  : 1;
				u16 cmod  : 1;
				u16 clip  : 1;
				u16 pclp  : 1;
				u16 hss   : 1;
				u16 : 2;
				u16 mon   : 1;
			};
		} pmod;
		
		r16 colr;
		r16 srca;
		
		union {
			r16 reg;
			struct {
				u8 x;
				u16 y : 6;
				u16 : 2;
			};
		} size;
		
		union {
			r16 reg[2];
			struct {
				u16 x : 11;
				u16 xcode : 5;
				u16 y : 11;
				u16 ycode : 5;
			};
		} a;
		
		union {
			r16 reg[2];
			struct {
				u16 x : 11;
				u16 xcode : 5;
				u16 y : 11;
				u16 ycode : 5;
			};
		} b;
		
		union {
			r16 reg[2];
			struct {
				u16 x : 11;
				u16 xcode : 5;
				u16 y : 11;
				u16 ycode : 5;
			};
		} c;
		
		union {
			r16 reg[2];
			struct {
				u16 x : 11;
				u16 xcode : 5;
				u16 y : 11;
				u16 ycode : 5;
			};
		} d;
		
		r16 gouraud;
		
		u16 : 16;
	};
} vdp1_command;

/* /commands */

union vdp1 {
	u8 raw[2048*1024]; // 2MB
	struct {
		union { /* vram, 512KB */
			u8 vram[512*1024];
			vdp1_command commands[512*1024/32];
		};
		
		union { /* framebuffer(s), 256 KB */
			u8 fb[256*1024];
			u16 fb16[128*1024];
			struct {
				u8 fb0[128*1024];
				u8 fb1[128*1024];
			};
		};
		
		u8 reserved[256*1024]; // 256 KB
		
		struct { /* vdp1 registers, 512KB */
			union {
				r16 registers[12];
				u16 rraw[12];
				
				struct {
					union { // write only
						struct {
						/* TVMR */ union vdp1_tvmr tvmr;
						/* FBCR */ union vdp1_fbcr fbcr;
						};
						union vdp1_fbcmr fbcmr;
					};
					/* PTMR */ union vdp1_ptmr ptmr;
					union {
						struct {
							/* EWDR */ union vdp1_ewdr ewdr;
							/* EWLR */ union vdp1_ewlr ewlr;
							/* EWRR */ union vdp1_ewlr ewrr;
						};
						vdp1_ew ew;
					};
					/* ENDR */ union vdp1_endr endr;
					/* reserved */ u16 : 16;
					/* EDSR */ union vdp1_edsr edsr;
					/* LOPR */ union vdp1_lopr lopr;
					/* COPR */ union vdp1_copr copr;
					/* MODR */ union vdp1_modr modr;
				};
			};
			
			u8 empty[512*1024 - 12*2];
		};
		
		u8 prohibited[512*1024]; // 512 KB
	};
};

union vdp1 *vdp1 = (union vdp1*)VDP1;

/* macro */


#define vdp1_fbcmr__set(m) { vdp1->fbcmr.raw |= m; }

#pragma pack(pop)

#endif