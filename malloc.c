#include "devcart.h"

#define specialLogFlag (0x05A00000+511*1024+1000)
#define specialLogBf (specialLogFlag+4)
#define specialLogBfSz (specialLogFlag+8)
#define specialLogSize (16*1024)

void yalog(const char * s){
	static u8 bf[specialLogSize];
	static u8 init = 0;
  
	if(!init)
	{	memset(bf,0,specialLogSize);
		*(u32*)specialLogBf = (u32)bf;
		*(u32*)specialLogBfSz = specialLogSize;
		*(u32*)specialLogFlag = 0;
		init = 1;
	}
  
	u8 * b = bf;
	if(*(u32*)specialLogFlag == 0) *b = 0;
	*(u32*)specialLogFlag = 42;
	strcat((char*)b,s);
	*(u32*)specialLogFlag = (u32)0x42424242;
}

void ftlog(const char * s){
	while(*s){
		while((USB_FLAGS & USB_TXE) != 0); USB_FIFO = *s++;
	};
}

char dbglogbf[4096];

void printl(const char * format, ...)
{	va_list va;
	va_start(va, format);
	vsprintf(dbglogbf,format,va);
	yalog(dbglogbf);
	//ftlog(dbglogbf);
}

void printlr(const char * format, ...)
{	va_list va;
	va_start(va, format);
	vsprintf(dbglogbf,format,va);
	strcat(dbglogbf,"\n");
	yalog(dbglogbf);
	//ftlog(dbglogbf);
}

/* 
 * * bbb - bubblebloc -
 * (c) 2013 noferi mickaël
 *    under creative common 3.0 by-nc-sa
 * *
 * ** * /
 
  -- head map --
 0  0  u32 pointer to memory
 4  1  u32 reserved
 8  2  u32 total buffer size
12  3  u32 reserved

16     4~2044 -- free list
         u32 size
         u32 offset

    2048~4096 -- allocated list
         u32 offset
         u32 size
eof

*/

#define maxBbb 4 /* max memory number to manage */
#define bbbHeadSize 4096  // total head memory, in u32
#define bbbHeadAlloc 2048 // head offset to allocated list

/* bnew, declare memory zone
    bbbhead bnew(adress,sizeInKB);
 */
#define bnew(a,s) (u32*)bbb((u32*)a,s,1)

/* bmalloc, bfree */
#define bfree(a,h) bbb((u32*)h,(u32)a,-1)
#define bmalloc(h,s) bbb(h,s,0)

