/***********************************************************************/
/*�ணࠬ�� ��ࠡ�⪨ rez-䠩��� */
/***********************************************************************/

#include <stdio.h>           
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <alloc.h>
#include <sys\stat.h>
#include <time.h>
#include <math.h>
#include <fcntl.h>
#include <dos.h>
#include <io.h>
#include <dir.h>

int Vel_ust[16]; //����稭� ��⠢�� �� ������� ������
int kol_obr_f=0; //���-�� ��ࠡ�⠭��� 䠩���
int kol_zap_f=0; //���-�� ����ᠭ��� 䠩���
int kol_prev_f=0;//���-�� 䠩��� ������ �ॢ�襭�� ���祭�� ��� ��⠢���

FILE *myfile;   //���� � ��⠢����
FILE *basefile; //log-䠩�
 
char f_ust[45]; //��� 䠩�� � ��ࠬ��ࠬ� �����
char name_f1[150]; //��᪠ ��� ��� ��宦����� ��ࢮ�� 䠩�� �����
	char str1[60];
	char tttt[30];
	char str3[60];
	char str4[13];
	char* filu=NULL;
	char naz_filov[128];
int osh2=0;
int srv=1;
int flg_srv=0;
	
  int obr_f_dan(void);   // �㭪�� ��ࠡ�⪨ 䠩�� ��⠭����
  int obr_f_rez(char *str);   //�㭪�� �⥭�� rez-䠩��
  

  void main(void)
  { clrscr();
    //mkdir ("biv"); //��⠫�� �㤠 �㤥� ����ᥭ ᮡ࠭�� 䠩�
    printf("\n\t��ࠡ�⪠ 䠩��� १��⮢ OB_REZ (����� 1.0)\n");
    strcpy(tttt,"rez\\");
    strcpy(f_ust,"ust.txt");
    int osh=obr_f_dan();
    if (osh>0)
    {printf("\n� 䠩�� ��ᯮ�⮢ ����饭� %d �訡��(��) \n",osh);}
    
    struct ffblk ffblk;
    int done;
    char *sp;
    strcpy(name_f1,"REZ\\*.rez");
    sp=name_f1;
    done=findfirst(sp,&ffblk,0);
    sprintf(str3,"obr_rez.log");
    while (!done)
    {
       strncpy(str4,ffblk.ff_name,8);
       osh2=obr_f_rez(str4);
       if(osh2>0)
       {
	basefile=fopen(str3,"at+");
	if(basefile==NULL)
	    {printf("�� ���� ������ 䠩�%35s\n",str3);}
	srv=1;
	flg_srv=0;
	do
		{
		filu=fgets(naz_filov,13,basefile);
		srv=strcmp(filu,strlwr(ffblk.ff_name));
		if (srv==0) {flg_srv++;}
		filu=fgets(naz_filov,13,basefile);
		}  while (filu!=NULL);
	if (flg_srv==0)
		{ fprintf(basefile,"%s\n",strlwr(ffblk.ff_name));
		  kol_zap_f++;
		}
	fclose(basefile);	 
       kol_prev_f++;	
       }
       kol_obr_f++;
       done=findnext(&ffblk);
    }
    printf("��ࠡ�⠭� %d 䠩���, �� ��� %d �ॢ�蠥� ��⠢�� � %d ����ᠭ� � log-䠩�\n",kol_obr_f,kol_prev_f,kol_zap_f);
  }
