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

int *Nom_kol; //���ᨢ � ����ࠬ� ������� ᡮ�
int *Nom_deg; //���ᨢ � ����ࠬ� ������� ��� ����
int nom_kan_otm[1];

FILE *myfile;   //���� � ��ࠬ��ࠬ� ����
FILE *basefile; //���� �����

char ch_kan[40]; //������ ���� � ���� ��ப�
char ch_deg[40]; //������ ����୮�� ���� � ���� ��ப�
int far **aRes; //���ᨢ � ����묨 �� ����樨 � ����� ���
char f_ust[45]; //��� 䠩�� � ��ࠬ��ࠬ� �����
char name_f1[15]; //��� ��ࢮ�� 䠩�� �����(�������� �� ��� ��ப� ��� bat-䠩��)
char name_f2[7]; //���� 6 ᨬ����� ������� �������� 䠩��
char rassh[5]; //���७�� 䠩�� �����
int nom_fila;    //����� ���������� �������� 䠩��
char nom_f_c[3];
	char str1[50];
	char str2[50];
	char kod[8];
	char vremia[18];
	
  typedef struct 
       { char regim_opr; //०�� ���� (0-����⨢�� 1-������)
	 int kol_filov; //���-�� 䠩��� ᡮ�
	 int adres;  //���� ����� ��� (220)
	 float Chastota; //����� ���� ���
	 int fl; //���-�� ������� ��� ᡮ�
	 int Number; //���-�� ����⮢ �� �����
	 char zap_otm; //����� �� �⬥�稪� (1-��, 0-���)
	 char num_otm[2]; //����� ������ �⬥�稪�
	 int num_nab;  //����� ����� �����樥�⮢
	 float min_porog;//��ண��� ���祭�� ��� ����᪠ ����
	 float max_porog;
	 int fl_deg; //���-�� ������� ��� ��� ����
       } partip;   //������� ������ � ��ࠬ��ࠬ� ����

  typedef struct 
       { int kol_filov2; //���-�� 䠩��� ᡮ�
	 float Chastota2; //����� ���� ���
	 char ch_kan2[18]; //������ ���� � ���� ��ப�
	 int Number2; //���-�� ����⮢ �� �����
	 int num_otm2; //����� ������ �⬥�稪�
	 int num_nab2;
       } vuhtip;   //������� ������ � ��ࠬ��ࠬ� ��� ����� � ��室��� 䠩�

    float aKof1[]={0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,
		   0.25,0.25,0.25,0.25,0.25,0.25,0.25,};
    float aKof2[]={0.02657878, 0.254065041, 0.026966292, 0.257953568,
                   0.026773762, 0.259515571, 0.027548209, 0.265674814,
		   0.02608242, 0.247116969, 0.027083145, 0.260642919,
		   0.026821157, 0.255362615, 0.02728513, 0.261551874};//���-1 (��室� �� 諥�䮢� ��樫�����)
   float aKof3[]={0.273224044,0.267379679,0.273224044,0.304878049,
		  0.263157895,0.267379679,0.255102041,0.267379679,
		  0.273224044,0.267379679,0.273224044,0.304878049,
		  0.263157895,0.267379679,0.255102041,0.267379679}; //���-2 (��室� �� ����. ����殮��� �� ���)		 
   float aKof4[]={0.025106703,0.239808153,0.025106703,0.234192037,
		  0.025297243,0.235294118,0.02728513,0.265251989,
		  0.024431957,0.228832952,0.024956326,0.232018561,
		  0.024697456,0.224719101,0.0264527839,0.230946882};//���-2 (��室� �� 諥�䮢� ��樫�����) 
   float aKof[16]={1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,1.0,
		   1.0,1.0,1.0,1.0};

  partip par;
  vuhtip vuh;
  int obr_f_dan(void);   // �㭪�� ��ࠡ�⪨ 䠩�� ��⠭����
  void kol_kan(char* strok, int *mas1); //�㭪�� ��।������ ���-�� ������� � ���-�� �� ����७��

  void main(int argc,char* argv[])
  { clrscr();
    mkdir ("biv"); //��⠫�� �㤠 �㤥� ����ᥭ ᮡ࠭�� 䠩�
    printf("\n\t\t���� ������� ��� \n");
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
    nom_fila=atoi(nom_f_c);

    strcpy(f_ust,"C:\\TC\\WORK2\\param.txt");
    int osh=obr_f_dan();
    if (osh>0)
    {printf("\n� 䠩�� ��ᯮ�⮢ ����饭� %d �訡��(��) \n",osh);}
    printf("\t\t��ࠬ���� ���� \n");
    printf("\n\t\t����� ����: ");
    switch  (par.regim_opr)
	 { case 0: printf("����⨢��"); break;
	   case 1: printf("������"); break; }
    printf("\n\t\t���-�� 䠩��� ᡮ�: %d",par.kol_filov);
    printf("\n\t\t���� ����� ��� LA70: %d",par.adres);
    printf("\n\t\t����� ���� ���: %.3f",par.Chastota);
    printf("\n\t\t������ ��� ��� ᡮ�: %s",ch_kan);
    printf("\n\t\t���-�� ����⮢ �� �����: %d",par.Number);
    printf("\n\t\t����� �� �⬥�稪�? ");
    switch (par.zap_otm)
	 { case 0: printf("���"); break;
	   case 1: printf("��");  break; }
    printf("\n\t\t����� ������ �⬥�稪�: %s",par.num_otm);
    printf("\n\t\t����� ����� �����樥�⮢: %d",par.num_nab);
    if (par.regim_opr==1)
       { printf("\n\t\t��ண��� ���祭�� ��� ����᪠ ����:");
	 printf("\n\t\t\t�������쭮�: %.2f",par.min_porog);
	 printf("\n\t\t\t���ᨬ��쭮�: %.2f",par.max_porog);
         printf("\n\t\t������ ��� ��� ����୮�� ����: %s",ch_deg);
       }

      
    Nom_kol=(int*)calloc(par.fl,sizeof(int));
    if(Nom_kol==NULL){printf("Out memory\n");return;}
    kol_kan(ch_kan,Nom_kol); 
    Nom_deg=(int*)calloc(par.fl_deg,sizeof(int));
    if(Nom_deg==NULL){printf("Out memory\n");return;}
    kol_kan(ch_deg,Nom_deg);
    kol_kan(par.num_otm,nom_kan_otm);

    sprintf(kod,"vibracia");
    sprintf(str2,"biv\\%s",argv[1]);
    basefile=fopen(str2,"wb+");

    fwrite(&kod,8,1,basefile);
    fwrite(&name_f1,8,1,basefile);
    fwrite(&par.regim_opr,sizeof(par.regim_opr),1,basefile);
    fwrite(&par.kol_filov,sizeof(par.kol_filov),1,basefile);
    fwrite(&par.adres,sizeof(par.adres),1,basefile);
    fwrite(&par.Chastota,sizeof(par.Chastota),1,basefile);
    fwrite(&ch_kan,16,1,basefile);
    fwrite(&par.Number,sizeof(par.Number),1,basefile);
    fwrite(&par.zap_otm,sizeof(par.zap_otm),1,basefile);
    fwrite(&nom_kan_otm[0],2,1,basefile);
    fwrite(&par.num_nab,sizeof(par.num_nab),1,basefile);
    fwrite(&par.min_porog,sizeof(par.min_porog),1,basefile);
    fwrite(&par.max_porog,sizeof(par.max_porog),1,basefile);
    fwrite(&ch_deg,16,1,basefile);
    fclose(basefile);
    free(Nom_kol); 
    free(Nom_deg);	 
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
	case 1: par.regim_opr=atoi(prom);		 break;
	case 2: par.kol_filov=atoi(prom);	 	 break;
	case 3: par.adres=atoi(prom);                    break;
	case 4: par.Chastota=atof(prom);	         break;
	case 5: par.fl=strlen(prom);
		//fl=par.fl;
		if ((par.fl>16)||(par.fl<1))
		{osh=osh+1;}
		strcpy(ch_kan,prom);   	 	 
		break;
	case 6: par.Number=atoi(prom); 	 	 break;
	case 7:  if (strchr(prom,'Y')!=NULL) 
	         {par.zap_otm=1;}
		 else par.zap_otm=0; 	 	 break;
	case 8:  strncpy(par.num_otm,prom,1); 	 
		 break;
	case 9: par.num_nab=atoi(prom); 	 break;
	case 10: par.min_porog=atof(prom); 	 break;
	case 11: par.max_porog=atof(prom); 	 break;
	case 12: par.fl_deg=strlen(prom); 
		 //fl_deg=par.fl_deg;       
		 if ((par.fl_deg>16)||(par.fl_deg<1))
		 {osh=osh+1;}
                 strcpy(ch_deg,prom);
	         break;
	default: break;
	     }
	   
        if (df==NULL) break;
	
      }  while (on!=0);
        if (df==NULL) break; 
     }	
     fclose(myfile);
      return osh;
       } //����� ������� obr_f_dan
/*************************************************************/
/* ��।������ ���-�� ������� � ����ᥭ�� �� ����஢ � ���ᨢ*/
/*************************************************************/
  void kol_kan(char *strok, int *mas1)
     {
     char odsim[2];
     odsim[0]='\0';
     odsim[1]='\0';
     int ch_ccc=strlen(strok);
     for(int tf=0;tf<ch_ccc;tf++)
     {
      memcpy(odsim,&strok[tf],1);      //�८�ࠧ������ ��ப� � ����ࠬ�
      sscanf(odsim,"%x",&mas1[tf]); //������� � ������� �᫠ � 
      strset(odsim,'\0');              //��࠭���� �� � ���ᨢ� mas1
     }
/*        int vrNom=0;
         for(tf=0;tf<fl;tf++)             //���������� ��ண� �⮫�� ���ᨢ�
         {                                //�⢥��饣� �� ������⢮ ����७��  
          vrNom=mas1[0][tf];               //������� ������
           int schNom=0;
           for(int ff=tf;ff<fl;ff++)
	    {if (vrNom==mas1[0][ff]&&mas1[1][ff]==0)
		 {schNom=schNom+1;mas1[1][ff]=-1;}
            }
          mas1[1][tf]=schNom;
	 }                       */
      //	 for(tf=0;tf<fl;tf++)
     //	 printf("%d\n",mas1[tf]);
     }   //����� ������� kol_kan