void * bbb(u32*h,u32 s,char a){
	if(a!=0){ // special case
		static u32 mbbb[maxBbb][bbbHeadSize] = { { 0,0 }, { 0,0 } }; // memory for each memory state
		/* free */
		if(a < 0){
			u32 *b = (u32*)s; u8  *m = (u8*)*b;
			u32 * mb = b; mb += bbbHeadAlloc;
			u32  o = (u32)h - (u32)m;
			u32 tmem = b[2];
			
			//printlr("\n *** full memory : %u",tmem);
			
			u32 * bs = &b[4];
			      b  = bs;
			     
			//printl("free offset %u ",o);
		
			while(*mb != o && *mb != 0xffffffff) mb += 2; // search offset
			
			if(*mb == 0xfffffffe || *mb == 0xffffffff)
			{ /*printlr("free 0x%x fail",o);*/ return (void*)0xffffffff; } // not found
			
			// add a free bloc
			while(*b && *b != 0xffffffff) b+=2; // search free freebloc
			if(*b == 0xffffffff)
			{	//printlr("create a freebloc");
				b[2] = 0xffffffff; b[3] = 0;
			}
			
			//printlr("new free bloc at %u size %u",mb[0],mb[1]);
			*b = mb[1]; b[1] = *mb; // copy from malloc to free list
			// remove allocated bloc
			*mb = 0xfffffffe; mb[1] = 0;
			
			/* bubble part with bloc fusion */
			{	//u32 n = 0;
				u32*fp = bs;
				u32 anyChange = 0;
				
				do {
					fp = bs;
					anyChange = 0;
					//printlr("bubble loop");
					while(*fp != 0xffffffff){
						//printlr("bubble progress");
						u32 s1 = *(fp+1);
						u32 s2 = *(fp+3);
						u32 sz1 = *fp;
						u32 sz2 = *(fp+2);
						u32 e1 = s1 + sz1;
						//u32 e2 = s2 + sz2;
						
						//printlr("1.%u.%u",s1,e1);
						//printlr("2.%u.%u",s2,e2);

						if(s2 < s1 && sz1 != 0xffffffff && sz2 != 0xffffffff){
							// bubble on offset value
							//printlr("switch bloc");
							/* switch blocs */
							u32 tmp = *fp; // switch size
							*fp = *(fp+2);
							*(fp+2) = tmp;
							
							tmp = *(fp+1); // switch offset
							*(fp+1) = *(fp+3);
							*(fp+3) = tmp;
							
							/* refresh var */
							
							s1 = *(fp+1);
							s2 = *(fp+3);
							sz1 = *fp;
							sz2 = *(fp+2);
							e1 = s1 + sz1;
							//e2 = s2 + sz2;
							
							anyChange = 1;
						}

						/* check if colide */
						if(sz1 && e1 == s2){ /* yes ! */
							if(sz2 && sz2 != tmem){
//								printlr("fusion of two free bloc => %u.%u",s1,e1");
								*fp += *(fp+2); // add sizes
								*(fp+2) = 0;    // old block zero size
								*(fp+3) = tmem; // and full mem offset
								anyChange = 1;
							}
						}
						
						fp += 2;
					};
					
					{
//					  printlr("end."); /* trim empty allocated blocs */
					  fp -= 2; while(!*fp) fp -= 2;
					  fp += 2;
					  *fp = 0xffffffff; fp[1] = 0;
					}
				} while(anyChange);
			}
			/*
			{	u32*zz = bs;
				u32 n = 0;
				printlr("\n** free blocs **");
				while(*zz != 0xffffffff && n<10)
				{	n++;
					printlr("free bloc %u, size %u offset %u",n,*zz,zz[1]);
					zz+=2;
				}
				printlr("**\n");
			}
			*/
			return 0;
		}
		
		/* new memory zone */
		s = s*1024; // kb
		u32 n=0; while(mbbb[n][0] && n<maxBbb) n++;
		if(n == maxBbb) return 0;
		u32 *oo = mbbb[n], *o; o = oo; memset(o,0,bbbHeadSize*sizeof(u32));
		*o++ = (u32)h; *o++ = s; *o++ = s; *o++ = 1; // header
		u32 *ooo = &oo[bbbHeadAlloc]; // allocated blocs
		*o++ = s; *o++ = 0; // first bloc with full size
		*o++ = 0xffffffff; *o = 0; // magic stop for free bloc
		*ooo++ = 0xffffffff; *ooo = 0; // magic stop for malloc bloc
		//printlr("new %uk memory to manage [0x%x]\nbloc head 0x%x",s>>10,*oo,(u32)oo);
		return (void*)oo;
	}
	
	/* malloc */
	u32 * bs = &h[4];
	u32 * mb = &h[bbbHeadAlloc];
	u32  * b = bs;
	 u8  * m = (u8*)*h;
	 
	//printl("allocating .. %u",s);
	
	while(s%4) s++;
	
	while(*b < s) b+=2;
	if(*b == 0xffffffff){ // you're ass fucked
		//printlr("allocating %u bytes fail !",s);
		return 0;
	}
	
	m += b[1]; // compute out adress
	
	// add to malloc list
	while(*mb != 0xffffffff && *mb != 0xfffffffe) mb+=2; // find new malloc bloc
	//if(*mb) { mb[2] = 0xffffffff; mb[3] = 0; } // move stop bloc
	if(*mb == 0xffffffff){ /*printlr("move malloc bloc end");*/ mb[2] = 0xffffffff; mb[3] = 0; } // reach end, move stop
	//else printlr("*** use an empty malloc bloc ***");
	*mb = b[1]; mb[1] = s; // write bloc info
	//printlr(" * added malloc bloc, offset %u size %u",*mb,mb[1]);

	/*{	u32*zz = &h[bbbHeadAlloc];
		u32 n = 0;
		printlr("\n** allocated blocs **");
		while(*zz != 0xffffffff && n<10)
		{	n++;
			if(*zz == 0xfffffffe)
				printlr("malloc bloc %u empty",n);
			else	printlr("malloc bloc %u offset %u size %u",n,*zz,zz[1]);
			zz+=2;
		}
		printlr("**\n");
	}*/

	//printlr("reduct freebloc,\n current %u bytes free, offset : %u",*b,b[1]);

	// update free bloc
	*b -= s; b[1] += s; // reduce size, up offset
	
	if(!*b){ // bloc is full, delete
		u32 * l = bs;
//		printlr("no space left, delete this free bloc");
		while(*l != 0xffffffff) { /*printl("*");*/ l+=2; }; // jump to last
		//printlr("deleting the free bloc");
		// switch with the last one
		while(*l == 0xffffffff && !l[1] && l>bs){ l-=2; /*printl("o");*/ } // search
		if(l > bs){
		*b = *l; b[1] = l[1]; // switch last with current
		*l = 0xffffffff; l[1] = 0; // magic stop on switched bloc
		
//		printlr("switched free bloc");
		
		} else { /*printlr("\nnot any free bloc left");*/ *b = 0xffffffff; b[1]=0; }
	}// else printlr("    =>   %u bytes free, offset : %u",*b,b[1]);
	
	//printlr("allocated offset : %u",((u32)m - *h));
	
	return (void*)m;
}



/*
	u32 * c; // current bloc
	
	if(b[*l] >= s){
	    // need size is in bubbled range
		u32 end = *l;
		u32  p  = end;
		while(p && b[p] > s) if(p > 12) p -= 12; else p = 0; // back down 6 by 6
		while(b[p] < s) p++++; // advance and find the first bloc with good size
		if(b[p] == 0xffffffff) return 0; // eof
		u32 bsize = b[p], off = b[p+1];
		void *m = (void*)(*h + off); // current bloc
		// split the bloc
		b[p+1] += s; // up bloc offset by the size
		*b -= s; // and down his free size
		c = b;
		
	} else {
	    // need size is upper than max bubbled one,
	    // restart bubbling at this offset and up one by one
	    while(done){
		done = 0;
		b = &b[*l];
		while(*b < s)
		{	if(*b > b[2]){ // bubble sort, next is bigger
				// move offset
				u32 tmp = b[1];
				b[1] = b[3];
				b[3] = tmp;

				// move size
				tmp = *b;
				*b = b[2];
				b[2] = tmp;
				
				*l = &h[1]-&b[2]; // update last bubbled position to next bloc
			}
			
			if(b[2] == 0xffffffff && b[3] == 0) return 0; // eof, your assfucked
			b++; b++; // switch to next bloc
		}
	    }
	}
	
	
	
	return m;
}
*/


/*
 * 	do {	done = 0;
		u32 z = *nb - 2;
	  	while(z--){
			if(*b <= size)
			{
			  
			  
			}
			  
			if(*b > b[2])
			{	 // move offset
				u32 tmp = b[1];
				b[1] = b[3];
				b[3] = tmp;

				// move size
				tmp = *b;
				*b = b[2];
				b[2] = tmp;
			}
			
			if(*b 
			
			if(b[n<<1])
		}
	  
	  
	} while(done);
	procédure tri_bulle(tableau T, entier n)
     répéter
         u32 done = 0;
	 for(u32 n=0; n < nb-2; n++){
             if(h[j] > T[j + 1], alors
                 échanger T[j] et T[j + 1]
                 échange_effectué = vrai
     tant que échange_effectué

 * 
 * 
 * 
 * 



40 4k offset for <32
40 4k offset for 

*/




