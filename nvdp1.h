#ifndef _
#define _

using namespace std;

/* vdp1 */
#define VDP1_ADR 0x5c00000

#define VDP1VRAM VDP1_ADR
#define VDP1VRAMSIZE (512*1024)

#define VDP1FB0 (VDP1_ADR+VDP1VRAMSIZE)
#define VDP1FB0SIZE (256*1024)

#define VDP1FB1 (VDP1FB1+VDP1FB0SIZE)
#define VDP1FB1SIZE VDP1FB0SIZE

#define VDP1REG (VDP1_ADR+0x100000)

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

namespace vdp1
{
	// res
	// color deep
  
	// interlace
  
	// change mode
	// change time
	
  
  
	u32 setMode(u32 width,u32 height,u32 rotate){
		u32 interlace = height > 239;
		u32 highres = width > 351;
		
		if(	( highres && deep != 8 )
		||	( rotate && ( highres || interlace ) )
		) return 0;
		
		
		if(width > 351){
			interlaceheight > 239 ? 
		  
		  
		}
	  
	  
	  
	}
  
  
	/* TVMR */
	enum __attribute__ ((__packed__)) screen_mode {
		    normal = 0b000,
			hd = 0b100,
		rotation16 = 0b010,
		rotation8 = 0b110,
		      hdtv = 0b001
	};
      
	/* registers */

	/* TVMR */
	enum __attribute__ ((__packed__)) tvmr__tvm {
		    normal = 0b000,
			hd = 0b100,
		rotation16 = 0b010,
		rotation8 = 0b110,
		      hdtv = 0b001
	};

	union tvmr {
		r16 reg;
		struct { // zz
			u16 : 12;
			u16 vbe : 1;
			tvmr__tvm tvm : 3;
		};
	};

	/* FBCR */
	enum __attribute__ ((__packed__)) fbcr__plot_lines { // dil, die
		both = 0b00, // no/single interlace
		even = 0b10, //double interlace
		odd = 0b11, //double interlace
		double_interlace = 0b11,
		single_interlace = 0b00,
	};

	union fbcr {
		r16 reg;
		struct { // zz
			u16 : 11;
			u16 eos : 1; // enum { even=0,odd=1 }
			u16 die : 1;
			u16 dil : 1;
			u16 fcm : 1;
			u16 fct : 1;
		};
		struct { // zz
			u16 : 12;
			fbcr__plot_lines plot_line : 2;
			u16 : 2;
		};
		struct { // zz
			u16 : 12;
			fbcr__plot_lines interlace : 2;
			u16 : 2;
		};
	};

	/* TVMR + FBCR */

	union fbcmr {
		u32 raw;
		struct { // zz
			u32 : 14;
			u32 fcm : 1;
			u32 fct : 1;
			u32 : 12;
			u32 vbe : 1;
			u32 : 3;
		} reg;
	};

	enum fbcmr__mode {
		one_cycle = 0,
		manual_erase = ( 1<<17 ),
		manual_change = ( 1<<16 | 1<<17 ),
		manual_erase_change = ( 1<<3 | 1<<16 | 1<<17 )
	};

	/* PTMR */

	enum __attribute__ ((__packed__)) fbcr__ptm {
		idle = 0b00,
		manual = 0b10,
		automatic = 0b01
	};

	union ptmr {
		r16 reg;
		struct { // zz
			u16 : 14;
			fbcr__ptm ptm : 2;
		};
	};

	/* EWDR */
	union ewdr {
		r16 reg;
		u16 raw;
		u16 color; // 16b
		struct {   // 8b
			u8 even;
			u8 odd;
		};
	};

	/* EWLR */
	union ewlr {
		r16 reg;
		struct { // zz
			u16 : 1;
			u16 ewX1 : 6;
			u16 ewY1 : 9;
		};
	};

	/* EWRR */
	union ewrr {
		r16 reg;
		struct {
			u8 ewY3;
			u8 ewX3;
		};
	};

