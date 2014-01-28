#ifndef _bitfield_
#define _bitfield_

#pragma pack(push, 1)

/* 8b bitfield */
union b8 {
	u8 raw;
	struct {
	    u32 b0  : 1;
	    u32 b1  : 1;
	    u32 b2  : 1;
	    u32 b3  : 1;
	    u32 b4  : 1;
	    u32 b5  : 1;
	    u32 b6  : 1;
	    u32 b7  : 1;
	};
};

/* 16b bitfield */
union b16 {
	u16 raw;
	struct {
	    u32 b0  : 1;
	    u32 b1  : 1;
	    u32 b2  : 1;
	    u32 b3  : 1;
	    u32 b4  : 1;
	    u32 b5  : 1;
	    u32 b6  : 1;
	    u32 b7  : 1;
	    u32 b8  : 1;
	    u32 b9  : 1;
	    u32 b10 : 1;
	    u32 b11 : 1;
	    u32 b12 : 1;
	    u32 b13 : 1;
	    u32 b14 : 1;
	    u32 b15 : 1;
	};
};

/* 32b bitfield */
union b32 {
	u32 raw;
	struct {
	    u32 b0  : 1;
	    u32 b1  : 1;
	    u32 b2  : 1;
	    u32 b3  : 1;
	    u32 b4  : 1;
	    u32 b5  : 1;
	    u32 b6  : 1;
	    u32 b7  : 1;
	    u32 b8  : 1;
	    u32 b9  : 1;
	    u32 b10 : 1;
	    u32 b11 : 1;
	    u32 b12 : 1;
	    u32 b13 : 1;
	    u32 b14 : 1;
	    u32 b15 : 1;
	    u32 b16 : 1;
	    u32 b17 : 1;
	    u32 b18 : 1;
	    u32 b19 : 1;
	    u32 b20 : 1;
	    u32 b21 : 1;
	    u32 b22 : 1;
	    u32 b23 : 1;
	    u32 b24 : 1;
	    u32 b25 : 1;
	    u32 b26 : 1;
	    u32 b27 : 1;
	    u32 b28 : 1;
	    u32 b29 : 1;
	    u32 b30 : 1;
	    u32 b31 : 1;
	};
};

/* 8b register */
typedef union {
	u8 raw;
	union b8 bits;
} r8;

/* 16b register */
typedef union {
	u16 raw;
	union b16 bits;
	struct {
		r8 B0;
		r8 B1;
	};
} r16;

/* 32b register */
typedef union {
	u32 raw;
	union b32 bits;
	struct {
		r16 W0;
		r16 W1;
	};
	struct {
		r8 B0;
		r8 B1;
		r8 B2;
		r8 B3;
	};
} r32;

#pragma pack(pop)

#endif
