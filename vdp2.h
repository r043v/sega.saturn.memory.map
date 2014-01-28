#ifndef _vdp2_
#define _vdp2_

/* vdp2 */

#define VDP2 0x5E00000
#define VDP2VRAM  VDP2
#define VDP2VRAM0 VDP2VRAM        /* bank 0 128k */
#define VDP2VRAM1 (VDP2+128*1024) /* bank 1 128k */
#define VDP2VRAM2 (VDP2+256*1024) /* bank 2 128k */
#define VDP2VRAM3 (VDP2+384*1024) /* bank 3 128k */

#define VDP2VRAMA VDP2VRAM0 /* bank A 256k */
#define VDP2VRAMB VDP2VRAM2 /* bank B 256k */

#define VDP2CRAM (VDP2+0x100000) /* palette */
#define VDP2REG  (VDP2+0x180000) /* register */

#define TVMR2      (*(u16*)(VDP2REG))
#define VDP2EXT    (*(u16*)(VDP2REG+0x02))
#define SCRSTATE   (*(u16*)(VDP2REG+0x04))
#define VRAMSIZE   (*(u16*)(VDP2REG+0x06))
#define VDP2RAMCR  (*(u16*)(VDP2REG+0x0E))
#define BGREG      (*(u16*)(VDP2REG+0x20))
#define MOZAIC     (*(u16*)(VDP2REG+0x22)) /* Mosaic Control Register */
#define VDP2CCRA   (*(u16*)(VDP2REG+0x28)) /* Character Control Register */
#define VDP2CCRB   (*(u16*)(VDP2REG+0x2A))
#define VDP2BMPAL  (*(u16*)(VDP2REG+0x2C)) /* Bit Map Palette Number (NBG0, NBG1) */

#define BG0PATTERN (*(u16*)(VDP2REG+0x30))
#define BG1PATTERN (*(u16*)(VDP2REG+0x32))

#define MAPOFFR    (*(u16*)(VDP2REG+0x3C))

#define BG0SCROLLHI (*(u16*)(VDP2REG+0x70)) /* NBG0 horizontal direction (integer part) */
#define BG0SCROLLHF (*(u16*)(VDP2REG+0x72)) /* NBG0 horizontal direction (fractional part) */
#define BG0SCROLLVI (*(u16*)(VDP2REG+0x74)) /* NBG0 vertical direction (integer part) */
#define BG0SCROLLVF (*(u16*)(VDP2REG+0x76)) /* NBG0 vertical direction (fractional part) */

#define BG1SCROLLHI (*(u16*)(VDP2REG+0x80)) /* NBG1 horizontal direction (integer part) */
#define BG1SCROLLHF (*(u16*)(VDP2REG+0x82)) /* NBG1 horizontal direction (fractional part) */
#define BG1SCROLLVI (*(u16*)(VDP2REG+0x84)) /* NBG1 vertical direction (integer part) */
#define BG1SCROLLVF (*(u16*)(VDP2REG+0x86)) /* NBG1 vertical direction (fractional part) */

#define BGDZOOM    (*(u16*)(VDP2REG+0x98)) /* reduction enable register */

#define BGVPOSINCR (*(u16*)(VDP2REG+0x9A))
#define SPPRIORITY (*(u16*)(VDP2REG+0xEA))
#define BGPRIORITY (*(u16*)(VDP2REG+0xF8))

#pragma pack(push, 2)

struct vdp2_rotation_data_bank {
	u16 vrama0 : 2;
	u16 vrama1 : 2;
	u16 vramb0 : 2;
	u16 vramb1 : 2;
};

/* TVMD */
union vdp2_tvmd {
	r16 reg;
	struct {
		u16  hreso : 3;
		u16 : 1;
		u16  vreso : 2;
		u16   lsmd : 2;
		u16 bdclmd : 1;
		u16 : 6;
		u16   disp : 1;
	};
};

/* EXTEN */
union vdp2_exten {
	r16 reg;
	struct {
		u16 exbgen : 1;
		u16  dasel : 1;
		u16 : 6;
	};
};