	/* EW* */
	union r_ew {
		r16 reg[3];
		struct {
			union {
				u16 color; // 16b
				struct {   // 8b
					u8 even;
					u8 odd;
				} colors;
			};
			
			struct { //zz
				u16 : 1;
				u16 x1 : 6;
				u16 y1 : 9;
			};
			
			struct {
				u8 y3;
				u8 x3;
			};
		};
	};

	/* ENDR */
	union endr {
		r16 reg;
		u16 endr;
	};

	/* EDSR */
	union r_edsr {
		r16 reg;
		struct { // zz
		  	u16 : 14;
			u16 cef : 1;
			u16 bef : 1;
		};
	};

	/* LOPR */
	union r_lopr {
		r16 reg;
		struct { // zz
			u16 lopr : 14;
			u16 : 2;
		};
	};

	/* COPR */
	union r_copr {
		r16 reg;
		struct { // zz
			u16 copr : 14;
			u16 : 2;
		};
	};

	/* MODR */
	union r_modr {
		r16 reg;
		struct { // zz
			u16 ver  : 4;
			u16 : 3;
			u16 ptm1 : 1;
			u16 eos  : 1;
			u16 die  : 1;
			u16 dil  : 1;
			u16 fcm  : 1;
			u16 vbe  : 1;
			u16 tvm  : 3;
		};
	};

	/* /registers */

	/* commands */

	/* .ctrl.jp */
	enum __attribute__ ((__packed__)) commands_jump_mode {
		  jump_auto = 0b000, // auto jump to next command
		jump_assign = 0b001, // jump to .link
		  jump_call = 0b010, // subroutine
		jump_return = 0b011, // return
		  jump_skip_next = 0b100, // next, not proceed this one
		jump_skip_assign = 0b101, // jump to .link, not proceed this one
		  jump_skip_call = 0b110, // subroutine, not proceed this one
		jump_skip_return = 0b111  // return, not proceed this one
	};

	/* .ctrl.zp */
	enum __attribute__ ((__packed__)) commands_zoom_point {
		zoom_specifies     = 0b0000,
		zoom_up_left       = 0b0101,
		zoom_up_center     = 0b0110,
		zoom_up_right      = 0b0111,
		zoom_center_left   = 0b1001,
		zoom_center_center = 0b1010,
		zoom_center_right  = 0b1011,
		zoom_lower_left    = 0b1101,
		zoom_lower_center  = 0b1110,
		zoom_lower_right   = 0b1111
	};

	/* .ctrl.comm */
	enum __attribute__ ((__packed__)) commands_command {
		draw_end = 0b0000, /* +bit 15, .ctrl.end at 1 */
		/* textured */
		draw_sprite_normal = 0b0000,
		draw_sprite_scaled = 0b0001,
		draw_sprite_distorted = 0b0010,
		/* un textured */
		draw_polygon = 0b0100,
		draw_polyline = 0b0101,
		draw_line = 0b0110,
		/* clipping */
		user_clipping = 0b1000,
		system_clipping = 0b1001,
		local_coordinate = 0b1010
	};	
	
	/* .pmod.color.mode */
	enum __attribute__ ((__packed__)) commands_color_mode {
		bank_16 = 0b000,
		lookup_16 = 0b001,
		bank_64 = 0b010,
		bank_128 = 0b011,
		bank_256 = 0b100,
		bank_32k = 0b101
	};


