/***********************************************************************/
/*�ணࠬ�� ᡮ� ������ �� ����樨 � ������� ����� LA70 (����⨢�� */
/* � ������ �����                                                   */
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

int Nom_kol[16]; //���ᨢ 䫠��� �� ⥬ ������� ����� ����
int Vel_ust[16]; //����稭� ��⠢�� �� ������� ������
int aRes[16]; //���ᨢ � ����묨 �� ����樨 楫� �᫠
int nom_kan_otm[1];


FILE *myfile;   //���� � ��⠢����
FILE *basefile; //log-䠩�
FILE *rezfile; //rez-䠩� 

     


char f_ust[45]; //��� 䠩�� � ��ࠬ��ࠬ� �����
char name_f1[15]; //��� ��ࢮ�� 䠩�� �����(�������� �� ��� ��ப� ��� bat-䠩��)
char name_f2[7]; //���� 6 ᨬ����� ������� �������� 䠩��
char rassh[5]; //���७�� 䠩�� �����
int nom_fila;    //����� ���������� �������� 䠩��
char nom_f_c[3];
	char str1[50];
	char str2[50];
	char* filu=NULL;
	char naz_filov[128];
	
 
  int obr_f_dan(void);   // �㭪�� ��ࠡ�⪨ 䠩�� ��⠭����
  int obr_f_rez(void);   //�㭪�� �⥭�� rez-䠩��
  

  void main(int argc,char* argv[])
  { clrscr();
    //mkdir ("biv"); //��⠫�� �㤠 �㤥� ����ᥭ ᮡ࠭�� 䠩�
    printf("\n\t\��ࠡ�⪠ 䠩��� १��⮢ \n");
    strncpy(name_f1,argv[1],8);
    if (strlen(argv[1])!=12)
     {printf("���ࠢ��쭠� ����� �������� 䠩�� �����!\n"); return;}
    if(strrchr(argv[1],'.')==NULL)
     {printf("�� 㪠���� ���७�� � �������� 䠩�� �����!\n"); return;}
    strncpy(name_f2,argv[1],6);
    strrev(strncpy(rassh,strrev(argv[1]),4));
    strrev(argv[1]);
    strrev(strncpy(nom_f_c,strrev(name_f1),2));
    strrev(name_f1);

    strcpy(f_ust,"C:\\TC\\WORK2\\ust.txt");
    int osh=obr_f_dan();
    if (osh>0)
    {printf("\n� 䠩�� ��ᯮ�⮢ ����饭� %d �訡��(��) \n",osh);}
    int osh2=obr_f_rez();

      
     
    

    
    sprintf(str2,"C:\\TC\\WORK2\\obr_rez.log");
    basefile=fopen(str2,"at+");
    filu=fgets(naz_filov,13,basefile);
    fprintf(basefile,"%s \n",argv[1]);
    
    fclose(basefile);	 
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
  int obr_f_rez(void)
       { 
	 int osh=0;
	 int on=0;        //���� ᨬ���� ���� ��ப�
	 int flg_kan=0;   //���� ������
	 int flg_K=0;     //䫠� �㪢� �
	 int flg_space=0;  //䫠� �஡���
	 char* pred=" ";
	 char prom[100];
         char stroka[128];
	 char t[2];
	 int n_pp=0;
	 int kol_kan=0;
	 sprintf(str2,"C:\\TC\\WORK2\\REZ\\%s.rez",name_f1);       //  C:\\TC\\WORK2\\
	 rezfile=fopen(str2,"rt");  //����⨥ ��⠭���筮�� 䠩��
	 if(myfile==NULL)
	    {printf("�� ���� ������ 䠩�%35s\n",str2);return 0;}
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
	     if(aRes[Nom_kol[ttt]-1]>Vel_ust[Nom_kol[ttt]-1]);
	     {return 1;}
	     }
	  }
	 fclose(rezfile);
	 return osh;
       }     //����� ������� obr_f_rez