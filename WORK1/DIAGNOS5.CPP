#include <stdio.h>
#include <conio.h>
#include <dos.h>
#include <alloc.h>
#include <string.h>
#include <sys\stat.h>
#include <fcntl.h>
#include <io.h>
#include <math.h>
#include "menu.h"// 䠩� �㭪権 ��� ࠡ��� � ���������� ०���
#include <dir.h>
//
const int CN=16;
const int Number=8192;
const unsigned char mask1=0x21;
float aKof[]={0.02657878, 0.254065041, 0.026966292, 0.257953568,
						  0.026773762, 0.259515571, 0.027548209, 0.265674814,
						  0.02608242, 0.247116969, 0.027083145, 0.260642919,
							0.026821157, 0.255362615, 0.02728513, 0.261551874};
float aK_vit[]={0.5243365425, 0.5243365425};
void interrupt(*TIMER_INT)(...);  // �窠 �室� ���뢠��� ⠩��� BIOS
void interrupt(*ADC_INT)(...);    // �窠 �室� ������ ���뢠��� ⠩���
void interrupt multiple_int(...); // �ணࠬ�� ��ࠡ��� ���뢠��� ⠩���
void Obrab(void);
void S_Furie(int, int);
char OldMask1;
char End_of_work;
int huge aRes[CN][Number];
float huge aGv[Number];
float huge aGm[Number];
float huge nGbv[Number>>1];
float huge nGbm[Number>>1];
float Chast;                      	// ���� ���� ������ ���
int i, k;
int Kanal;                        // ������⢮ �����訢����� �������
typedef   struct fil
	  {  char name[14];
	     int  flag;
	  }  styp ;
    styp katal[60];
  int kolotm=0;   // ������⢮ ��࠭��� 䠩��� ��� ��ࠡ�⪨