	typedef union __attribute__ ((__packed__))  {
		r16 reg[16];
		
		struct {
			union {
				r16 reg;
				struct { // zz
					u16 : 10;
					u16 y : 1;
					u16 x : 1;
					u16 : 4;
				} dir;
				struct { // zz
					u16 end : 1;
					enum commands_jump_mode  jp : 3;
					enum commands_zoom_point zp : 4;
					u16 : 4;
					enum commands_command comm : 4;
				};
			} ctrl;
			
			union {
				r16 reg;
				struct { // zz
					u16 : 2;
					u16 offset8 : 14;
				};
			} link;
			
			union {
				r16 reg;
				union {
					struct { // zz
						u16:13;
						u16 semi_background : 1;
						u16 semi_graphics : 1;
						u16 gouraud : 1;
					} calc;
					struct { // zz
						u16 : 10;
						u16 mode : 3;
						u16 : 3;
					};
				} color;
				struct{ // zz
					u16 mon   : 1;
					u16 : 2;
					u16 hss   : 1;
					u16 pclp  : 1;
					u16 clip  : 1;
					u16 cmod  : 1;
					u16 mesh  : 1;
					u16 ecd : 1;
					u16 spd : 1;
					u16 : 6;
				};
			} pmod;
			
			r16 colr;
			r16 srca;
			
			union {
				r16 reg;
				struct { // zz
					u16 : 2;
					u16 x : 6;
					u16 y : 8;
				};
			} size;
			
			union {
				r16 reg[2];
				struct { // zz
					u16 ycode : 5;
					u16 y : 11;
					u16 xcode : 5;
					u16 x : 11;
				};
			} a;
			
			union {
				r16 reg[2];
				struct { // zz
					u16 ycode : 5;
					u16 y : 11;
					u16 xcode : 5;
					u16 x : 11;
				};
			} b;
			
			union {
				r16 reg[2];
				struct { // zz
					u16 ycode : 5;
					u16 y : 11;
					u16 xcode : 5;
					u16 x : 11;
				};
			} c;
			
			union {
				r16 reg[2];
				struct { // zz
					u16 ycode : 5;
					u16 y : 11;
					u16 xcode : 5;
					u16 x : 11;
				};
			} d;
			
			r16 gouraud;
			
			u16 : 16;
		};
	} command;

	/* /commands */

