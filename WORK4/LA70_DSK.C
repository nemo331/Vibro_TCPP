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
#define SECND_DONE     (SINGLE_BUF_SIZE & counter)  // counter � DIM/2
#define FIRST_not_DONE   SECND_DONE                 // counter � DIM/2
#define FIRST_DONE     (!SECND_DONE)                // counter < DIM/2
#define SECND_not_DONE (!SECND_DONE)                // counter < DIM/2

// ������������������������������������������������������������������������
// � Interrupt Handlers Support �������������������������������������������
// ������������������������������������������������������������������������
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
��������������������������������������������������������������
IRQ  XT             AT            IRQ  (2-nd 8259A)
��������������������������������������������������������������
  0  timer          timer        �  8  real time clock
  1  keyboard       keyboard     �  9  soft.redirected to IRQ2
  2  reserved    *  I/O channel � 10  reserved
  3  COM1        *  COM2         � 11  reserved
  4  COM2        *  COM1         � 12  reserved
  5  fixed disk  *  LPT2         � 13  x87 coprocessor
  6  diskette       diskette     � 14  fixed disk
  7  LPT1           LPT1         � 15  reserved
��������������������������������������������������������������
*/
#define WORKING_MASK  0xD8   /* ���������������������������������Ŀ
                              �  1101.1000 enables : KBD and      �
                              �  XT - Tmr & reserved & fixed disk �
                              �  AT - Tmr & I/O channel & LPT2    �
                              ����������������������������������� */

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
         Channel_new_number);     /* next Channel setting ࠧ�蠥� ���:   */\
                                  /* �� ���� �� �⮬� ������ �� �室�    */\
                                  /* ��� ᨣ��� ������ ��⠭�������, �⮡� */\
                                  /* �� �뫮 ��������쭮�� �஭������     */\
                                  /*                                    �  */\
   _AH = inpB(_DX) & 0x0f;        /* READ of previously                 �  */\
   _DX--;                         /* started digityzing                 �  */\
   _AL = inpB(_DX);               /*                                    �  */\
   *BUF_POINTER++ = _AX;          /* Bufferization                      �  */\
                                  /*                                    �  */\
   outB( _DX, _AL );              /* START �� ��⠭��������� ������ �����  */\
   counter --;                                                               \
																									  \
   old_index = new_index;                                                    \
   new_index = (new_index+1)%Nchans;                                         \
   Channel_new_number = Channel[new_index];                                  \
};                                          /* end if( !Tmissing )         */\
	Tmissing = (Tmissing+1)%TmissIni;                                         \
	}
// ������������������������������������������������������������������������
   void set_cur_lock(int,int,int);
   #define LINE      32
   #define CURPOS 			((unsigned int far *)MK_FP(0x40,0x50))
   #define TYPE_PROGRESS	if(!((++num)%piece)){putchar('�'); BarNum++;}
	#define SAVE_CURPOS		{cRow = (CURPOS[0]>>8); cCol = CURPOS[0]&0xff;}
	#define SET_CURPOS(C,R)	set_cur_loc(C,R,0)
// ������������������������������������������������������������������������
   char  OldMask1;
	unsigned int cRow, cCol;
   void  interrupt    (*Old_Timer_Intr)(void);   // BIOS timer
   void  interrupt    (*Old_Int15_Hndl)(void);   // Device Bisy Loop
   void  interrupt    (*Old_Int76_Hndl)(void);   // Hard Disk IRQ

// ������������������������������������������������������������������������
// � Internal prototypes ��������������������������������������������������
// ������������������������������������������������������������������������
   void *tcalloc(int elsize, int sizeof_el);
   void tfree( void *address);
// ������������������������������������������������������������������������
// � Error support ��������������������������������������������������������
// ������������������������������������������������������������������������
   #define  error_return(x) {                                            \
	SET_CURPOS(cCol,cRow);                                                \
            printf("\n\7�訡�� %s",ErrMsg[x]);                           \
            if(handler+1){                                               \
            chsize(handler, tell(handler));                              \
            close(handler);};                /* ������� 䠩�           */\
            if(buf0!=NULL)tfree(buf0);       /* �᢮������ ������      */\
            return(x);}
	#define  exit_if_error {                                              \
				if(ErrNum){ Disable_Acquisition(); error_return(ErrNum);};}

   char         *ErrMsg[]={
               "0 - �ணࠬ�� �����襭� �ᯥ譮",
               "1 - �� ���� ࠧ������ 䠩� ����襭���� ࠧ��� �� ��᪥",
               "2 - �� ���� ࠧ������ ���� ������ � �����",
               "3 - User's Ctrl-Break",
               "4 - ���ᯥ� ����� ᢮������� ����",
					"5 - ᫨誮� ��᮪�� ���� ����",
					"6 - ᫨誮� ������ ���� ����"
               },
               ErrNum=0;
// ������������������������������������������������������������������������
   unsigned char
         low_byte, hig_byte;
   int   low_port, hig_port;
// ������������������������������������������������������������������������
// � Arguments ������������������������������������������������������������
// ������������������������������������������������������������������������
   char unsigned  res_name[128]="ps.dat";
   float          FREQ=10000.;              // ���� ���஢��
   long unsigned  //FREQ=10000,               // ���� ���஢��
                  rate;                     // �����.��ਮ� ����᪠ � ⨪��
   int unsigned   Base_Address = 0,         // 0 ��� ����஫� ��㬥��
                  VOLUME       = 1 blocks,  // ��ꥬ � ������
				  REM_SIZE	   = 0,			// ���⮪ (��楫� ����) � ����.
                  SINGLE_BUF_SIZE  words ;  // ࠧ��� �����筮�� ����
   long unsigned  DIM              words ;  // ��騩 ࠧ��� �������� ����
   char unsigned  wait_flag=0; /* if(!VOLUME && RES_SIZE) ���� �� �ᯥ�� �⫮����
								  ���室 counter �१ RES_SIZE */

// ������������������������������������������������������������������������
   int unsigned
         far *BUF_POINTER,        // ��६���� 㪠��⥫� �Ŀ
         counter=0xffff,          // offset-���稪 ��� ������ 64 KB !!!
         Tcounter,                // counter ��� ��⥬���� ⠩���
			TmissIni=1,              // �ய�᪮� IRQ0 +1 (��� ������ ����)
			Tmissing,                // ���稪 �ய�᪮� IRQ0
         T2Delay,                 // ⥪�騩 �� ���-�� �⠭������ ��᫥����
         T2pCount,                // ��᫥���� ��� counter for Timer2
         T2Count,                 // ⥪�騩   �� (check missed Timer IRQ)
         Nchans=1,                // ������⢮ �������
         old_index = 0,           // ������ � ���ᨢ� ����஢ �������
         new_index = 0;           // ������ � ���ᨢ� ����஢ �������
   char  Channel_new_number,      // ���� ����� ������
         Channel[16];             // ���ᨢ ����஢ �������
// ������������������������������������������������������������������������
// � New Interrupt Handler Functions ��������������������������������������
// ������������������������������������������������������������������������
   char unsigned DeviceBisyLoop=0;
   int unsigned far *scr,inn=0,outt=0;
   int cbrk_hnd(void);

   void  interrupt   New_Int15_Hndl(void)
// ������������������������������������������������������������������������
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
   asm   jne EndIntr;                  // DBL ����� ���� ���饭 �� ⮣� !!!
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
// ������������������������������������������������������������������������
	char ncr=0;
   void  interrupt   New_Timer_Intr(void)
// ������������������������������������������������������������������������
{
/*	GET_T2_COUNT; GET_DELAY;
	STORE_T2_COUNT;
*/
	ADC_BODY;
   outB(0x20,0x20);
}
// End interrupt
// ������������������������������������������������������������������������
// ������������������������������������������������������������������������
// � Functions ������������������������������������������������������������
// ������������������������������������������������������������������������
   void near Disable_Acquisition(void)
// ������������������������������������������������������������������������
{
   disable();
   outB( MASK1, 0xFF );                // disable all IRQ from 8259
	outB( 0x20, 0xC7 );                 // restore default priorities: 7=less

   // ����⠭����� �०��� �����

   setvect( 0x08, Old_Timer_Intr);
   setvect( 0x15, Old_Int15_Hndl);  DeviceBisyLoop = 0;

   outB(0x43,0x36);                    // Timer0 <- 18.2 Hz
   outB(0x40,0xFF); outB(0x40,0xFF);

   outB( MASK1, OldMask1 );            // ॣ���� ��᪨ 8259
   outB(0x43,0x74);
   outB(0x41,( char) 18 );             // Timer1 (refresh) previous state
   outB(0x41,( char)  0 );

   enable();
}
// ������������������������������������������������������������������������
   int near Enable_Acquisition(void)