/* TVSTAT */
union vdp2_tvstat {
	r16 reg;
	struct {
		u16    pal : 1;
		u16    odd : 1;
		u16 hblank : 1;
		u16 vblank : 1;
		u16 : 4;
		u16 exsyfg : 1;
		u16 exltfg : 1;
		u16 : 6;
	};
};

/* VRSIZE */
union vdp2_vrsize {
	r16 reg;
	struct {
		u16 ver : 4;
		u16 : 11;
		u16 vramsz : 1;  
	};
};

/* HCNT */
union vdp2_hcnt {
	r16 reg;
	struct {
		u16 hct : 10;
		u16 : 6;
	};
};

/* VCNT */
union vdp2_vcnt {
	r16 reg;
	struct {
		u16 vct : 10;
		u16 : 6;
	};
};

/* RAMCTL */
union vdp2_ramctl {
	r16 reg;
	struct {
		union {
			u8 reg;
			struct {
				u8 a0 : 2;
				u8 a1 : 2;
				u8 b0 : 2;
				u8 b1 : 2;
			};
		} rotation;
		
		u8 vramd : 1;
		u8 vrbmd : 1;
		u8 : 2;
		u8 crmd : 2;
		u8 : 1;
		u8 crkte : 1;
	};
};

union vdp2_vcp {
	r16 reg[2];
	struct {
		/* CYC??L */
		u16 vcp3 : 4;
		u16 vcp2 : 4;
		u16 vcp1 : 4;
		u16 vcp0 : 4;
		
		/* CYC??U */
		u16 vcp7 : 4;
		u16 vcp6 : 4;
		u16 vcp5 : 4;
		u16 vcp4 : 4;
	};
};

/* CYC* */
union vdp2_cyc {
	u16 reg[8];
	struct {
		union vdp2_vcp a0;
		union vdp2_vcp a1;
		union vdp2_vcp b0;
		union vdp2_vcp b1;
	};
};

/* BGON */
union vdp2_bgon {
	r16 reg;
	struct {
		struct { // byte 0
			u8 nbg0 : 1;
			u8 nbg1 : 1;
			u8 nbg2 : 1;
			u8 nbg3 : 1;
			u8 rbg0 : 1;
			u8 rbg1 : 1;
			u8 : 2;
		} enable;
		
		union { // byte 1
			u8 raw;
			struct {
				u8 nbg0 : 1;
				u8 nbg1 : 1;
				u8 nbg2 : 1;
				u8 nbg3 : 1;
				u8 rbg0 : 1;
				u8 : 3;
			};
			struct {
				u8 rbg1 : 1;
				u8 exbg : 1;
				u8 : 6;
			};
		} transparency;
	};
};

/* MZCTL */
union vdp2_mzctl {
	r16 reg;
	struct {
		union { // byte 0
			u8 raw;
			struct {
				u8 nbg0 : 1;
				u8 nbg1 : 1;
				u8 nbg2 : 1;
				u8 nbg3 : 1;
				u8 rbg0 : 1;
				u8 : 3;
			};
			struct {
				u8 rbg1 : 1;
				u8 : 7;
			};
		} enable;
		
		struct { // byte 1
			u8 horizontal : 4;
			u8 vertical : 4;
		};
	};
};

/* SFSEL */
union vdp2_sfsel {
	r16 reg;
	struct {
		u16 nbg0 : 1;
		u16 nbg1 : 1;
		u16 nbg2 : 1;
		u16 nbg3 : 1;
		u16 rgb0 : 1;
		u16  : 11;
	};
	struct {
		u16 rgb1 : 1;
		u16 : 15;
	};
};

/* SFCODE */
union vdp2_sfcode {
	r16 reg;
	struct {
		r8 a;
		r8 b;
	};
};

/* CHCTL A/B */

union vdp2_chctl {
	r32 reg;

	struct {
		r16 chctla;
		r16 chctlb;
	};
	
