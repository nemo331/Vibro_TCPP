#include <stdio.h>
#include <conio.h>
#include <dos.h>
//#include <alloc.h>
#include <string.h>
//#include <sys\stat.h>
//#include <fcntl.h>
//#include <io.h>
//#include <math.h>
#include <time.h>
#include <stdlib.h>
const int CN=16;
int Number=0;
time_t sekund=0;
time_t vremia=0;
const unsigned char mask1=0x21;
void interrupt(*TIMER_INT)(...);  // �窠 �室� ���뢠��� ⠩��� BIOS
void interrupt(*ADC_INT)(...);    // �窠 �室� ������ ���뢠��� ⠩���
void interrupt multiple_int(...); // �ணࠬ�� ��ࠡ��� ���뢠��� ⠩���
int aRes;    //�᫮ ���⠭��� �� ����
int Chast;                      	// ���� ���� ������ ���
int k;
char OldMask1;
unsigned int End_of_work;
int ustavka;
int *Nom_kol;
char chans[20];
int fl;
void kol_kan(char* strok, int *mas1); //�㭪�� ��।������ ���-�� �������
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
  for(k=0;k<fl;k++)   // ����� ������� ���
	{
		outportb(hig_port,(unsigned char)(Nom_kol[k]));     // ��⠭���� ����� ������ ���
		outportb(low_port,0xFF);                 // ����� ���(�� �᫮)
		while((0x7f|inportb(0x222))!=0xff) {};   // �������� ���� �८�ࠧ�������
		aRes=((0x0F&inportb(hig_port))*256+inportb(low_port)-2048)&0xFFFE;
		Number=Number+1;
		
		  if(Number==Chast)
	             {
	               Number=0;
		       sekund=sekund+1;//������ ᥪ㭤 ��襤�� � ��砫� ����
		     }
		printf("\r����� ��� N %d",k);
		printf("\t%5d",aRes);
		if(abs(aRes)>=ustavka) break;
		
	}
	 long ouar=sekund/3600;
	 long min=(sekund-ouar*3600)/60;
         long sek=sekund-(ouar*3600+min*60);
	 printf("\t%03ld:%02ld:%02ld\r",ouar,min,sek);
	 if (kbhit())  { if (getch()==27) goto E_ESC;} //��室 �� ESC
	 if (kbhit())  { if (getch()){End_of_work=1; goto E_ESC;}}
	 if(abs(aRes)<ustavka) goto M_RET;          //�᫮��� �த������� ����
 E_ESC:
	 outportb(mask1,0xFF);
	 if (abs(aRes)>=ustavka||End_of_work==1)  {End_of_work =1;}
	 else End_of_work =2;
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
int main(int argc,char* argv[])
{
    clrscr();
    printf("\t\t�ணࠬ�� ����୮�� ���� ���\n");
    printf("    ��室 � DOS �� ESC\n");
    printf("  ���������� ����� ᡮ� ������ �� F1-F12\n");
     Nom_kol=(int*)calloc(16,sizeof(int));
     if(Nom_kol==NULL){printf("Out memory\n");return 0;}
     printf("\t����஫��㥬� ������\n");
     strcpy(chans,argv[1]);
     printf("%s\n",chans);
     fl=strlen(chans);
     if(fl>16||fl<1)
       {printf("���ࠢ��쭮� �᫮ �������!!!\n");return 0;}
     printf("\t��⠢�� �� ��� ������\n");
     ustavka=abs(atoi(argv[2]));
     printf("%d\n",ustavka);
    if(ustavka>2047||ustavka<1)
       {printf("���ࠢ��쭮� ��᮫�⭮� ���祭�� ��⠢��!!!\n");return 0;}
    Chast=400;
    kol_kan(chans,Nom_kol);

    vremia=time(NULL); //��������� ��⥬��� �६�

    End_of_work=0;
      printf("����饥 ���祭�� ���\n");
      ADC_INT =multiple_int;
      adc_install();
      for(;End_of_work==0;) {}
      free (Nom_kol);
      printf("\n");
      vremia=vremia+sekund;
      stime (&vremia);//��⠭����� ��⥬��� �६� � ��⮬ ࠡ��� �ணࠬ��
      if(End_of_work==1)
      {printf("�ॢ�襭�� �஢�� ᨣ����!!!");
      return 10;}
      if(End_of_work==2)
      { printf("��室 �� ESC!!!");
	return 0;
      }

}
//-------------------------------------------------------------------------

     void kol_kan(char *strok, int *mas1)
     {
     char odsim[2];
     odsim[0]='\0';
     odsim[1]='\0';
     for(int tf=0;tf<fl;tf++)
     {
      memcpy(odsim,&strok[tf],1);      //�८�ࠧ������ ��ப� � ����ࠬ�
      sscanf(odsim,"%x",&mas1[tf]); //������� � ������� �᫠ � 
      strset(odsim,'\0');              //��࠭���� �� � ���ᨢ� mas1
     // printf("%d\n",mas1[tf]);
     }

}   //����� ������� kol_kan
//---------------------------------------------------------------------