// ������������������������������������������������������������������������
{
   low_port = Base_Address+4;
   hig_port = Base_Address+5;

// ���� ����᪠ ⠩��� � ������⢮ �ய�᪠���� IRQ0

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

   // �।���⥫�� START �� ��ࢮ�� �� ��࠭��� �������:
   // �������� ��� �ࠢ��쭮�� ��᫥���饩 ࠡ��� New_Timer_Intr

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

   Old_Timer_Intr = getvect(0x08);     // ��࠭��� ���� Timer0 Handler
   setvect( 0x08, New_Timer_Intr);     // ��⠭����� ���� Timer0 Handler

   Old_Int15_Hndl = getvect(0x15);     // ��࠭��� ���� DBL/FNI
   setvect( 0x15, New_Int15_Hndl);     // ��⠭����� ���� DBL/FNI

//   outB( MASK1, WORKING_MASK);         // 1111.1010 enables only Tmr & 2nd 8259
   enable();
	return 0;
}
// ������������������������������������������������������������������������
   int near PS_available(void)
// ������������������������������������������������������������������������
{  int i;
   int bases[8]={0x200,0x210,0x220,0x230,0x300,0x310,0x320,0x330};
   for(i=0; i<8; i++)
   {
      outportb(bases[i]+5,13);       // ��⠭������� �����
      if( ( inportb(bases[i]+5)>>4 ) // ���訩 ���㡠�� ������ ���� ࠢ��
               != 13 )               // ��⠭��������� ������ ������
      continue;
      return(bases[i]);
   }
   return(0);                        // PS not available
}
// ������������������������������������������������������������������������
   int unsigned far *buf0=NULL, far* not_written=0;
   int handler=-1;        			// data output file
   int main(int Nargs, char *arg[])