	union {
		struct {
			// word 0, a
			u32 : 4;
			u32 nbg0 : 3; // bit 4-6
			u32 : 5;
			u32 nbg1 : 2; // bit 12-13
			u32 : 2;
			// word 1, b
			u32 : 1;
			u32 nbg2 : 1; // bit 1
			u32 : 3;
			u32 nbg3 : 1; // bit 5
			u32 : 6;
			u32 rbg0 : 3; // bit 12-14
			u32 : 1;
		};
		struct {
			u32 : 4;
			u32 rbg1 : 3; // bit 4-6
			u32 : 5;
			u32 exbg : 2; // bit 12-13
			u32 : 18;
		};
	} colorNb;
	
	struct {
		// word 0, a
		u32 : 1;
		u32 nbg0 : 1; // bit 1
		u32 : 7;
		u32 nbg1 : 1; // bit 9
		u32 : 6;
		// word 1, b
		u32 : 8;
		u32 rbg0 : 1; // bit 9
		u32 : 7;
	} mode;
	
	union {
		struct {
			// word 0, a
			u32 : 2;
			u32 nbg0 : 2; // bit 2-3
			u32 : 6;
			u32 nbg1 : 2; // bit 10-11
			u32 : 4;
			// word 1, b
			u32 : 10;
			u32 rbg0 : 1; // bit 10
			u32 : 5;
		} bitmap;
		
		union {
			struct {
				// word 0, a
				u32 nbg0 : 1; // bit 0
				u32 : 7;
				u32 nbg1 : 1; // bit 8
				u32 : 7;
				// word 1, b
				u32 nbg2 : 1; // bit 0
				u32 : 3;
				u32 nbg3 : 1; // bit 4
				u32 : 3;
				u32 rgb0 : 1; // bit 8
				u32 : 7;
			};
			struct {
				u32 rbg1 : 1;
				u32 : 31;
			};
		} character;
	} size;
};

/* BMPN A/B */
union vdp2_bmpn {
	r32 reg;
	
	struct {
		r16 bmpna;
		r16 bmpnb;
	};
	
	struct {
		u32 : 5;
		u32 nbg0 : 1; // bit 5
		u32 : 8;
		u32 nbg1 : 1; // bit 13;
		u32 : 1;
		
		u32 : 5;
		u32 rgb0 : 1; // bit 5
		u32 : 10;
	} priority;
	
	struct {
		u32 : 4;
		u32 nbg0 : 1; // bit 4
		u32 : 7;
		u32 nbg1 : 1; // bit 12
		u32 : 3;
		
		u32 : 4;
		u32 rbg0 : 1; // bit 4
		u32 : 11;
	} colorCalc;
	
	struct {
		u32 nbg0 : 3; // bit 0-2
		u32 : 5;
		u32 nbg1 : 3; // bit 8-10
		u32 : 5;
		
		u32 rbg0 : 2; // bit 0-2
		u32 : 14;
	} palNb;
};


/* cell mode */

/*
PNC*
PLSZ
MPOFN
MPOFR
MP*
MP* rotation
*/



/* PNC* */

union vdp2_pnc_bg {
	r16 reg;
	struct {
		u32 : 15;
		u32 patternDataSize : 1; // bit 15
	};
	struct {
		u32 characterNb : 5;	// bit 0-4
		u32 palNb : 3;		// bit 5-7
		u32 colorCalc : 1;	// bit 8
		u32 priority : 1;	// bit 9
		u32 : 4;
		u32 characterNbSup : 1;	// bit 14
		u32 : 1;
	} patternData1W;
};

union vdp2_pnc {
	r16 reg[5];
	struct {
		union vdp2_pnc_bg nbg0;
		union vdp2_pnc_bg nbg1;
		union vdp2_pnc_bg nbg2;
		union vdp2_pnc_bg nbg3;
		union vdp2_pnc_bg rbg0;
	};
	struct {
		union vdp2_pnc_bg rbg1;
		u32 : 32;
		u32 : 32;
	};
};

/* PLSZ */
union vdp2_plsz {
	r16 reg;
	struct {
		u32 nbg0 : 2;
		u32 nbg1 : 2;
		u32 nbg2 : 2;
		u32 nbg3 : 2;
		u32 ra : 2;
		u32 raScreenOver : 2;
		u32 rb : 2;
		u32 rbScreenOver : 2;
	};
};

