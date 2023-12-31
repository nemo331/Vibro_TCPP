#define ARGUMENT(x) ((arg[x][0]=='/') || (arg[x][0]=='-'))
#define arg_error_return(x) {printf("\n\7%s\n",ArgErrMsg[x]); goto PrintFormat;}
{   char         c, m, k, recognized[10]={0,0,0,0,0,0,0,0,0,0};
    static char *ArgErrMsg[]={
                  "Too many arguments",
                  "Too long file name",
                  "Rate: invalid assignment",
                  "Number of Channels: invalid assignment",
                  "Data Volume: invalid switch",
                  "Unrecognized parameter",
                  "Invalid (too long or zero) data volume requested",
                  "Rate: invalid switch",
				  "You PC has no LA70 board"
                  };

printf(
"\n             浜様様様様様様様様様様様様様様様様様邑"
"\n             � PSV Tools 1.0, Chernyshyov V.Yu.   �"
"\n             把陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳調"
"\n             � LA70 Fast Disk Acquisition Utility �"
"\n             �  Version 1.08, PSV Tools, 1992-93  �"
"\n             喞陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳潰");
// 陳陳陳 �牀▲爲� �爍祠キ皰� 陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳陳�
   Nargs--;
   if(Nargs>5) arg_error_return(0);

   for(c=1; c<=Nargs; c++) for(m=0; m<strlen(arg[c]); m++)
   {
      k = arg[c][m];
      if( k>='a' && k<='z' ) arg[c][m] -= 0x20;  // upper case
   }

   // 様� /? 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様

      for(c=1; c<=Nargs; c++)
      {   if( ! ARGUMENT(c) ) continue;
         if( arg[c][1]=='?' )
         {
PrintFormat:
printf(
"\n��爼�� �襷���: la70_dsk [━� �����] [/��鈑1] [/��鈑2]..."
"\n��鈑�: Cxx  - ���ム� ������� (hex<10) ���爬�ム C01af, �� 祠������� C0"
"\n       Axxx - ��М�覃 �むメ LA70 (hex)"
"\n       R    - 瓷�牀痰� (皀��) ��牀�� (1):"
"\n              RFxxx - �♂�� ��痰��� ���竅�� [��],     �� 祠������� 10032 <�"
"\n              RIxxx - ┃皀燿�� �ウゃ ���竅���� [皋�], �� 祠������� 119   <�"
"\n              RCxxx - ��痰��� ���竅�� [��] � ��瘍モ� �� 1 �����."
"\n       V    - �♀ガ 甌；��ガ�� ┃筮爼�罔� (2):        �� 祠������� 1 ゛��"
"\n              VSyyy - � 瓮�祗���,    VNyyy - � �矚腑��� (�� 1 �����),"
"\n              VMyyy - � �┃竄��,     VByyy - � ゛���� (�� 64 KB),"
"\n              VHyyy - � �����."
"\n�｀� ����諷 ��Ε� °碎 �痰���←キ � �遏�� ���キ� ����皋ガ Ctrl-Break."
#if 0
"\n (1) - �爬 �����┬ 皀��� 腑爛� ┃皀燿�� �ウゃ ���竅���� (� 皋��� 瓱痰ガ����"
"\n       ����ム�) 甄イ礇� �����碎, 艪� ��痰��� ィ� ���竅�� 1193820 ��;"
"\n     - �����┘ 皀��� 腑爛� /R� 蹣※���キ皚� /RF, メ�� ��痰�矣 祠��Θ碎 ��"
"\n       腮甄� �������: la70_dsk /c012 /rc1000  <=> la70_dsk /c012 /rf3000."
"\n (2) - �爬 �♀ガ� > 1 ゛��� �� か� ≡ィ� 〓К�Ν��� え���М�� ��痰�� ���竅��"
"\n       �痺薀痰※� �ク爛琺↓覃 ��玻オ皚覃 瓠�� ┃筮爼�罔�: ���竅� � ������ � �"
"\n       瓮爛え�� ��Δ��� ゛��� 痰���※矚� �オ※皋牀����覓, 〓К�Ν� �牀�竅��"
"\n       �矚腑皰�. �� ├．���┘ �オ�玻オ皚��� 瓠��� 甄イ礇� ������腮��碎"
"\n       痺���爿竡 ��痰�矣 ���竅��: か� DOS 3.3 - 3 ���, か� DOS 5.0 - 1.2 ���."
#endif
);
return(1);
      }};
   // 様様━� �����様様様様様様様様様様様様様様様様様様様様様様様様様様様様

      if(Nargs)
      do{
         if( ARGUMENT(1) ) break;
         c=strlen(arg[1]);
	 if( c>=127 ) arg_error_return(1);
         recognized[1]=1;
         strcpy(res_name,arg[1]);
         break;
      }   while(1);

   // 様様様様様様様様様様様様様様様様� /A%x : ��М�覃 �むメ P&S board 様様

	{  int b; k=0;

      for(c=1; c<=Nargs; c++)
      {  if( ! ARGUMENT(c) ) continue;
         if(arg[c][1]=='A') { k=sscanf(arg[c]+2,"%3x", & b ); break;};
      };
		if( k )
		{
			outportb(b+5,13);       /* 竅����←│�� ����� */
			if( ( inportb(b+5)>>4 ) /* 痰�琥┤ ���磧��� ぎ�Ε� °碎 ��▲� */
						!= 13 )        /* 竅����←キ���� ���ム� ������       */
			{		Base_Address=0;
					printf("\7");
			}
			else Base_Address=b;
		}
      if(c<=Nargs) recognized[c]=1;
	}
   // 様様様様様様様様様様様様様様様様様様� /C%x%x... : ���ム� ������� 様様

      for(c=1; c<=Nargs; c++)
      {  if( ! ARGUMENT(c) ) continue;
         if(arg[c][1]=='C')
         {  Nchans = strlen(arg[c]) - 2;
				for(i=0; i<Nchans; i++)
				{
					char x = arg[c][i+2];
					if( x>='0' && x<='9') Channel[i]= x - '0';
					else
					if( x>='A' && x<='F') Channel[i]= x - 'A'+10;
					else {Nchans=0; break;}
				}
				if( Nchans>16 || !Nchans ) arg_error_return(3);
            break;
         };
      };
      if(c<=Nargs) recognized[c]=1;

   // 様様様様様様様様様様様様様様様様様様様様様� /F%l : ��痰��� □��� 様様
      rate = TIMER_FR/FREQ;

		for(c=1; c<=Nargs; c++)
      {  if( ! ARGUMENT(c) )  continue;
         if(arg[c][1] != 'R') continue;
         switch(arg[c][2]){
         case 'F': k=sscanf(arg[c]+3,"%f", & FREQ );
                   if( !k || FREQ==0.) arg_error_return(2);
                   rate = (long unsigned)floor(TIMER_FR/FREQ);
                   if( floor(TIMER_FR/FREQ) != rate ) arg_error_return(2); break;
         case 'C': k=sscanf(arg[c]+3,"%f", & FREQ );
						 FREQ *= Nchans;
                   if( !k || FREQ==0.) arg_error_return(2);
                   rate = (long unsigned)floor(TIMER_FR/FREQ);
                   if( floor(TIMER_FR/FREQ) != rate ) arg_error_return(2); break;
         case 'I': k=sscanf(arg[c]+3,"%lu", & rate );
                   FREQ = (long unsigned)(TIMER_FR/rate);
                   if( !k ) arg_error_return(2); break;
         default : arg_error_return(7);
         };
         break;
      }

      if(c<=Nargs) recognized[c]=1;
   // 様様様様様様様様様様様様様様様� /V - �♀ガ 甌；��ガ�� ┃筮爼�罔� 様様

   DIM = (BLOCK Kbytes) * (512L words each);
   SINGLE_BUF_SIZE =
         (BLOCK Kbytes) * (256 words each);      // DIM / 2

      for(c=1; c<=Nargs; c++)
      {  if( ! ARGUMENT(c) ) continue;
         if(arg[c][1]=='V')
         {  long unsigned numnum; float VOL;
            k=sscanf(arg[c]+3,"%lu", & numnum );
            if( !k ) arg_error_return(3);
            switch(arg[c][2])
            {
               case 'B': VOL =    1. * numnum;                    break;
               case 'N': VOL =    1. * numnum*Nchans / DIM;       break;
               case 'S': VOL =    1. * numnum        / DIM *FREQ; break;
               case 'M': VOL =   60. * numnum        / DIM *FREQ; break;
               case 'H': VOL = 3600. * numnum        / DIM *FREQ; break;
               //                     �per unit,per strobe�     �
               //                     青陳陳陳陳陳陳陳陳陳�     �
               //                     �per second               �
               //                     青陳陳陳陳陳陳陳陳陳陳陳陳�
               default : arg_error_return(4);
            }
			{
				numnum = (long unsigned)VOL;
				REM_SIZE = (short unsigned)((VOL-numnum)*DIM);
            	VOLUME = (int unsigned )numnum;
            	if(	(!VOLUME && !REM_SIZE) ||
					numnum != (long unsigned)VOLUME) arg_error_return(6);
			};
            break;
         };
      };
      if(c<=Nargs) recognized[c]=1;
   // 様様様様様様様様様様様様様様様様様様様様様様様様様様���甎�Л��┘様様�
      for(c=1; c<=Nargs; c++)
      if( !recognized[c] )
      { printf("\n*** %s ***",arg[c]); arg_error_return(5);};
   // 様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様�
		if(!Base_Address)	Base_Address = PS_available();
		if(!Base_Address) arg_error_return(8);
}