// �㭪�� �롮� 䠩��� �� 㪠������� ��⠫���
// �������� ���ᨢ katal �⬥祭�묨 䠩����      
//----------------------------------------------
void dir(char str[10])
{            struct ffblk ffblk;


  int done;
  char *sp;
  sp=str;
  done=findfirst(sp,&ffblk,0);
  int i=0;
  int j=0;
  int kol=0;
	okno(9,10,72,12);
  while (!done)

     { for(j=0; j<9; j++)
	 { if ( ffblk.ff_name[j]=='.')  ffblk.ff_name[j]=0;} //㡨ࠥ� ���७��
       stpcpy(katal[kol].name,ffblk.ff_name);
       textbackground((kol==0)?0:3);
       cprintf("%-12s",katal[kol].name);
       katal[kol].flag=0;
       i++;
       kol++;
       if (i==5) { cprintf("\n\r "); i=0;}
       done=findnext(&ffblk);
     }
     katal[kol].flag=22;//�ਧ��� ���� 䠩��� � ��⠫���
     int x=2;
     int y=2;
     gotoxy (x,y);
     i=1; // ����� � �⮫��
     j=0; // ⥪�騩 䠩�
     kolotm=0;
     while(5==5)
      { gotoxy(x,y);
	switch(getch())
	 { case 32 :    katal[j].flag=((katal[j].flag==1)?0:1); //�뤥�����
			kolotm=((katal[j].flag==1)?kolotm+1:kolotm-1);//��������
			textbackground(0);
			textcolor((katal[j].flag==1)?14:15);
			cprintf("%-11s ",katal[j].name);
			break;
	   case 13 : return; // ��室 �� ENTER
	   case  0 : switch( getch() )
			{ case 75 :  if (j==0) break; // ����� �����
				textbackground(3);
				textcolor((katal[j].flag==1)?14:15);
				cprintf("%-11s ",katal[j].name);
				j--;
				if (i==1) {  y--;
					     x+=48;
					     i=5;
					  }
				else      {  x-=12;
					     i--;
					  };
				gotoxy(x,y);
				textbackground(0);
				textcolor((katal[j].flag==1)?14:15);
				cprintf("%-11s ",katal[j].name);
				break;
			  case 77 :  if (j==kol-1) break; //����� ��ࠢ�
				textbackground(3);
				textcolor((katal[j].flag==1)?14:15);
				cprintf("%-11s ",katal[j].name);
				j++;
				if (i==5) { x=2;
					    i=1;
					    y++;
					  }
				else      {  x+=12;
					    i++;
					  };
				gotoxy(x,y);
				textbackground(0);
				textcolor((katal[j].flag==1)?14:15);
                                cprintf("%-11s ",katal[j].name);
				break;
		       case 72 :  if (y==2) break; //����� �����
				textbackground(3);
				textcolor((katal[j].flag==1)?14:15);
				cprintf("%-11s ",katal[j].name);
				j-=5;
				y--;
				gotoxy(x,y);
				textbackground(0);
				textcolor((katal[j].flag==1)?14:15);
				cprintf("%-11s ",katal[j].name);
				break;
		       case 80 :  if (j+6>kol) break; // ����� ����
				textbackground(3);
				textcolor((katal[j].flag==1)?14:15);
				cprintf("%-11s ",katal[j].name);
				j+=5;
				y++;
				gotoxy(x,y);
				textbackground(0);
				textcolor((katal[j].flag==1)?14:15);
				cprintf("%-11s ",katal[j].name);
				break;
			  default : break;
			};
	  default : break;
	 };
      };
}
//--------------------------------------------------------------
/**********************************************************
//   ��楤�� ���⠫�樨 ������ ���뢠��� ⠩���
***********************************************************/
void near adc_install(void)
{
	unsigned int m_delay;
	outportb(0x223,0x88);            // �ணࠬ��஢���� ��஢��� ���� ���
	TIMER_INT=getvect(0x08);         // ��࠭���� ���뢠��� ⠩��� BIOS
	disable();                       // ����� ��᪨�㥬�� ���뢠���
	OldMask1 = inportb(mask1);       // ��࠭���� ��᪨ �������� ���뢠���
	outportb(mask1,0xFF);            // ����� �������� ���뢠���
	setvect(0x08,ADC_INT);           // ��⠭���� ������ ���뢠���
	outportb(0x43,0x36);                        //  #
	m_delay=(unsigned int)(1193820L/Chast);     //  # �ணࠬ��஢���� ⠩���
	outportb(0x40,(unsigned char)m_delay);      //  #   (����� ���祭��)
	outportb(0x40,(unsigned char)(m_delay>>8)); //  #  
	outportb(mask1,0xF8);             // ��⠭���� ��᪨ �������� ���뢠���
	enable();                         // ࠧ�襭�� ���뢠���
	return;
}
/********************************************************
//  �ணࠬ�� ��ࠡ�⪨ ������ ���뢠��� ⠩���
*********************************************************/
void interrupt multiple_int(...)
{
	const int low_port=0x224;         // ����� �����/�뢮�� ��� (������
	const int hig_port=0x225;         // ���� 0�220 ��⠭���������� ��६�窮�)
  for(k=0;k<CN-Kanal;k++)   // ����� ������� ���
	{
		outportb(hig_port,(unsigned char)(k+Kanal));     // ��⠭���� ����� ������ ���
		outportb(low_port,0xFF);                 // ����� ���(�� �᫮)
		while((0x7f|inportb(0x222))!=0xff) {};   // �������� ���� �८�ࠧ�������
		aRes[k][i]=((0x0F&inportb(hig_port))*256+inportb(low_port)-2048)&0xFFFE;
	}
	i++;                       // ���稪 ����஢
	if(Number!=i)goto M_RET;
	outportb(mask1,0xFF);      // ����� �������� ���뢠���
	End_of_work =0xFF;         // ��⠭��� 䫠�� �����襭�� ����
	disable();                 // ����� ��᪨�㥬�� ���뢠���
	setvect(0x08,TIMER_INT);   // ����⠭������� ���뢠��� ⠩��� BIOS
	outportb(0x43,0x36);       //  #
	outportb(0x40,0xFF);       //  # �ணࠬ��஢���� ⠩���
	outportb(0x40,0xFF);       //  # (����⠭������� ���祭��)
	outportb(mask1,OldMask1);  // ����⠭������� ��᪨ �������� ���뢠���
	enable();                  // ࠧ�襭�� ���뢠���
M_RET:
	outportb(0x20,0x20);
return;
}
/***********************************************
*            �᭮���� �ணࠬ��
************************************************/
void main(void)
{
	int ns;
	int x, y;
	char key;
	char sBN[6];
	char sName[25];
	FILE *myfile;
	int sw;
	mkdir("bin");       // ���뢠�� ��४�ਨ
	mkdir("b_vitok");   //����室��� �� ࠡ��
	mkdir("furie");
	mkdir("f_vitok");
	mkdir("max_f");
	mkdir("ogibanie");
	mkdir("oscill");
	mkdir("os_vitok");

begin:	zag("�������⨪�.    * ����� 5.1 DOS. *");
	clear();//���⪠ ��࠭�
	sw=win(20,7,60,"���  ������ ?","���������� �������","������������ ����������","�����","","");
	if (sw==3) { oldreg(); return;};
	clear();
	k=win(20,7,60,"���� ?","��������","����� ","����� ","","");
	if (k==3) goto begin;
	fflush(stdin);
	switch(sw)
	{
		case 1:
			switch(k)
			{
				case 1:
					zag("�������⨪�. ����� ���(��������)   * ����� 5.1 DOS. *");
					Chast=400.0;           // ���� ���� ������ ���
					Kanal=0;             // ������⢮ �����訢����� �������
					break;
				case 2:
					zag("�������⨪�. ����� ���(�����)   *   ����� 5.1 DOS. *");
					Chast=6000.0; 	// ���� ���� ������ ���
					Kanal=14;            // ������⢮ �����訢����� �������
					break;
				default:
					goto begin;
			} 
PDD:
	//----------------------------------------------------------------
//������ � �ନ�㥬 ��� 䠩��
	clear();
	okno(26,15,56,5);
	cprintf("������ ������� ��� 䠩���.\n\r (�� ����� ��� ᨬ�����!!!)");
	textcolor(0);
	_setcursortype(_SOLIDCURSOR);
	gotoxy(13,5);
	do
	{char *sBNb;
	sBN[0]=6;
	sBNb=cgets(sBN);
	strcpy(sBN,sBNb);}
	while (sBN[0]==0);
	ns=strlen(sBN);

			for(i=ns;i<6;i++)
			{
				sBN[i]='_';
			}
			int name=1;
//---------------------------------------------------------
Ret_Opr:
			i=0;
			End_of_work=0;
	char zagol[30];
	sprintf(zagol," %-3i������ ������ ��� ?",name);
	clear();
	key=win(25,7,56,zagol,"�� ","���","","","");


	  	fflush(stdin);
			if(key==2) goto End_Opr;
		clear();
		okno(25,15,55,2);
		textcolor(128+15);
		cprintf("�����!!! ���� %2i-� ����!",name);

			ADC_INT =multiple_int;
			adc_install();
			for(;End_of_work==0;) {}
			if(Kanal==0)
				sprintf(sName,"bin\\%.5s_%02i.bin",sBN,name);

			else
			  sprintf(sName,"b_vitok\\%.5s_%02i.bin",sBN,name);
			myfile=fopen(sName,"wb+");
			if(myfile==NULL)
			{
				clear();
				soob(30,15,"���� �� �����!");
				zag("�������  �����  �������");
				getch();
				goto begin;
			}
			for (i=0;i<Number;i++)
			{
				for(k=0;k<CN-Kanal;k++)
					fwrite(&aRes[k][i],2,1,myfile);
			}
			fclose(myfile);
			gotoxy(x,y);
			printf("                                                   ");
			name++;
			goto Ret_Opr;
End_Opr:                clear();
			soob(31,15,"����� ������!!!");
			zag("�������  �����  �������");
			getch();
			break;
		case 2:
			Obrab();
			break;
		default:
			goto begin;
	}
	goto begin;
}
/*
//--------------------------------------------------
//  �/�ணࠬ�� ��ࠡ�⪨ ������ ��� 
//--------------------------------------------------
*/
void Obrab(void)
{
	FILE *maxfile;
	FILE *myfile;
	FILE *basefile;
	char str[40];
	char cBuf[10];
	float Kv_p, Kv_t;
	float Time, S_kv, D_Z;
	float nMax;
	float nOgr;
	int KeyM;
	int ox, oy;
	int x, y;
	int I_min, I_m_p, I_max, Step, Period;

	switch(k)
	{
		case 1:
			zag("�������⨪�. ���������(��������)   * ����� 5.1 DOS. *");
			Chast=400.0;                        // ���� ���� ������ ���
			Kanal=0;
	int fogr;
	clear();
	fogr=win(22,7,60,"������� ����������� �� ���������","�� ","���","","","");
	clear();
	okno(18,4,63,3);
	textcolor(14);
	cprintf(" �롥�� '��������'  䠩�� ��� ��ࠡ�⪨\n\r");
	cprintf("             � ������  ENTER");
	
	dir("bin\\*.bin"); //�㭪�� �롮� 䠩��� ��� ��ࠡ�⪨
	int nomfile;
	nomfile=0;
	int nom=0;
  while (katal[nomfile].flag!=22) // ���� ���� 䠩�� � ��४�ਨ
   {                              // �믮��塞 ....
      if (kbhit())  { if (getch()==27) return;} //��室 �� ESC
      if ( katal[nomfile].flag==1) // �᫨ 䠩� ����祭, �
      {                           // ��ࠡ��뢠�� ���
	      clear();
	      strcpy(cBuf,katal[nomfile].name);
	      sprintf(str,"bin\\%.8s.bin",cBuf);
	      myfile=fopen(str,"rb");
			if(myfile==NULL)
			{       clear();
				okno(26,15,56,3);

		     cprintf("������ ������ �����!!!");
		     return;
			};
	okno(26,8,58,4);
	cprintf(" ��ࠡ�⠭� %-2d 䠩��� �� %-2d\n\r",nom,kolotm);
	cprintf("      ���� ��ࠡ�⪠ 䠩��\n\r");
	textcolor(14);
	cprintf("            %-8s\n\r",katal[nomfile].name);

	char *sBNb;
	basefile=myfile;

	char strnamb[10];
	okno(22,17,62,4);
	if (fogr==1)
	   {  cprintf("������ ��࠭�祭�� �� ������㤥:");
	      textcolor(0);
	      _setcursortype(_SOLIDCURSOR);
	      gotoxy(15,4);
	      strnamb[0]=10;
	      sBNb=cgets(strnamb);
	      strcpy(strnamb,sBNb);
	      nOgr=atof(strnamb);
	    }
	else  nOgr=0;
			ox=wherex();
			oy=wherey();
	           for(k=0;k<CN;k++)
			{
				gotoxy(ox,oy-1);
	       okno(22,17,62,4);
		textcolor(128+15);
		cprintf("�����!!! ���� ��ࠡ�⪠ %2i-�� ������!\n\r",k+1);

				for(i=0;i<Number;i++)
				{
					fseek(myfile,2L*k+2L*CN*i,SEEK_SET);
					fread(&aRes[0][i],2,1,myfile);
				}
				S_Furie(Number,k);
				sprintf(str,"oscill\\%.8s.o%02i",cBuf,k+1);
				myfile=fopen(str,"w+");
				textcolor(15);
				cprintf("          ��樫���ࠬ��:");
				x=wherex();
				y=wherey();
				for(i=0;i<Number;i++)
				{
					gotoxy(x,y);
					cprintf("%5.1f%",((float)i/Number)*100);
					sprintf(str,"%9.4f\t%12.4f\n",i/Chast,aRes[0][i]*aKof[k]);
					fwrite(str,23,1,myfile);
				}
				fclose(myfile);
				sprintf(str,"furie\\%.8s.f%02i",cBuf,k+1);
				myfile=fopen(str,"w+");
				sprintf(str,"max_f\\%.8s.m%02i",cBuf,k+1);
				maxfile=fopen(str,"w+");
				cprintf("\n\r              ����:");
				x=wherex();
				y=wherey();
				nMax=nGbv[0];
				for(i=0;i<(Number>>1);i++)
				{
					cprintf("%5.1f%",((float)i/Number)*200);
					gotoxy(x,y);
					sprintf(str,"%9.4f\t%12.4f\n",nGbm[i],nGbv[i]);
					fwrite(str,23,1,myfile);
					if(nMax>nGbv[i])
					{
						if((KeyM==0)&&(nMax>nOgr))
						{
							sprintf(str,"%9.4f\t%12.4f\n",nGbm[i-1],nGbv[i-1]);
							fwrite(str,23,1,maxfile);
							KeyM=-1;
						}
						nMax=nGbv[i];
					}
					else
					{
						nMax=nGbv[i];
						KeyM=0;
					}
				}
				fclose(myfile);
				fclose(maxfile);
				myfile=basefile;
				gotoxy(ox,oy);
			}
			fclose(basefile);
			nom++;// ���६��� ���-�� ��ࠡ�⠭��� 䠩���
	       } ;
	       nomfile++; // �த������� �� ��⠫���
	};
			oldreg();
			break;
		case 2:
			zag("�������⨪�. ���������(�����)   *   ����� 5.1 DOS.  *");
			Chast=6000.0;  // ���� ���� ������ ���
			Kanal=14;    //  ��� ��⪠ Period=60, ��� �� Period=60
			Period=60;   //  �� ���� ���� ��� 6000 ��.
	clear();
	okno(18,4,63,3);
	textcolor(14);
	cprintf(" �롥�� '��������'  䠩�� ��� ��ࠡ�⪨\n\r");
	cprintf("             � ������  ENTER");
	dir("b_vitok\\*.bin");
	nom=0;
	nomfile=0;
  while (katal[nomfile].flag!=22)
   { if (kbhit())  { if (getch()==27) return;}
     if ( katal[nomfile].flag==1)
      {
	      clear();
	      strcpy(cBuf,katal[nomfile].name);
	      sprintf(str,"b_vitok\\%.8s.bin",cBuf);

			myfile=fopen(str,"rb");
			if(myfile==NULL)
			{       clear();
				okno(26,15,56,3);

		     cprintf("������ ������ �����!!!");
		     getch();
		     return;
			};
	okno(25,8,57,4);
	cprintf(" ��ࠡ�⠭� %-2d 䠩��� �� %-2d\n\r",nom,kolotm);
	cprintf("      ���� ��ࠡ�⪠ 䠩��\n\r");
	textcolor(14);
	cprintf("            %-8s\n\r",katal[nomfile].name);

			basefile=myfile;
			printf("\n");
			ox=wherex();
			oy=wherey();
			for(k=0;k<CN-Kanal;k++)
			{
				gotoxy(ox,oy-1);
					okno(21,15,61,4);
					textcolor(128+15);
					cprintf("�����!!! ���� ��ࠡ�⪠ %2i-�� ������!\n\r",k+1);
					textcolor(15);
				for(i=0;i<Number;i++)
				{
					fseek(myfile,2L*k+2L*(CN-Kanal)*i,SEEK_SET);
					fread(&aRes[0][i],2,1,myfile);
				}
				S_Furie(Number,k);
				sprintf(str,"ogibanie\\%.8s.g%02i",cBuf,k+1);
				maxfile=fopen(str,"w+");
        i=0;
				aGv[i]=0;
				for(i=1;i<Number;i++)
				{
					Kv_p=(aRes[0][i-1]*aK_vit[k])*(aRes[0][i-1]*aK_vit[k]);
					Kv_t=(aRes[0][i]*aK_vit[k])*(aRes[0][i]*aK_vit[k]);
					aGv[i]=aGv[i-1]+(Kv_p+Kv_t)/Chast/2;
				}
				Step=0;
				I_m_p=0;       // ������ �।��饣� �������쭮�� ���祭�� 
				I_min=0;       // ������ ⥪�饣� �������쭮�� ���祭��
				while(Number>(I_m_p+Step+Period+1))
				{
					for(i=I_m_p+Step;i<I_m_p+Step+Period;i++)
					{
						if(aRes[0][I_min]>aRes[0][i]) I_min=i;
					}
					if(Step!=0)
					{
						I_max=I_m_p;
						for(i=I_m_p;i<I_min;i++)
						{
							if(aRes[0][I_max]<aRes[0][i]) I_max=i;
						}
            nMax=aRes[0][I_max]*aK_vit[k];
						S_kv=aGv[I_min]-aGv[I_m_p];
						Time=(float)I_min/Chast-(float)I_m_p/Chast;
						D_Z=sqrt(S_kv/Time);
						sprintf(str,"%8.5f\t%12.4f\t%15.6f\n",I_max/Chast,nMax,D_Z);
						fwrite(str,38,1,maxfile);
          }
					I_m_p=I_min;
					Step=(int)(Period/2);      
					I_min=I_min+Step;
				}				
				fclose(maxfile);
				sprintf(str,"os_vitok\\%.8s.o%02i",cBuf,k+1);
				myfile=fopen(str,"w+");
			       cprintf("         ��樫���ࠬ��:");
				x=wherex();
				y=wherey();
				for(i=0;i<Number;i++)
				{
					gotoxy(x,y);
					printf("%5.1f%",((float)i/Number)*100);
					sprintf(str,"%11.5f\t%12.4f\n",i/Chast,(aRes[0][i]*aK_vit[k]));
					fwrite(str,25,1,myfile);
				}
				fclose(myfile);
				sprintf(str,"f_vitok\\%.8s.f%02i",cBuf,k+1);
				myfile=fopen(str,"w+");
				cprintf("\n\r             ����:");
				x=wherex();
				y=wherey();
				for(i=0;i<(Number>>1);i++)
				{
					printf("%5.1f%",((float)i/Number)*200);
					gotoxy(x,y);
					sprintf(str,"%9.4f\t%12.4f\n",nGbm[i],nGbv[i]);
					fwrite(str,23,1,myfile);
		}
				fclose(myfile);
				myfile=basefile;
			}
			fclose(basefile);
//************************************************************************
// �� ���졥 �������� �ନ�㥬 �� ���� 䠩� 
// ⨯�    int �����  float ��樫���ࠬ��
//************************************************************************
			typedef struct elfile
				{   int p;
				  float eds;
				}   strfile;
			strfile  sf[200];

			FILE *inos, *inog, *outog;
			float i,j,k,l,time,bufertime, bufersred=0;
			int flagmax=0,worflag,polys,stroka;


			sprintf(str,"os_vitok\\%.8s.o01",cBuf);
			inos=fopen(str,"r");
			sprintf(str,"ogibanie\\%.8s.g02",cBuf);
			inog=fopen(str,"r");

			sprintf(str,"ogibanie\\%.8s.#02",cBuf);
			outog=fopen(str,"w+");
			polys=96;
			flagmax=0; worflag=1;
			while((fscanf(inos,"%f %f",&i,&j)!=EOF))
			{
			  if((((j<0)?-j:j)>80)&&(worflag==1)) {flagmax=1;worflag=0;}
			  if((flagmax==1)&&(j<0)){  time=bufertime; flagmax=0;};
			  bufertime=i ;

			}
			worflag=1;
			stroka=-1;
			int cstrok;
			cstrok=0 ;
			float sraz=100,nraz;
			while(fscanf(inog,"%f %f %f",&i,&j,&k)!=EOF)
			 {
				if(worflag==1)
				{	nraz=(i-time)*(i-time) ;
					if (nraz>sraz) { worflag=0;time=bufertime;}
					else {  sraz=(time-i)*(time-i); bufertime=i;stroka++;};
				};
				cstrok++;
			 };

			 sprintf(str,"ogibanie\\%.8s.g02",cBuf);
			 inog=fopen(str,"r");
			 int cycl;
			 int levper;
			 levper=0;
			 if ((cstrok-stroka)>=96)
				{ for (cycl=1; (cycl<stroka+1); cycl++) fscanf(inog,"%f %f %f",&i,&j,&k);
				 while(polys>0)
				  {fscanf(inog,"%f %f %f",&i,&j,&k);
				   sf[levper].p=polys--;
				   sf[levper++].eds=k ;
				  }		 ;
				}
			 else
				{ polys=96-(cstrok-stroka);
				for (cycl=1; (cycl<(cstrok-96+1)); cycl++) fscanf(inog,"%f %f %f",&i,&j,&k);
				for (cycl=1; (cycl<97); cycl++)
				  {    if( fscanf(inog,"%f %f %f",&i,&j,&k)!=EOF)
					{ sf[levper].p=polys--;
					  sf[levper++].eds=k ;
					  polys=(polys==0)?96:polys;
					};
				  };

				};
//**************************************************************************
//-----------------------����㥬 �� ����ᠬ -----------------------------
				   strfile bufper;
				   for (cycl=0 ; cycl<95 ;cycl++)
				     {for (levper=0 ; levper<95-cycl ;levper++)
					 { if (sf[levper].p>sf[levper+1].p)
					      { bufper=sf[levper];
						sf[levper]=sf[levper+1];
						sf[levper+1]=bufper;
					       };
					  };
				       };
//**************************************************************************
//  ----------------�����뢠�� � 䠩�-------------------------------------
				   for (cycl=0 ; cycl<96 ;cycl++)
				     fprintf(outog,"%4d\t%15.6f\n",sf[cycl].p,sf[cycl].eds);

			fclose(inos);
			fclose(inog);
			fclose(outog);
//************************************************************************
//************************************************************************
			nom++; //���६��� ���-�� ��ࠡ�⠭���
	      } ;
	  nomfile++; //�த������� �� ��⠫���
	};

			break;
		default:
			return;
	} 
	clear();
	soob(30,15,"����� ���������!!!");
	zag("�������  �����  �������");
	getch();
	return;
}
//--------------------------------------------------------
//   ����஥ �८�ࠧ������ ����
//--------------------------------------------------------
void S_Furie(int Num,int Chen)
{
	int nMatr;
	unsigned int i;
	unsigned int j;
	unsigned int k;
	unsigned int m;
	unsigned int nSt;
	long int ni;
	long int ip;
  float Koeff;
	float nV;
	float nM;
	float nGtv;
	float nGtm;
	float dT =(float)1/Chast;
	double SumKv;
	if(Chast==400)
		Koeff=aKof[Chen];
	else
		Koeff=aK_vit[Chen];
	nMatr=1;
	for(i=0;(Num>>i)!=1;i++)
		nMatr++;
	for(i=1;i<nMatr;i++)
	{
		nSt=(Num>>i);
		ip=0;
		for(j=0;j<(1<<(i-1));j++)
		{
			nV=cos((2*M_PI/(1<<i))*j);
			nM=sin((2*M_PI/(1<<i))*j);
			for(k=0;k<nSt;k++)
			{
				ni=2*nSt*j+k;
				if (i==1)
				{
				aGv[ip]=Koeff*(float)(aRes[0][ni]+aRes[0][ni+nSt]);
				aGv[ip+nSt]=Koeff*(float)(aRes[0][ni]-aRes[0][ni+nSt]);
				aGm[ip]=0;
				aGm[ip+nSt]=0;
				}
				else
				{
				nGtv=aGv[ni]+nV*aGv[ni+nSt]+nM*aGm[ni+nSt];
				nGbv[ip]=aGv[ni]-nV*aGv[ni+nSt]-nM*aGm[ni+nSt];
				nGtm=aGm[ni]+nV*aGm[ni+nSt]-nM*aGv[ni+nSt];
				nGbm[ip]=aGm[ni]-nV*aGm[ni+nSt]+nM*aGv[ni+nSt];
				aGv[ip]=nGtv;
				aGm[ip]=nGtm;
				}
				ip++;
			}
		}
		if (i!=1)
		{
			for(m=0;m<(Num>>1);m++)
			{
				aGv[m+(Num>>1)]=nGbv[m];
				aGm[m+(Num>>1)]=nGbm[m];
			}
		}
	}
	i=0;
	nGbv[i]=aGv[i]/Num;
	nGbm[i]=0;
	for (i=1;i<(Num>>1);i++)
	{
		SumKv=sqrt((aGv[i]/Num)*(aGv[i]/Num)+(aGm[i]/Num)*(aGm[i]/Num));
		nGbv[i]=2.0*(float)SumKv;
		nGbm[i]=i/(dT*Num);
	}
	return;
}