union vdp2_mp_bg {
	r32 reg;
	struct {
		u32 a:6;
		u32:2;
		u32 b:6;
		u32:2;
		u32 c:6;
		u32:2;
		u32 d:6;
		u32:2;
	};
};


struct vdp2_mp {
	union vdp2_mp_bg nbg0;
	union vdp2_mp_bg nbg1;
	union vdp2_mp_bg nbg2;
	union vdp2_mp_bg nbg3;
};

union vdp2_mp_srp {
	r16 raw[16];
	struct {
		u32 a : 6;
		u32 : 2;
		u32 b : 6;
		u32 : 2;
		u32 c : 6;
		u32 : 2;
		u32 d : 6;
		u32 : 2;
		u32 e : 6;
		u32 : 2;
		u32 f : 6;
		u32 : 2;
		u32 g : 6;
		u32 : 2;
		u32 h : 6;
		u32 : 2;
		u32 i : 6;
		u32 : 2;
		u32 j : 6;
		u32 : 2;
		u32 k : 6;
		u32 : 2;
		u32 l : 6;
		u32 : 2;
		u32 m : 6;
		u32 : 2;
		u32 n : 6;
		u32 : 2;
		u32 o : 6;
		u32 : 2;
		u32 p : 6;
		u32 : 2;
	};
};

struct vdp2_mp_rp {
	union vdp2_mp_srp a;
	union vdp2_mp_srp b;
};

union vdp2_screen_scroll_direction_f {
	r16 reg[2];
	struct {
		u32 integer : 11;
		u32 : 13;
		u32 fractional : 8;
	};
};

union vdp2_screen_scroll_direction_s {
	r16 reg;
	struct {
		u32 integer : 11;
		u32 : 5;
	};
};

union vdp2_screen_scroll_direction_ci {
	r16 reg[2];
	struct {
		u32 integer : 2;
		u32 : 22;
		u32 fractional : 8;
	};
};

/* VDP2REG + 0x70 ~ 0x9A */
union vdp2_screen_scroll_value {
	r16 reg[22];
	/*struct {
		vdp2_screen_scroll_direction_f horizontal;
		vdp2_screen_scroll_direction_f vertical;
		u32 : 32; u32 : 32; u32 : 32; u32 : 32;
		struct {
		vdp2_screen_scroll_direction_ci horizontal;
		vdp2_screen_scroll_direction_ci vertical;
		} ci;
		u32 : 32; u32 : 32;
		u32 reductionZoom : 2;
		u32 : 14;
		
		struct {
			// zmctl
		  
		u32 verticalCellScrollEnable : 1;
		
		
		struct { // scrctl
			u32 enableVertical
			
		} scrollOption;
		} enable;
	} nbg0;*/
	struct {
		u32 : 32; u32 : 32;
		vdp2_screen_scroll_direction_f horizontal;
		vdp2_screen_scroll_direction_f vertical;
		u32 : 32; u32 : 32; u32 : 32; u32 : 32;
		struct {
		vdp2_screen_scroll_direction_ci horizontal;
		vdp2_screen_scroll_direction_ci vertical;
		} ci;
		u32 : 8;
		u32 reductionZoom : 2;
		u32 : 6;
	} nbg1;
	struct {
		 u32 : 32; u32 : 32; u32 : 32; u32 : 32;
		vdp2_screen_scroll_direction_s horizontal;
		vdp2_screen_scroll_direction_s vertical;
		 u32 : 32; u32 : 32; u32 : 32; u32 : 32;
		 u32 : 16;
	} nbg2;
	struct {
		 u32 : 32; u32 : 32; u32 : 32; u32 : 32;
		vdp2_screen_scroll_direction_s horizontal;
		vdp2_screen_scroll_direction_s vertical;
		 u32 : 32; u32 : 32; u32 : 32; u32 : 32;
		 u32 : 16;
	} nbg3;  
};

/* ZMCTL */
union vdp2_zmctl {
  
  
};


/* sc* / zm* */
//struct vdp2_scroll

/*

bgon
mozaic
special function
special function code
char control bg 0/1
char control bg 2/3/r0
bmp palete number bg 0/1
bmp palete number rgb0
pattern name control bg0
pattern name control bg1
pattern name control bg2
pattern name control bg3
pattern name control rbg0
plane size
map offset bg 0/3
map offset rotation a/b
map bg0 plane a/b
map bg0 plane c/d
map bg1 plane a/b
map bg1 plane c/d
map bg2 plane a/b
map bg2 plane c/d
map bg3 plane a/b
map bg3 plane c/d
map rotation a plane a/b
map rotation a plane c/d
map rotation a plane e/f
map rotation a plane g/h
map rotation a plane i/j
map rotation a plane k/l
map rotation a plane m/n
map rotation a plane o/p

map rotation b plane a/b
map rotation b plane c/d
map rotation b plane e/f
map rotation b plane g/h
map rotation b plane i/j
map rotation b plane k/l
map rotation b plane m/n
map rotation b plane o/p

horiz  int screen scroll bg0
horiz frac screen scroll bg0
vert  int screen scroll bg0
vert frac screen scroll bg0
horiz  int screen scroll increment bg0
horiz frac screen scroll increment bg0
vert  int screen scroll increment bg0
vert frac screen scroll increment bg0

horiz  int screen scroll bg1
horiz frac screen scroll bg1
vert  int screen scroll bg1
vert frac screen scroll bg1
horiz  int screen scroll increment bg1
horiz frac screen scroll increment bg1
vert  int screen scroll increment bg1
vert frac screen scroll increment bg1

horiz screen scroll bg2
vert  screen scroll bg2

horiz screen scroll bg3
vert  screen scroll bg3

reduction enable bg 0/1

Line and Vertical Cell Scroll Control (NBG0, NBG1)

Vertical Cell Scroll Table Address (NBG0, NBG1)

Vertical Cell Scroll Table Address (NBG0, NBG1)

Line Scroll Table Address (NBG0)
Line Scroll Table Address (NBG0)

Line Scroll Table Address (NBG1)
Line Scroll Table Address (NBG1)

*/

/* end of bg */

/*

Line Color Screen table Address
Line Color Screen table Address

Back Screen Table Address
Back Screen Table Address
Back Screen Table Address

Rotation Parameter Mode
Rotation Parameter Read Control

Coefficient Table Control

Coefficient Table Address Offset (Rotation Parameter A, B)

Screen-over Pattern Name (Rotation Parameter A)
Screen-over Pattern Name (Rotation Parameter B)

Rotation Parameter Table Address (Rotation Parameter A, B)
Rotation Parameter Table Address (Rotation Parameter A, B)

Window Position (W0, Horizontal Start Point)
Window Position (W0, Vertical Start Point)
Window Position (W0, Horizontal End Point)
Window Position (W0, Vertical End Point)

Window Position (W1, Horizontal Start Point)
Window Position (W1, Vertical Start Point)
Window Position (W1, Horizontal End Point)
Window Position (W1, Vertical End Point)

*/

/* bg back */

/*

Window Control (NBG0, NBG1)
Window Control (NBG2, NBG3)
Window Control (RBG0, Sprite)
Window Control (Rotation Window, Color Calculation Window)

Line Window Table Address (W0)
Line Window Table Address (W0)
Line Window Table Address (W1)
Line Window Table Address (W1)

sprite control

Shadow Control

Color RAM Address Offset (NBG0~NBG3)
Color RAM Address Offset (RBG0, Sprite)

Line Color Screen Enable

Special Priority Mode

Color Calculation Control
Special Color Calculation Mode

Priority Number (Sprite 0, 1)
Priority Number (Sprite 2, 3)
Priority Number (Sprite 4, 5)
Priority Number (Sprite 6, 7)

Priority Number (NBG0, NBG1)
Priority Number (NBG2, NBG3)
Priority Number (RBG0)

reserved

Color Calculation Ratio (Sprite 0, 1)
Color Calculation Ratio (Sprite 2, 3)
Color Calculation Ratio (Sprite 4, 5)
Color Calculation Ratio (Sprite 6, 7)

Color Calculation Ratio (NBG0, NBG1)
Color Calculation Ratio (NBG2, NBG3)
Color Calculation Ratio (RBG0)

Color Calculation Ratio (Line Color Screen, Back Screen)

Color Offset Enable
Color Offset Select

Color Offset A (RED)
Color Offset A (GREEN)
Color Offset A (BLUE)
Color Offset B (RED)
Color Offset B (GREEN)
Color Offset B (BLUE)

*/