	union vdp1 {
		u8 raw[2048*1024]; // 2MB
		struct {
			union { /* vram, 512KB */
				u8 vram[512*1024];
				command commands[512*1024/32];
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
							/* TVMR */ union tvmr tvmr;
							/* FBCR */ union fbcr fbcr;
							};
							union fbcmr fbcmr;
						};
						/* PTMR */ union ptmr ptmr;
						union {
							struct {
								/* EWDR */ union ewdr ewdr;
								/* EWLR */ union ewlr ewlr;
								/* EWRR */ union ewlr ewrr;
							};
							r_ew ew;
						};
						/* ENDR */ union endr endr;
						/* reserved */ u16 : 16;
						/* EDSR */ union r_edsr edsr;
						/* LOPR */ union r_lopr lopr;
						/* COPR */ union r_copr copr;
						/* MODR */ union r_modr modr;
					};
				};
				
				u8 empty[512*1024 - 12*2];
			};
			
			u8 prohibited[512*1024]; // 512 KB
		};
	};

	union vdp1 *vdp1 = (union vdp1*)VDP1_ADR;
	
	void randVram(){
		u16 * p = (u16*)vdp1->vram; u16 *l = &p[(512*1024/2) - 1];
		while(p != l) *p++ = rand()%65536;
	}

	void zeroVram(){
		u16 * p = (u16*)vdp1->vram; u16 *l = &p[(512*1024/2) - 1];
		while(p != l) *p++ = 0;
	}
	
	inline void copyCmd(command*s,command*d){
		u32 n = 16; while(--n) d->reg[n].raw = s->reg[n].raw;
	}
	
	inline void zeroCmd(command*c){
		//u32 *p = (u32*)c; *p = p[1] = p[2] = p[3] = 0;
		u32 n = 16; while(n--) c->reg[n].raw = 0;
	}

	inline command newCmd(void){
		command c; zeroCmd(&c); return c;
	}
	
	u32 palOff = 256*8; //*8
	u32 sprOff = 512*8;
	
	void initSprite(void){
		// create a 8b 64*64px various colored sprite at offset 512
		u8 * spr = &vdp1->vram[sprOff];
		u32 n = 64*64; while(--n) spr[n] = 7; //32+(n%200);
	}

	void clearCmd(void){
		//command c = newCmd();
		command c; zeroCmd(&c);// u32 *p = (u32*)&c; *p = p[1] = p[2] = p[3] = 0;// memset(&c,0,32); // temp command register
		c.ctrl.end = 1;
		copyCmd(&c,&vdp1->commands[0]);
		/*
		c.ctrl.comm = 0;
		c.ctrl.dir.x = 0; // normal x way
		c.ctrl.dir.y = 0; //   ..   y  ..
		c.ctrl.jp = jump_return;
		c.ctrl.zp = zoom_center_center;
		c.ctrl.end = 0;
		copyCmd(&c,&vdp1->commands[0]);*/
	}

	void log_u16(u16 *nb){
		u16 i = *nb;
		printl(" 0x%x [",nb);
		u32 n=15;
		do {
			if(n==7)
				printl(" ] [");
			printl(" %u",(i>>n)&1);
		} while(n--);
		printlr(" ] 0x%4x %u",i,i);
	}

	void setSprite(void){
		command c; zeroCmd(&c); // temp command register
		
		c.ctrl.comm = draw_sprite_normal;
		c.ctrl.dir.x = 0; // normal x way
		c.ctrl.dir.y = 0; //   ..   y  ..
		c.ctrl.jp = jump_auto;
		c.ctrl.zp = zoom_center_center;
		c.ctrl.end = 0;
		c.link.offset8 = 0;
		c.pmod.color.calc.gouraud = 0;
		c.pmod.color.calc.semi_background = 0;
		c.pmod.color.calc.semi_graphics = 0;
		c.pmod.color.mode = bank_256;
		
		c.pmod.hss = 0;  // no high speed shrink
		c.pmod.ecd = 1;  // disable end char code

		c.pmod.spd = 1; // no transparent color
		
		c.pmod.mesh = 0; // no mesh
		c.pmod.pclp = 1; // no clip and no h invert
		c.pmod.clip = 0; // no user clipping
		c.pmod.cmod = 0; // no user clipping
		
		c.pmod.mon = 0;  // msb on (8bit)
		c.size.x = 8; // 64px
		c.size.y = 64;
		c.a.x = 128;
		c.a.y = 16;
		c.srca.raw = (sprOff/8)<<2; // character pattern
		
		u8 * cp = (u8*)&c;
		
		for(u32 n=0;n<32;n++)
			printlr("%u .. 0x%x .. %u",n,cp[n],cp[n]);
		for(u32 n=0;n<16;n++){
			printl("reg %2u ",n);
			log_u16(&c.reg[n].raw);
		}
		
		printlr("comm : 0x%x ",c.ctrl.comm);
		
		//memcpy(&vdp1->commands[0],&c,32); // temp command to first command
		
		//for(u32 n=0;n<16;n++)
		//	vdp1->commands[0].reg[n].raw = c.reg[n].raw;
		
		copyCmd(&c,&vdp1->commands[0]);
		
		zeroCmd(&c);
		c.ctrl.end=1;
		copyCmd(&c,&vdp1->commands[1]);
		
/*		for(u32 n=0;n<16;n++){
			printl("reg %2u ",n);
			log_u16(&c.reg[n].raw);
		}*/
		
		/*printlr("link 0x%x",&vdp1->commands[0].link.reg.raw);
		printlr("color calc 0x%x",&vdp1->commands[0].pmod.color.calc);
		printlr("color mode 0x%x",&vdp1->commands[0].colr);
		printlr("src 0x%x",&vdp1->commands[0].srca.raw);
		printlr("size 0x%x",&vdp1->commands[0].size);
		printlr("a 0x%x",&vdp1->commands[0].a);
		printlr("b 0x%x",&vdp1->commands[0].b);
		printlr("c 0x%x",&vdp1->commands[0].c);
		printlr("d 0x%x",&vdp1->commands[0].d);
		printlr("gouraud 0x%x",&vdp1->commands[0].gouraud);*/
	}
	
} /* /namespace VDP1 */

} /* /saturn */

union VDP1::vdp1 *vdp1 = VDP1::vdp1;

	/* macro */

#define vdp1_fbcmr__set(m) { vdp1->fbcmr.raw |= m; }

#pragma pack(pop)

#endif