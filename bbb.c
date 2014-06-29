/* 
  -- bbb - memory map of memory state memory -
 0  0  u32 pointer to memory
 4  1  u32 total free
 8  2  u32 reserved
12  3  u32 freeblocks nb, default : 1
16  4 -- freeblocks, 8b each
       u32 size
       u32 offset

       u32 0xffffffff
       u32 0
eof

*/


#define maxBbb 4
#define bbbHeadSize 4096 // 16k

/* bbb - bubble bloc */
void * bbb(u32*h,u32 s){
	static u32 mbbb[maxBbb][bbbHeadSize] = { { 0,0 }, { 0,0 } }; // memory for each memory state

	if(s>>16 == 0xffff && h){ // new memory zone
		u8*m = (u8*)h;
		s = (s&0xffff)*1024; // kb ftw
		u32 n=0; while(mbbb[n][0] && n<maxBbb) n++; if(n == maxBbb) return 0;
		u32 *oo = mbbb[n], *o; o = oo; 
		*o++ = (u32)m; *o++ = s; *o++ = 0; *o++ = 1; // header
		*o++ = s; *o++ = 0; // first bloc with full size
		*o++ = 0xffffffff; *o = 0; // magic stop
		return (void*)oo;
	}
	
	u32 * nb = &h[3];
	u32 * bs = &h[4];
	u32  * b = bs;
	 u8  * m = (u8*)*h;

	while(s%4) s++;
	while(*b < s) b+=2;
	if(*b == 0xffffffff) return 0; // you're ass fucked
	m += b[1]; // compute out adress
	// split bloc
	*b -= s; b[1] += s; // reduce size, up offset
	if(!*b){ // bloc is full, delete
		// switch with the last one
		u32 * l = &bs[*nb]; while(*l == 0xffffffff && !l[1] && l>bs) l-=2; // search
		*b = *l; b[1] = l[1]; // switch
		*l = 0xffffffff; l[1] = 0; // magic stop
		*nb -= 1; // decrease bloc nb
	}	return (void*)m;
}

int main(void){
	
	
	
}