/* end vdp2 */

/* vdp2 registers */

/* vdp2 */

//i try to use a union on large memory zone to define structures view based on each different bg :S
/* BG, multi registers access map */

/*
union vdp2_bg {
	u16 raw[128];
	
	/ * nbg0 map * /
	struct {
		/ * BGON * /
		u32 transparency:1; // bit 0
		u32 : 7;
		u32 enable : 1; // bit8
		u32 : 7;
		
		/ * MZCTL * /
		u32 mozaic : 1; // bit0
		u32 : 15;
		
		/ * SFSEL * /
		u32 specialFunctionCodeSelect : 1; // bit0
		u32 : 15;
		
		/ * SFCODE * /
		u32 : 16;
		
		union { // Character Control Registers 28/2A
			r16 raw[2];
			
			struct {
				/ * CHCTLA * /
				u32 charSize : 1;
				u32 mode : 1; // 1: bitmap mode, 0: cell mode
				u32 bitmapSize : 2;
				u32 colorCount  : 3; // 4-6
				u32  : 14;
				
				/ * CHCTLB * /
				u32  : 16;
			};
			
			union {
				struct {
				/ * CHCTLA * /
				u32 charSize : 1;
				
				
				u32 size   : 1; // 0
				
				u32 color  : 3; // 4-6
					
					
					
				u32 : 1;
				u32:4;
				u32 characterControl : 3; // chctlb bit 6/4
				u32 : 9;
				} character;
			} cell;
		
			union {
				/ * CHCTLA * /
				u32 size   : 1; // 0
				u32 color  : 3; // 4-6
			  
			} bitmap;
		};
		
		u32 : 16;
		
		
	} nbg0;
	
	struct {
		u32 : 1;
		u32 transparency:1;
		u32 : 7;
		u32 enable : 1;
		u32 : 6;
	} nbg1;
	
	struct {
		u32 : 2;
		u32 transparency:1;
		u32 : 7;
		u32 enable : 1;
		u32 : 5;
	} nbg2;
	
	struct {
		u32 : 3;
		u32 transparency:1;
		u32 : 7;
		u32 enable : 1;
		u32 : 4;
	} nbg3;
	
	struct {
		u32 : 4;
		u32 transparency:1;
		u32 : 7;
		u32 enable : 1;
		u32 : 3;
	} rgb0;
};*/

/* BGON */

/* fist byte, transparency */
/*
union vdp2_bgon_transparency {
	r8 reg;
	struct {
		
		
	};
};*/

/* second byte, enable display */
/*
union vdp2_bgon {
	r16 reg;
	struct {
		
		
	};
};*/

/*
union vdp2 {
	/ *   TVMD * / union vdp2_tvmd tvmd;
	/ *  EXTEN * / union vdp2_exten exten;
	/ * TVSTAT * / union vdp2_tvstat tvstat;
	/ * VRSIZE * / union vdp2_vrsize vrsize;
	/ * HCNT   * / union vdp2_hcnt hcnt;
	/ * VCNT   * / union vdp2_vcnt vcnt;
	/ * reserved * / u32 : 16;
	
	/ * RAMCTL * / union vdp2_ramctl ramctl;
	
	/ * CYC* * / vdp2_cyc cyc;
	
	/ * BG0N * /
	union vdp2_bgon bg;
	
	
	
};
*/

#pragma pack(pop)

#endif
