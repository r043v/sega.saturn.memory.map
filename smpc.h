#ifndef _smpc_
#define _smpc_

using namespace std;

#pragma pack(push, 1)

namespace SMPC
{
	enum __attribute__ ((packed)) smpc_reg_value {
		/* iosel */    
		automatic = 0,
		  manual = 1,
		/* exle */
		  disable = 0,
		  enable = 1,
		/* intback command, input0 */
		    stop = (1<<6),
		    more = (1<<7)
	};

	union smpc_reg {
		u8 raw[2];
		struct {
			u8 junk;
			volatile union {
				u8 reg; /* byte access only */
				enum smpc_reg_value value : 8;
			};
		};
	};

	union smpc_peripheral_data_collection_mode {
		u8 raw;
		struct {
			u8 :1;
			u8 ope :1;
			u8 :1;
			u8 pen :1;
			union {
				enum {
					B15 = 0x00,
					B255 = 0x01,
					  B0 = 0x11
				} mode;
				
			} port[2];
		};
	};

	enum smpc_cmd {
		/* resetable */
		master_SH2_on = 0x00,
		mshon = 0x00,
		slave_SH2_on = 0x02,
		sshon = 0x02,
		slave_SH2_off = 0x03,
		sshoff = 0x03,
		sound_on = 0x06,
		sndon = 0x06,
		sound_off = 0x06,
		sndoff = 0x06,
		cd_on = 0x08,
		cdon = 0x08,
		cd_off = 0x09,
		cdoff = 0x09,
		reset = 0x0d,
		sysres = 0x0d,
		clock_352 = 0x0e,
		ckchg352 = 0x0e,
		clock_320 = 0x0f,
		ckchg320 = 0x0f,
		nmi_request = 0x18,
		nmireq = 0x18,
		reset_enable = 0x19,
		resenab = 0x19,
		reset_disable = 0x1a,
		resdisa = 0x1a,
		/* non resetable */
		interupt_back = 0x10,
		intback = 0x10,
		memory_setting = 0x17,
		setsmem = 0x17,
		/* rtc */
		time_setting = 0x16,
		settime = 0x16
	};

	union smpc_cmd_reg {
		u8 raw[2];
		struct {
			u8 junk;
			enum smpc_cmd name : 8;
			//u8 name;
		};
	};
	/*
	enum smpc_iosel_mode {
		smpc_control = 0,
		sh2_direct = 1
	};

	union smpc_iosel_reg {
		u8 raw;
		struct {
			enum smpc_iosel_mode iosel1 : 1;
			enum smpc_iosel_mode iosel2 : 1;
			u8 :6;
		};
	};*/

	union smpc {
		volatile u8 raw[128];
		volatile struct {
			/* 8 bits */
			smpc_reg in[7];		// read 14
			u8 junk0[16]; // 16 30
			
			u8 : 8;
			enum smpc_cmd command;
			
			//smpc_cmd_reg command;	// write 2 32
			smpc_reg out[32];	// write 64 96
			smpc_reg sr;		// read 2 98
			smpc_reg sf;		// read / write 2 100
			u8 junk1[16];	// 116
			/* 7 bits */
			smpc_reg pd1; /* r/w 2 118 */
			smpc_reg pd2; /* r/w 2 120 */
			smpc_reg dd1; /* w 2 122 */
			smpc_reg dd2; /* w 2 124 */
			/* 2 bits */
			u8 :8;
			smpc_reg iosel; /* w 2 126 */
			smpc_reg exle;  /* w 2 128 */
		};
	};

} /* /namespace SMPC */

#define SMPC_ADR 0x00100000;

union SMPC::smpc *smpc = (union SMPC::smpc*)SMPC_ADR;

void resetPlease(void){
	while(smpc->sf.value == SMPC::enable);
	smpc->sf.value = SMPC::enable;
	smpc->command = SMPC::reset;
}

#pragma pack(pop)

#endif