/**************************/
/* �⥭�� 䠩�� ��ࠬ��஢*/
/**************************/
  int obr_f_dan(void)
       {

	int osh=0;
	int c;
	int ot=0;        //���� ᨬ���� ⠡��樨
	int on=0;        //���� ᨬ���� ���� ��ப�
	int kav=0;       //���� ᨬ���� ����祪
	int fldv=0;      //���� ᨬ���� �������
	char pred[100];
	char prom[100];
	char t[2];
       sprintf(str1,"%s",f_ust);       //  C:\\TC\\WORK2\\
       myfile=fopen(str1,"rt");  //����⨥ ��⠭���筮�� 䠩��
      if(myfile==NULL)
	    {printf("�� ���� ������ 䠩�%35s\n",str1);return 0;}
      char* df=NULL;

      c=0;
      for(int tf=0;tf<100;tf++)
     {
   do{
	 strcpy(pred,"");
         fldv=0;    //䫠� �� ������� ��襭
      do {
	  df=fgets(t,2,myfile);
	    ot=stricmp(df,"\t"); //    �ࠢ���� �� ����稥 ᨬ���� ⠡��樨
	    on=stricmp(df,"\n"); //    �ࠢ���� �� ����稥 ᨬ���� ��ॢ��� ��ப�
	    kav=stricmp(df,"\:");
	    if(kav==0) {fldv=1;}
             if (df==NULL) break; 
	   if (fldv!=0 && on!=0 && kav!=0) { strcat(pred,t);}
	   if (fldv!=0) strcpy(prom,pred);
	   if(kav==0) {c=c+1;}
	 }  while (ot!=0 && on!=0); // ������ ���� �� ⠡���� ��� ���� �� ��ॢ�� ��ப�
	  
	     switch(c)
	     { 
	case 1: Vel_ust[0]=atoi(prom);		 break;
	case 2: Vel_ust[1]=atoi(prom);	 	 break;
	case 3: Vel_ust[2]=atoi(prom);            break;
	case 4: Vel_ust[3]=atoi(prom);	         break;
	case 5: Vel_ust[4]=atoi(prom);		 break;
	case 6: Vel_ust[5]=atoi(prom); 	 	 break;
	case 7: Vel_ust[6]=atoi(prom);		 break;
	case 8: Vel_ust[7]=atoi(prom);	 	 break;
	case 9: Vel_ust[8]=atoi(prom);            break;
	case 10: Vel_ust[9]=atoi(prom);	         break;
	case 11: Vel_ust[10]=atoi(prom);		 break;
	case 12: Vel_ust[11]=atoi(prom); 	 	 break;
	case 13: Vel_ust[12]=atoi(prom);		 break;
	case 14: Vel_ust[13]=atoi(prom);	 	 break;
	case 15: Vel_ust[14]=atoi(prom);           break;
	case 16: Vel_ust[15]=atoi(prom);	         break;
	
	default: break;
	     }
	   
        if (df==NULL) break;
	
      }  while (on!=0);
        if (df==NULL) break; 
     }	
     fclose(myfile);
      return osh;
       } //����� ������� obr_f_dan

/********************/
/* �⥭�� rez-䠩�� */
/********************/
  int obr_f_rez(char* str)
       { 
	 FILE *rezfile; //rez-䠩�
	 int Nom_kol[]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //���ᨢ 䫠��� �� ⥬ ������� ����� ����
	 int aRes[]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //���ᨢ � ����묨 �� ����樨 楫� �᫠
	 int osh3=0;
	 int on=0;        //���� ᨬ���� ���� ��ப�
	 int flg_kan=0;   //���� ������
	 int flg_K=0;     //䫠� �㪢� �
	 int flg_space=0;  //䫠� �஡���
	 char* pred=" ";
	 char prom[100];
	 char stroka[128];
	 char ssss[80];
	 
	 char t[2];
	 int n_pp=0;
	 int kol_kan=0;
	 strcpy(ssss,tttt);
	 strcat(str,".rez");
	 strcat(ssss,str);
	 rezfile=fopen(ssss,"rt+");  //����⨥ 䠩�� १���⮢
	 if(rezfile==NULL)
	    {printf("�� ���� ������ 䠩�%35s\n",ssss);return 0;}
	 char* df=NULL;
	 
	  df=fgets(stroka,11,rezfile);

	  do {  
		df=fgets(t,2,rezfile);
		on=stricmp(df,"\n"); //    �ࠢ���� �� ����稥 ᨬ���� ��ॢ��� ��ப�
		flg_K=stricmp(df,"�");
		flg_space=stricmp(df," "); 
		if (flg_K==0) flg_kan=1;
		if (flg_space==0) flg_kan=0;
		
		if (flg_kan==1 && flg_K!=0) 
		  { 
		    strcat(pred,t);
		    n_pp++;
		    if (n_pp>1)
		    {Nom_kol[kol_kan]=atoi(pred)+1;
		     n_pp = 0;
		     strcpy(pred," ");
		     kol_kan++;
		    }
		  }

	     }while (on!=0);
	  df=fgets(stroka,128,rezfile);
	  df=fgets(stroka,128,rezfile);
	  df=fgets(stroka,10,rezfile);
	  for(int ttt=0;ttt<kol_kan;ttt++)
	  { 
	  df=fgets(stroka,6,rezfile);
	  //df=fgets(t,2,rezfile);
	  aRes[Nom_kol[ttt]-1]=atoi(stroka);
	  if (Vel_ust[Nom_kol[ttt]-1]>0)
	     {
	     if(aRes[Nom_kol[ttt]-1]>Vel_ust[Nom_kol[ttt]-1])
	     { osh3=1;}
	     }
	  }
	 df=fgets(stroka,128,rezfile);
	 fclose(rezfile);
	 return osh3;
       }     //����� ������� obr_f_rez