// ������������������������������������������������������������������������
{
   int unsigned
            far *buf1=NULL,       // ���ᨢ ᮡ�ࠥ��� ������
            far *buf2=NULL,
            far *buf11=NULL,
            far *buf21=NULL;
   int      i;

   scr = MK_FP(0xb800,0);
   Channel[0]=0;
   #include "ch_psdsk.c"

// ������������������������������������������������������������������������
   buf0 = tcalloc(SINGLE_BUF_SIZE items, 4 bytes each);// DIM �� 2 = 64 KB !!!
   if(buf0==NULL) error_return(2);
   buf1 = buf0;                                       // preserve for tfree
// ������������������������������������������������������������������������
   buf2 = buf1 + SINGLE_BUF_SIZE words;     // offs = 32 KB
   buf11 = buf1 + SINGLE_BUF_SIZE/2 words;  // offs = 16 KB
   buf21 = buf2 + SINGLE_BUF_SIZE/2 words;  // offs = 48 KB
   BUF_POINTER = buf1;
// ������������������������������������������������������������������������
   handler =
   _creat(res_name,FA_ARCH);      // create new / overwrite existing one

// ��१�ࢨ஢��� ���� �� ��᪥
   printf("\rWait...");
   if(
      chsize(handler, (VOLUME blocks) * (2L*DIM bytes each) + REM_SIZE*2L)
     ) error_return(1);

   lseek(handler,0L,SEEK_SET);    // 㪠��⥫� 䠩�� - �� ��砫�

   { float persec;
	{  long unsigned T,TI=1; float rrate;
		for(T=rate; T>=0x4000L; TI++) T = rate/TI;
		rrate=(float)T*TI;
        FREQ = TIMER_FR/rrate; // really loaded freq
	}
   persec = (float)FREQ/512;
   printf(
     "\r             ������������������������������������͵"
     "\n             �  ������ ����    %x         	  �"
	 "\n             �  ��ꥬ ������     %u ������  	  �"
	 "\n             �  ������ �����     %u KB      	  �"
	 "\n             �  ������ �६�     %.1f ᥪ㭤	  �"
	 ,
	Base_Address,
   VOLUME,BLOCK,(float)(VOLUME*BLOCK*1.024+(float)REM_SIZE/512)/persec);
   printf(
	 "\n             �  ����� ����   %.*f ��     	  �"
	 ,
   (FREQ<.01)?3:(FREQ<.1)?2:(FREQ<1.)?1:0, FREQ);
   printf(
	 "\n             �  ��᫮ �������    %i         	  �"
	 "\n             �  ��⮪ ������     %.1f KB/ᥪ	  �"
     "\n             ������������������������������������Ĵ"
	 "\n             �                                    �"
	 "\n             ��������������������������������������"
	 "\n\nPress any key to start\7",
   Nchans,persec);
   getch();
	printf("\r                      ");
   }
// ������������������������������������������������������������������������
   {  int unsigned
			piece, num=0, BarNum=0, line=LINE;

      char far *PAGE0;

   piece = (VOLUME*4)/LINE; if(!piece) line=VOLUME*4;
   piece += 1;

	SAVE_CURPOS;
	SET_CURPOS(cCol-6,cRow-3);
   for(i=1; i<=line; i++) putchar('�');
	SET_CURPOS(cCol-6,cRow-3);
// ������������������������������������������������������������������������
   if(Enable_Acquisition()) error_return(ErrNum);     // REAL START
   not_written=buf1;
   ctrlbrk(cbrk_hnd);	// only AFTER Enable_Acq
 if (VOLUME) {
   outB( MASK1, WORKING_MASK);         // 1111.1010 enables only Tmr & 2nd 8259
   while(1)
   {
   // ���������������������������������������������������������������������
      TYPE_PROGRESS;
      while(FIRST_not_DONE)
	  {	asm HLT; scr[3]++;   // ��� ���������� ��ࢮ�� ����
		if(kbhit()) getch();
	  }

      outB( MASK1, WORKING_MASK|2); // disable CBRK while writing�����Ŀ
      _write( handler, buf1, SINGLE_BUF_SIZE bytes);                 //�
                                                                     //�
      exit_if_error;                                                 //�
      TYPE_PROGRESS;                                                 //�
      if(SECND_DONE) ErrNum=4;              // ᭮�� �������� FIRST   �
      exit_if_error;                                                 //�
                                                                     //�
      _write( handler, buf11, SINGLE_BUF_SIZE bytes);                //�
      not_written = buf2;          // BUF_PTR > buf2                   �
      outB( MASK1, WORKING_MASK);  // enable CBRK after writing���������
      exit_if_error;
   // ���������������������������������������������������������������������
      TYPE_PROGRESS;
      while(SECND_not_DONE) {asm HLT; scr[4]++;  // ��� ���������� ��ண� ����
		if(kbhit()) getch();
	  }

      outB( MASK1, WORKING_MASK|2); // disable CBRK while writing�����Ŀ
      _write( handler, buf2, SINGLE_BUF_SIZE bytes);                 //�
      exit_if_error;                                                 //�
      TYPE_PROGRESS;                                                 //�
      if(FIRST_DONE) ErrNum=4;              // ᭮�� �������� SECND   �
      exit_if_error;                                                 //�
                                                                     //�
      _write( handler, buf21, SINGLE_BUF_SIZE bytes);                //�
      not_written = buf1;                                            //�
      outB( MASK1, WORKING_MASK);  // enable CBRK after writing���������
      exit_if_error;
   // ���������������������������������������������������������������������
      VOLUME--;
      if(!VOLUME) break;
   }
 } else wait_flag=1;

   if (REM_SIZE)
   {
	  short unsigned cmp=0xffff-REM_SIZE;
   // ���������������������������������������������������������������������
      TYPE_PROGRESS;
	  if (wait_flag) outB( MASK1, WORKING_MASK);
      while(counter>=cmp) {asm HLT; scr[3]++;  // ��� ���������� ���⪠ ����
		if(kbhit()) getch();
	  }
      _write( handler, buf1, REM_SIZE*2U bytes);
   // ���������������������������������������������������������������������
   }
   for(; BarNum<line; BarNum++) putchar('�');
   }
   if(kbhit()) if(!getch()) getch();
// ������������������������������������������������������������������������
// ������������������������������������������������������������������������
   Disable_Acquisition();
   error_return(ErrNum);
}


   int cbrk_hnd()
// ������������������������������������������������������������������������
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
// ������������������������������������������������������������������������
