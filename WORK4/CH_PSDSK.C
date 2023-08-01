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
"\n             ������������������������������������͸"
"\n             � PSV Tools 1.0, Chernyshyov V.Yu.   �"
"\n             ������������������������������������Ĵ"
"\n             � LA70 Fast Disk Acquisition Utility �"
"\n             �  Version 1.08, PSV Tools, 1992-93  �"
"\n             ��������������������������������������");
// ������ �஢�ઠ ��㬥�⮢ ���������������������������������������������
   Nargs--;
   if(Nargs>5) arg_error_return(0);

   for(c=1; c<=Nargs; c++) for(m=0; m<strlen(arg[c]); m++)
   {
      k = arg[c][m];
      if( k>='a' && k<='z' ) arg[c][m] -= 0x20;  // upper case
   }

   // ��� /? ��������������������������������������������������������������

      for(c=1; c<=Nargs; c++)
      {   if( ! ARGUMENT(c) ) continue;
         if( arg[c][1]=='?' )
         {
PrintFormat:
printf(
"\n��ଠ� �맮��: la70_dsk [��� 䠩��] [/����1] [/����2]..."
"\n����: Cxx  - ����� ������� (hex<10) ���ਬ�� C01af, �� 㬮�砭�� C0"
"\n       Axxx - ������ ���� LA70 (hex)"
"\n       R    - ᪮���� (⥬�) ���� (1):"
"\n              RFxxx - ���� ���� ����᪠ [��],     �� 㬮�砭�� 10032 <�"
"\n              RIxxx - ���ࢠ� ����� ����᪠�� [⨪], �� 㬮�砭�� 119   <�"
"\n              RCxxx - ���� ����᪠ [��] � ���� �� 1 �����."
"\n       V    - ��ꥬ ᮡ�ࠥ��� ���ଠ樨 (2):        �� 㬮�砭�� 1 ����"
"\n              VSyyy - � ᥪ㭤��,    VNyyy - � ������ (�� 1 �����),"
"\n              VMyyy - � ������,     VByyy - � ������ (�� 64 KB),"
"\n              VHyyy - � ���."
"\n���� ������ ����� ���� ��⠭����� � �� ������ ����⨥� Ctrl-Break."
#if 0
"\n (1) - �� ������� ⥬�� �१ ���ࢠ� ����� ����᪠�� (� ⨪�� ��⥬����"
"\n       ⠩���) ᫥��� �������, �� ���� ��� ����᪠ 1193820 ��;"
"\n     - ������� ⥬�� �१ /R� ���������⭮ /RF, �᫨ ����� 㬭����� ��"
"\n       �᫮ �������: la70_dsk /c012 /rc1000  <=> la70_dsk /c012 /rf3000."
"\n (2) - �� ��ꥬ� > 1 ����� �� ��� �ᥣ� ���������� ��������� ���� ����᪠"
"\n       �����⢨� �����뢭� ���४�� ᡮ� ���ଠ樨: ����� � ��砫� � �"
"\n       �।��� ������� ����� �⠭������ ������஢����, �������� �ய�᪨"
"\n       ����⮢. �� ��������� �����४⭮�� ᡮ� ᫥��� ��࠭�稢���"
"\n       �㬬���� ����� ����᪠: ��� DOS 3.3 - 3 ���, ��� DOS 5.0 - 1.2 ���."
#endif
);
return(1);
      }};
   // ���ͨ�� 䠩����������������������������������������������������������

      if(Nargs)
      do{
         if( ARGUMENT(1) ) break;
         c=strlen(arg[1]);
	 if( c>=127 ) arg_error_return(1);
         recognized[1]=1;
         strcpy(res_name,arg[1]);
         break;
      }   while(1);

   // ��������������������������������� /A%x : ������ ���� P&S board ����

	{  int b; k=0;

      for(c=1; c<=Nargs; c++)
      {  if( ! ARGUMENT(c) ) continue;
         if(arg[c][1]=='A') { k=sscanf(arg[c]+2,"%3x", & b ); break;};
      };
		if( k )
		{
			outportb(b+5,13);       /* ��⠭������� ����� */
			if( ( inportb(b+5)>>4 ) /* ���訩 ���㡠�� ������ ���� ࠢ�� */
						!= 13 )        /* ��⠭��������� ������ ������       */
			{		Base_Address=0;
					printf("\7");
			}
			else Base_Address=b;
		}
      if(c<=Nargs) recognized[c]=1;
	}
   // ������������������������������������� /C%x%x... : ����� ������� ����

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

   // ������������������������������������������� /F%l : ���� ����� ����
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
   // ������������������������������� /V - ��ꥬ ᮡ�ࠥ��� ���ଠ樨 ����

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
               //                     ���������������������     �
               //                     �per second               �
               //                     ���������������������������
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
   // ����������������������������������������������������͐�ᯮ�����������
      for(c=1; c<=Nargs; c++)
      if( !recognized[c] )
      { printf("\n*** %s ***",arg[c]); arg_error_return(5);};
   // ���������������������������������������������������������������������
		if(!Base_Address)	Base_Address = PS_available();
		if(!Base_Address) arg_error_return(8);
}