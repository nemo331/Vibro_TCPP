   #include   <math.h>
   #include   <dos.h>
   #include   <io.h>
   #include   <stdio.h>
   #include   <conio.h>
//   #include   <stdlib.h>
   #include   <string.h>

#define outB      outportb
#define outW      outport
#define inpB      inportb
#define inpW      inport

#define words
#define bytes
#define Kbytes
#define blocks
#define items
#define each

#define PORTION         0x1000 bytes
#define SECND_DONE     (SINGLE_BUF_SIZE & counter)  // counter ò DIM/2
#define FIRST_not_DONE   SECND_DONE                 // counter ò DIM/2
#define FIRST_DONE     (!SECND_DONE)                // counter < DIM/2
#define SECND_not_DONE (!SECND_DONE)                // counter < DIM/2

// ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ
// Û Interrupt Handlers Support ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ
// ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
#define TIMER_FR      1193820L
#define GET_T2_COUNT  asm{ push ax;                                       \
                           mov al,80h; out 43h,al; nop;nop;nop;           \
                           in al,42h;  mov ah,al;  nop;nop;nop;           \
									in al,42h;  xchg ah,al;                        \
									mov T2Count,ax; pop ax; }
#define STORE_T2_COUNT T2pCount = T2Count
#define GET_DELAY      T2Delay = T2pCount - T2Count
#define IRQ0_MISS_NUM (T2Delay/Tcounter)

#define MASK1         0x21
#define BLOCK         64 Kbytes
/*
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
IRQ  XT             AT            IRQ  (2-nd 8259A)
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
  0  timer          timer        Ú  8  real time clock
  1  keyboard       keyboard     ³  9  soft.redirected to IRQ2
  2  reserved    *  I/O channel µ 10  reserved
  3  COM1        *  COM2         ³ 11  reserved
  4  COM2        *  COM1         ³ 12  reserved
  5  fixed disk  *  LPT2         ³ 13  x87 coprocessor
  6  diskette       diskette     ³ 14  fixed disk
  7  LPT1           LPT1         À 15  reserved
ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
*/
#define WORKING_MASK  0xD8   /* ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ¿
                              ³  1101.1000 enables : KBD and      ³
                              ³  XT - Tmr & reserved & fixed disk ³
                              ³  AT - Tmr & I/O channel & LPT2    ³
                              ÀÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ */

/*	_BX=(int)IRQ0_MISS_NUM;       \
	_CX=_BX;                      \
	for(; _CX>2; _CX--)           \
	{                             \
		*BUF_POINTER++=5000;       \
		counter--;                 \
	}                             \
*/
#define ADC_BODY {                if( !Tmissing )  \
{                                                  \
	_DX=hig_port;                                                             \
	outB( _DX,                                                                \
         Channel_new_number);     /* next Channel setting à §à¥è ¥â ‚•:   */\
                                  /* ¤® áâ àâ  ¯® íâ®¬ã ª ­ «ã ­  ¢å®¤¥    */\
                                  /* ‚• á¨£­ « ¤®«¦¥­ ãáâ ­®¢¨âìáï, çâ®¡ë */\
                                  /* ­¥ ¡ë«® ¬¥¦ª ­ «ì­®£® ¯à®­¨ª ­¨ï     */\
                                  /*                                    ³  */\
   _AH = inpB(_DX) & 0x0f;        /* READ of previously                 ³  */\
   _DX--;                         /* started digityzing                 ³  */\
   _AL = inpB(_DX);               /*                                    ³  */\
   *BUF_POINTER++ = _AX;          /* Bufferization                      ³  */\
                                  /*                                    ³  */\
   outB( _DX, _AL );              /* START ¯® ãáâ ­®¢«¥­­®¬ã ª ­ «ã ÄÄÄÄÙ  */\
   counter --;                                                               \
																									  \
   old_index = new_index;                                                    \
   new_index = (new_index+1)%Nchans;                                         \
   Channel_new_number = Channel[new_index];                                  \
};                                          /* end if( !Tmissing )         */\
	Tmissing = (Tmissing+1)%TmissIni;                                         \
	}
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
   void set_cur_lock(int,int,int);
   #define LINE      32
   #define CURPOS 			((unsigned int far *)MK_FP(0x40,0x50))
   #define TYPE_PROGRESS	if(!((++num)%piece)){putchar('Û'); BarNum++;}
	#define SAVE_CURPOS		{cRow = (CURPOS[0]>>8); cCol = CURPOS[0]&0xff;}
	#define SET_CURPOS(C,R)	set_cur_loc(C,R,0)
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
   char  OldMask1;
	unsigned int cRow, cCol;
   void  interrupt    (*Old_Timer_Intr)(void);   // BIOS timer
   void  interrupt    (*Old_Int15_Hndl)(void);   // Device Bisy Loop
   void  interrupt    (*Old_Int76_Hndl)(void);   // Hard Disk IRQ

// ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ
// Û Internal prototypes ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ
// ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
   void *tcalloc(int elsize, int sizeof_el);
   void tfree( void *address);
// ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ
// Û Error support ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ
// ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
   #define  error_return(x) {                                            \
	SET_CURPOS(cCol,cRow);                                                \
            printf("\n\7è¨¡ª  %s",ErrMsg[x]);                           \
            if(handler+1){                                               \
            chsize(handler, tell(handler));                              \
            close(handler);};                /* § ªàëâì ä ©«           */\
            if(buf0!=NULL)tfree(buf0);       /* ®á¢®¡®¤¨âì ¯ ¬ïâì      */\
            return(x);}
	#define  exit_if_error {                                              \
				if(ErrNum){ Disable_Acquisition(); error_return(ErrNum);};}

   char         *ErrMsg[]={
               "0 - ¯à®£à ¬¬  § ¢¥àè¥­  ãá¯¥è­®",
               "1 - ­¥ ¬®£ã à §¬¥áâ¨âì ä ©« § ¯à®è¥­­®£® à §¬¥à  ­  ¤¨áª¥",
               "2 - ­¥ ¬®£ã à §¬¥áâ¨âì ¡ãä¥à ¤ ­­ëå ¢ ¯ ¬ïâ¨",
               "3 - User's Ctrl-Break",
               "4 - ­¥ãá¯¥¢ § ¯¨á¨ á¢®¡®¤­®£® ¡ãä¥à ",
					"5 - á«¨èª®¬ ¢ëá®ª ï ç áâ®â  ®¯à®á ",
					"6 - á«¨èª®¬ ­¨§ª ï ç áâ®â  ®¯à®á "
               },
               ErrNum=0;
// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ
   unsigned char
         low_byte, hig_byte;
   int   low_port, hig_port;
// ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ
// Û Arguments ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ
// ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
   char unsigned  res_name[128]="ps.dat";
   float          FREQ=10000.;              // ç áâ®â  ®æ¨äà®¢ª¨
   long unsigned  //FREQ=10000,               // ç áâ®â  ®æ¨äà®¢ª¨
                  rate;                     // ­¥®¡å.¯¥à¨®¤ § ¯ãáª  ¢ â¨ª å
   int unsigned   Base_Address = 0,         // 0 ¤«ï ª®­âà®«ï  à£ã¬¥­â 
                  VOLUME       = 1 blocks,  // ®¡ê¥¬ ¢ ¡«®ª å
				  REM_SIZE	   = 0,			// ®áâ â®ª (­¥æ¥«ë© ¡«®ª) ¢ ®âáç.
                  SINGLE_BUF_SIZE  words ;  // à §¬¥à ®¤¨­®ç­®£® ¡ãä¥à 
   long unsigned  DIM              words ;  // ®¡é¨© à §¬¥à ¤¢®©­®£® ¡ãä¥à 
   char unsigned  wait_flag=0; /* if(!VOLUME && RES_SIZE) ¬®£ã ­¥ ãá¯¥âì ®â«®¢¨âì
								  ¯¥à¥å®¤ counter ç¥à¥§ RES_SIZE */

// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ
   int unsigned
         far *BUF_POINTER,        // ¯¥à¥¬¥­­ë© ãª § â¥«ì ÄÄ¿
         counter=0xffff,          // offset-áç¥âç¨ª ¤«ï ÄÄÄÄÄÙ 64 KB !!!
         Tcounter,                // counter ¤«ï á¨áâ¥¬­®£® â ©¬¥à 
			TmissIni=1,              // ¯à®¯ãáª®¢ IRQ0 +1 (¤«ï ­¨§ª¨å ç áâ®â)
			Tmissing,                // áç¥âç¨ª ¯à®¯ãáª®¢ IRQ0
         T2Delay,                 // â¥ªãé¨© ­¥ ®¡ï§-­® áâ ­®¢¨âáï ¯®á«¥¤­¨¬
         T2pCount,                // ¯®á«¥¤­¨© ÄÂÄ counter for Timer2
         T2Count,                 // â¥ªãé¨©   ÄÙ (check missed Timer IRQ)
         Nchans=1,                // ª®«¨ç¥áâ¢® ª ­ «®¢
         old_index = 0,           // ¨­¤¥ªá ¢ ¬ áá¨¢¥ ­®¬¥à®¢ ª ­ «®¢
         new_index = 0;           // ¨­¤¥ªá ¢ ¬ áá¨¢¥ ­®¬¥à®¢ ª ­ «®¢
   char  Channel_new_number,      // ­®¢ë© ­®¬¥à ª ­ « 
         Channel[16];             // ¬ áá¨¢ ­®¬¥à®¢ ª ­ «®¢
// ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ
// Û New Interrupt Handler Functions ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ
// ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
   char unsigned DeviceBisyLoop=0;
   int unsigned far *scr,inn=0,outt=0;
   int cbrk_hnd(void);

   void  interrupt   New_Int15_Hndl(void)
// ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
{
   asm{  cmp ah,90h; je  DBL;
         cmp ah,91h; je  FNI;
         jmp OtherFun; }
DBL:                                   // Device Bisy Loop
/*
	GET_T2_COUNT; GET_DELAY;
	if((int)IRQ0_MISS_NUM>0 )
	{
		*BUF_POINTER++=5000; counter--;
//		T2Delay-=Tcounter;
//		STORE_T2_COUNT;
//		ADC_BODY;
	}
//	scr[800+(inn++)]=0x7020+IRQ0_MISS_NUM;
*/
   DeviceBisyLoop++;
   asm   cmp DeviceBisyLoop,1;         // can be modified only by Int15 FNI
   asm   jne EndIntr;                  // DBL ¬®¦¥â ¡ëâì ®¯ãé¥­ „ â®£® !!!
DBL1:
   scr[0]++;
   asm   sti;                          // wait hardware interrupt
	asm   hlt;
	asm   cli;
   asm   cmp DeviceBisyLoop,1;         // can be modified only by Int15 FNI
   asm   je  DBL1;                     // to preserve from CLI
   asm   jmp EndIntr;
FNI:                                   // Finish Interrupt
   scr[81]++;
/*
	GET_T2_COUNT; GET_DELAY;
	if((int)IRQ0_MISS_NUM>0 )
	{
		*BUF_POINTER++=6000; counter--;
//		STORE_T2_COUNT;
//		T2Delay-=Tcounter;
//		ADC_BODY;
	}
//	scr[960+(outt++)]=0x7020+IRQ0_MISS_NUM;
*/
   DeviceBisyLoop--;
   asm   jmp EndIntr;
OtherFun:                              // Other Functions
EndIntr:                               // End Interrupt
	;
}
// ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
	char ncr=0;
   void  interrupt   New_Timer_Intr(void)
// ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
{
/*	GET_T2_COUNT; GET_DELAY;
	STORE_T2_COUNT;
*/
	ADC_BODY;
   outB(0x20,0x20);
}
// End interrupt
// ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
// ÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜÜ
// Û Functions ÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛÛ
// ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
   void near Disable_Acquisition(void)
// ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
{
   disable();
   outB( MASK1, 0xFF );                // disable all IRQ from 8259
	outB( 0x20, 0xC7 );                 // restore default priorities: 7=less

   // ¢®ááâ ­®¢¨âì ¯à¥¦­¨¥ ¢¥ªâ®à 

   setvect( 0x08, Old_Timer_Intr);
   setvect( 0x15, Old_Int15_Hndl);  DeviceBisyLoop = 0;

   outB(0x43,0x36);                    // Timer0 <- 18.2 Hz
   outB(0x40,0xFF); outB(0x40,0xFF);

   outB( MASK1, OldMask1 );            // à¥£¨áâà ¬ áª¨ 8259
   outB(0x43,0x74);
   outB(0x41,( char) 18 );             // Timer1 (refresh) previous state
   outB(0x41,( char)  0 );

   enable();
}
// ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
   int near Enable_Acquisition(void)
// ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
{
   low_port = Base_Address+4;
   hig_port = Base_Address+5;

// ç áâ®â  § ¯ãáª  â ©¬¥à  ¨ ª®«¨ç¥áâ¢® ¯à®¯ãáª ¥¬ëå IRQ0

	{  long unsigned T;
		for(T=rate; T>=0x4000L; TmissIni++) T = rate/TmissIni;
 		Tcounter = (int unsigned) T;
	}

	if(Tcounter<40)    {ErrNum=5; return 1;};     // ??? FREQ > 30 kHz

   disable();

   // Save IRQ Mask

   OldMask1 = inpB( MASK1 );           // preserve 1st 8259 mask
   outB( MASK1, 0xFF );                // disable all IRQ from 8259
	outB( 0x20, 0xC0 );                 // to make HD more sign.: IRQ0=less

   // Timer Settings

   if(Tcounter<4096)
   {                                   // Timer1 (refresh) syncronization
         outB(0x43,0x74);
         outB(0x41,( char) (Tcounter & 0xff) );
         outB(0x41,( char)((Tcounter & 0xff00) >> 8));
   }

   outB(0x43,0x36);                    // New Timer0 counter = rate/TmissIni
   outB(0x40,(unsigned char)Tcounter);
   outB(0x40,(unsigned char)(Tcounter>>8));

   // ¯à¥¤¢ à¨â¥«ì­ë© START ¯® ¯¥à¢®¬ã ¨§ ¢ë¡à ­­ëå ª ­ «®¢:
   // ¤¥« ¥âáï ¤«ï ¯à ¢¨«ì­®áâ¨ ¯®á«¥¤ãîé¥© à ¡®âë New_Timer_Intr

   new_index = 0;
   old_index = new_index;
   Channel_new_number=Channel[new_index];
	Tmissing = 0;
	ADC_BODY; BUF_POINTER--; counter++;

   outB(0x43,0xB6);                    // Timer2 <- 18.2 Hz
   outB(0x42,0xFF); outB(0x42,0xFF);
	outB(0x61,(char)(~2)&(1|inpB(0x61)));  // open GATE, disable SOUND

	GET_T2_COUNT;
	STORE_T2_COUNT;

   // Hooked Vectors Setting

   Old_Timer_Intr = getvect(0x08);     // á®åà ­¨âì áâ àë© Timer0 Handler
   setvect( 0x08, New_Timer_Intr);     // ãáâ ­®¢¨âì ­®¢ë© Timer0 Handler

   Old_Int15_Hndl = getvect(0x15);     // á®åà ­¨âì áâ àë© DBL/FNI
   setvect( 0x15, New_Int15_Hndl);     // ãáâ ­®¢¨âì ­®¢ë© DBL/FNI

//   outB( MASK1, WORKING_MASK);         // 1111.1010 enables only Tmr & 2nd 8259
   enable();
	return 0;
}
// ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
   int near PS_available(void)
// ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
{  int i;
   int bases[8]={0x200,0x210,0x220,0x230,0x300,0x310,0x320,0x330};
   for(i=0; i<8; i++)
   {
      outportb(bases[i]+5,13);       // ãáâ ­ ¢«¨¢ î ª ­ «
      if( ( inportb(bases[i]+5)>>4 ) // áâ àè¨© ¯®«ã¡ ©â ¤®«¦¥­ ¡ëâì à ¢¥­
               != 13 )               // ãáâ ­®¢«¥­­®¬ã ­®¬¥àã ª ­ « 
      continue;
      return(bases[i]);
   }
   return(0);                        // PS not available
}
// ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
   int unsigned far *buf0=NULL, far* not_written=0;
   int handler=-1;        			// data output file
   int main(int Nargs, char *arg[])
// ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
{
   int unsigned
            far *buf1=NULL,       // ¬ áá¨¢ á®¡¨à ¥¬ëå ¤ ­­ëå
            far *buf2=NULL,
            far *buf11=NULL,
            far *buf21=NULL;
   int      i;

   scr = MK_FP(0xb800,0);
   Channel[0]=0;
   #include "ch_psdsk.c"

// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
   buf0 = tcalloc(SINGLE_BUF_SIZE items, 4 bytes each);// DIM ¯® 2 = 64 KB !!!
   if(buf0==NULL) error_return(2);
   buf1 = buf0;                                       // preserve for tfree
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
   buf2 = buf1 + SINGLE_BUF_SIZE words;     // offs = 32 KB
   buf11 = buf1 + SINGLE_BUF_SIZE/2 words;  // offs = 16 KB
   buf21 = buf2 + SINGLE_BUF_SIZE/2 words;  // offs = 48 KB
   BUF_POINTER = buf1;
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
   handler =
   _creat(res_name,FA_ARCH);      // create new / overwrite existing one

// § à¥§¥à¢¨à®¢ âì ¬¥áâ® ­  ¤¨áª¥
   printf("\rWait...");
   if(
      chsize(handler, (VOLUME blocks) * (2L*DIM bytes each) + REM_SIZE*2L)
     ) error_return(1);

   lseek(handler,0L,SEEK_SET);    // ãª § â¥«ì ä ©«  - ­  ­ ç «®

   { float persec;
	{  long unsigned T,TI=1; float rrate;
		for(T=rate; T>=0x4000L; TI++) T = rate/TI;
		rrate=(float)T*TI;
        FREQ = TIMER_FR/rrate; // really loaded freq
	}
   persec = (float)FREQ/512;
   printf(
     "\r             ÌÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍµ"
     "\n             º   §®¢ë©  ¤à¥á    %x         	  ³"
	 "\n             º  ¡ê¥¬ ¤ ­­ëå     %u ¡«®ª®¢  	  ³"
	 "\n             º   §¬¥à ¡«®ª      %u KB      	  ³"
	 "\n             º  ®«­®¥ ¢à¥¬ï     %.1f á¥ªã­¤	  ³"
	 ,
	Base_Address,
   VOLUME,BLOCK,(float)(VOLUME*BLOCK*1.024+(float)REM_SIZE/512)/persec);
   printf(
	 "\n             º  — áâ®â  ®¯à®á    %.*f ƒæ     	  ³"
	 ,
   (FREQ<.01)?3:(FREQ<.1)?2:(FREQ<1.)?1:0, FREQ);
   printf(
	 "\n             º  —¨á«® ª ­ «®¢    %i         	  ³"
	 "\n             º  ®â®ª ¤ ­­ëå     %.1f KB/á¥ª	  ³"
     "\n             ÇÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ´"
	 "\n             º                                    ³"
	 "\n             ÓÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÙ"
	 "\n\nPress any key to start\7",
   Nchans,persec);
   getch();
	printf("\r                      ");
   }
// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ
   {  int unsigned
			piece, num=0, BarNum=0, line=LINE;

      char far *PAGE0;

   piece = (VOLUME*4)/LINE; if(!piece) line=VOLUME*4;
   piece += 1;

	SAVE_CURPOS;
	SET_CURPOS(cCol-6,cRow-3);
   for(i=1; i<=line; i++) putchar('±');
	SET_CURPOS(cCol-6,cRow-3);
// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ
   if(Enable_Acquisition()) error_return(ErrNum);     // REAL START
   not_written=buf1;
   ctrlbrk(cbrk_hnd);	// only AFTER Enable_Acq
 if (VOLUME) {
   outB( MASK1, WORKING_MASK);         // 1111.1010 enables only Tmr & 2nd 8259
   while(1)
   {
   // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
      TYPE_PROGRESS;
      while(FIRST_not_DONE)
	  {	asm HLT; scr[3]++;   // ¦¤ã § ¯®«­¥­¨ï ¯¥à¢®£® ¡ãä¥à 
		if(kbhit()) getch();
	  }

      outB( MASK1, WORKING_MASK|2); // disable CBRK while writingÄÄÄÄÄÄ¿
      _write( handler, buf1, SINGLE_BUF_SIZE bytes);                 //³
                                                                     //³
      exit_if_error;                                                 //³
      TYPE_PROGRESS;                                                 //³
      if(SECND_DONE) ErrNum=4;              // á­®¢  § ¯®«­ï¥â FIRST   ³
      exit_if_error;                                                 //³
                                                                     //³
      _write( handler, buf11, SINGLE_BUF_SIZE bytes);                //³
      not_written = buf2;          // BUF_PTR > buf2                   ³
      outB( MASK1, WORKING_MASK);  // enable CBRK after writingÄÄÄÄÄÄÄÄÙ
      exit_if_error;
   // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
      TYPE_PROGRESS;
      while(SECND_not_DONE) {asm HLT; scr[4]++;  // ¦¤ã § ¯®«­¥­¨ï ¢â®à®£® ¡ãä¥à 
		if(kbhit()) getch();
	  }

      outB( MASK1, WORKING_MASK|2); // disable CBRK while writingÄÄÄÄÄÄ¿
      _write( handler, buf2, SINGLE_BUF_SIZE bytes);                 //³
      exit_if_error;                                                 //³
      TYPE_PROGRESS;                                                 //³
      if(FIRST_DONE) ErrNum=4;              // á­®¢  § ¯®«­ï¥â SECND   ³
      exit_if_error;                                                 //³
                                                                     //³
      _write( handler, buf21, SINGLE_BUF_SIZE bytes);                //³
      not_written = buf1;                                            //³
      outB( MASK1, WORKING_MASK);  // enable CBRK after writingÄÄÄÄÄÄÄÄÙ
      exit_if_error;
   // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
      VOLUME--;
      if(!VOLUME) break;
   }
 } else wait_flag=1;

   if (REM_SIZE)
   {
	  short unsigned cmp=0xffff-REM_SIZE;
   // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
      TYPE_PROGRESS;
	  if (wait_flag) outB( MASK1, WORKING_MASK);
      while(counter>=cmp) {asm HLT; scr[3]++;  // ¦¤ã § ¯®«­¥­¨ï ®áâ âª  ¡ãä¥à 
		if(kbhit()) getch();
	  }
      _write( handler, buf1, REM_SIZE*2U bytes);
   // ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
   }
   for(; BarNum<line; BarNum++) putchar('Û');
   }
   if(kbhit()) if(!getch()) getch();
// ÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍÍ
// ÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄÄ
   Disable_Acquisition();
   error_return(ErrNum);
}


   int cbrk_hnd()
// ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
   {
    Disable_Acquisition();
	SET_CURPOS(cCol,cRow);
    printf("\n\7User't Ctrl-Break");
    if(handler+1){
	   if (BUF_POINTER>not_written)
      _write( handler, not_written, (BUF_POINTER-not_written)*2U bytes);
    chsize(handler, tell(handler));
    close(handler);};
    if(buf0!=NULL)tfree(buf0);
    return(0);
   }
// ßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßßß
