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

  int **Nom_kol; //���ᨢ � ����ࠬ� �������
  FILE *myfile;
  FILE *basefile;
	char str1[50];
	char str2[50];
        char vremia[18];
  int fl; //���-�� �������
  int Number; //���-�� ����⮢ �� �����
  float Chastota=1; //����� ���� ���
  unsigned long tc_poz=0;//⥪��� ������ � 䠩��
  int Nom_bl=0;//����� �����
  int konec_f=0;
  int far **aRes; //���ᨢ � ����묨 �� ����樨 � ����� ���
  float **Pokaz;
  int kolPok=6;//���-�� ������⥫��(���.,����. ������ � �.�. )
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
                    1.0,1.0,1.0};
  FILE *datfile;  //���� � ��⠭������ ��� ��ࠡ�⪨
  char fdan[40];
  int c;
  int ot=0;        //���� ᨬ���� ⠡��樨
  int on=0;        //���� ᨬ���� ���� ��ப�
  int kav=0;       //���� ᨬ���� ����祪
  int fldv=0;      //���� ᨬ���� �������
  char pred[35];
  char prom[35];
  char t[2];
    char Name_f[15];  //��� 䠩��
    char n_f_pol[9];  //��� 䠩�� ��� ���७��
    char f_ust[45];   //��� ��⠭���筮�� 䠩��
    char chans[20];  //������
    char frec[10];   //����� ���� ��� �� ������ ������
    char size[10];   //������ �����
    char path[35];   //���� �� 䠩��
    char flagVib[2]; //���� (������ � 䠩� ���祭�� ����樨 ��� ���)
    char flagPok[2]; //���� (������ � 䠩� ������⥫� ����樨 ��� ���)
    char flagOtm[2]; //���� (����� ������ �⬥�稪�) flagVrem
    char flagUdpost[2];//���� (�������� ����ﭭ�� ��⠢���饩)
    char flagBl_f[2];//���� (��ନ஢��� ���� ����-䠩�)
    char flagDat_vr[2];//����(�뤠���� � 䠩� ����/�६� ��� ���)
    char Nom_nab[3];//����� ����� �����樥�⮢ ��� �८�ࠧ������ �� ����� ���
    char flagFour[2];//����(������ �८�ࠧ������ ����)
    char flagFiltr[2];//����(�����஢��� �����?)
    char Nig_f[10];//������ ��ண �����
    char Verh_f[10];//���孨� ��ண �����
      struct stat statbuf;
      struct tm *gmt;      
	int obr_f_dan(void);   // �㭪�� ��ࠡ�⪨ 䠩�� ��⠭����
	void kol_kan(char* strok, int **mas1); //�㭪�� ��।������ ���-�� ������� � ���-�� �� ����७��
    //	void vrem_fun(int pozic, char* vnut_vrem);//�㭪�� �८�ࠧ������ ����� ����樨 � ⥪�饥 �६�

    int far *otmPoz;        //�����⥫� �� ���ᨢ ����権 ��⮪ �⬥�稪�
    float far *vib_dan; //�����⥫� �� ���ᨢ ������ �� ����樨
    float far *nGbm;    //�����⥫� �� ���ᨢ ���祭�� ����
    float far *nGbv;    //�����⥫� �� ���ᨢ ���祭�� ������� ����樨
    float far *argum;   //�����⥫� �� ���ᨢ 䠧 �� �८�ࠧ������ ����
    float far *aGv;
    float far *aGm;
    void S_Furie(int,float far *vn_vib,float far *vn_nGbm,float far *vn_nGbv,float far *vn_argum,float far *vn_aGv,float far *vn_aGm);

    FILE *fltfile; //���� ��䨫��஢����� ������ �� ����樨
    float f_done;  //������ ���� 䨫���
    float f_up;    //������ ���� 䨫���

    int kof_zap=100;
    int Nom_Otm;
    char varus[255];
    int k_met;

    int kol_metok(int far *massiv1); //�㭪�� ������ ���-�� ��⮪ �⬥�稪�, ���������� ���ᨢ� �����ﬨ �⬥�稪� 
    void a_p_f(int chanel,float far *vn_vib,int far *mass);//�㭪�� ��宦����� ����� ᮮ⢥�����饣� ���ᨬ. ������㤥 � ����⭮� �����
			    //� ������ ��������, ����� � ����� ��饭�� � 䠩�
    FILE *balfile; //���� � ������㤮�, ����ᮬ, ���⮩ ��饭��
  void main(int argc,char* argv[])
{
      clrscr();
	printf("\n\t\t�८�ࠧ������ ������ �� ����� ��� \n");
	strcpy(f_ust,"C:\\TC\\WORK2\\balans.txt"); //C:\\TC\\WORK2\\
	int osh=obr_f_dan();
	if(argc==3)
	{ sprintf(f_ust,"C:%sBIN\\%s",path,argv[2]);
          if(strrchr(f_ust,'.')==NULL)
	   {printf("�� 㪠���� ���७�� ��⠭���筮�� 䠩��!\n"); return;}
	  osh=obr_f_dan();
        }
	printf("\n\t�஢��� 䠩� ��⠭���� %s !!! \n",f_ust);
	if(osh==0) return;
    printf("\n������������ ��ࠡ��뢠����� 䠩�� [xxxxx]\n");
      int srav=stricmp(Name_f,"");
      if(srav==0) strcpy(Name_f,argv[1]);
    printf("%s\n",Name_f);
      if(strrchr(Name_f,'.')==NULL)
       {printf("�� 㪠���� ���७�� 䠩��!\n"); return;}
	fl=strlen(chans);
         Nom_kol=(int**)calloc(2,sizeof(int*));
         if(Nom_kol==NULL){printf("Out memory\n");return;}
        for(int q=0;q<2;q++)
	 {
	  Nom_kol[q]=(int*)calloc(fl,sizeof(int));
	  if(Nom_kol[q]==NULL){printf("Out memory\n");return;}
	 }

	kol_kan(chans,Nom_kol);

	printf("���-�� ��ࠡ��뢠���� �������\n");
	  if(fl<=1||fl>16){fl=1;}
	  printf("%d\n",fl);

	 printf("���-�� ����⮢ ����� �� ����� �����\n");
           Number=atoi(size);
	   printf("%d\n",Number);

         printf("����� ���� ��� �� ����� �����\n");
	   Chastota=atof(frec);
	   printf("%.4f\n",Chastota);

          int kk;
	  int Nabor=atoi(Nom_nab);
          switch(Nabor)
	     {
               case 1: for(kk=0;kk<16;kk++)
		       aKof[kk]=aKof1[kk];
		       break;
               case 2: for(kk=0;kk<16;kk++)
		       aKof[kk]=aKof2[kk];
		       break;
               case 3: for(kk=0;kk<16;kk++)
		       aKof[kk]=aKof3[kk];
		       break;
	       case 4: for(kk=0;kk<16;kk++)
		       aKof[kk]=aKof4[kk];
		       break;
	      default: break;
	     }

          sprintf(str1,"c:%sBIN\\%s",path,Name_f);
	    myfile=fopen(str1,"rb");
	    if(myfile==NULL){printf("�� ���� ������ 䠩�%35s\n",str1);}

	  strcpy(n_f_pol,Name_f); //����� ���७�� �� ����� 䠩��
	    int dl=strlen(n_f_pol);
	    dl=dl-4;
            strset(Name_f,'\0');
	    strncpy(Name_f,n_f_pol,dl);

	         stat(str1, &statbuf); //������� ���ଠ�� � 䠩��
    //           printf("������ � �����: %ld\n", statbuf.st_size);
    //	         printf("�६� ��᫥����� ���������: %s\n",ctime(&statbuf.st_ctime));
                 gmt=localtime(&statbuf.st_ctime);
    //	         printf("���:%d\n",gmt->tm_sec);

  do{      if (kbhit())  { if (getch()==27) return;} //��室 �� ESC
           Nom_bl=Nom_bl+1;
        if  (stricmp(flagBl_f,"Y")==0)
	    {sprintf(str2,"c:%sOSC\\%s.vrt",path,Name_f);}
	else{sprintf(str2,"c:%sOSC\\%s%d.vrt",path,Name_f,Nom_bl);}
	     printf("%35s\n",str2);
	   basefile=fopen(str2,"wt");
	   if(basefile==NULL){printf("�� ���� ������ 䠩�%35s\n",str2);}
	      aRes=(int**)farcalloc(fl,sizeof(int*));
	      if(aRes==NULL){printf("Out memory1\n");return;}
		  for(int qq=0;qq<fl;qq++)
		      {    
		       aRes[qq]=(int*)farcalloc(Number,sizeof(int));
		       if(aRes[qq]==NULL){printf("Out memory2\n");return;}
		       } 
	 unsigned long kolStr=0;//���-�� ��ப � dat-䠩��
       for(int k=0;k<fl;k++)
       {
	 for(int i=0;i<Number;i++)
	 {
	  fseek(myfile,2L*k+2L*fl*(i+tc_poz),SEEK_SET);
	  fread(&aRes[k][i],2,1,myfile);
	  kolStr=kolStr+1;
	  if(feof(myfile)){i=Number;konec_f=1;}
	 }
       }
	Number=kolStr/fl;  //����쪮 ����⮢ �� ������� ������ � ���. 䠩��
			   //�⮫쪮 ��ப � ����筮� 䠩��

        for(k=0;k<Number;k++)
       {
	 for(int i=0;i<fl;i++)
         { aRes[i][k]=aRes[i][k]-2048;
	   aRes[i][k]=aRes[i][k]&0xFFFFFFFE;
	 }
       }
          Nom_Otm=15;
         sscanf(flagOtm,"%x",Nom_Otm);
		int flag=0;float sumotm=0;int otmkof=0;//��ࠡ�⪠ �⬥�稪�
		 for(int h=0;h<Number;h++)
		   sumotm=sumotm+(float)aRes[Nom_Otm][h];
		 otmkof=sumotm/Number;
		 for(h=0;h<Number;h++)
		   aRes[Nom_Otm][h]=aRes[Nom_Otm][h]-otmkof;
		   unsigned char per_met=0;//���� ��ࢮ� ��⪨
                   k_met=0;                //���-�� ��⮪
	           int mark=0;
		  for(h=0;h<Number;h++)  //��宦����� ��ࢮ� ��⪨ �⬥�稪�
                  {    mark=mark-1;      //� ���-�� ��⮪
		     if (abs(aRes[Nom_Otm][h])>kof_zap && mark<5)
		     { if(per_met==0) 
		         {flag=h; per_met=1;k_met=k_met+1;mark=20;}
		       else {k_met=k_met+1;mark=20;}
                     }    
		  }
	       for(int kk=0;kk<fl;kk++)
               {  for(h=0;h<Number;h++)        //����஭����� � �⬥�稪��
		   aRes[kk][h]=aRes[kk][h+flag];
	       }	   
	       Number=Number-flag;
	 Pokaz=(float**)calloc(kolPok,sizeof(float*));    //�뤥����� ����� 
         if(Pokaz==NULL){printf("Out memory3\n");return;} //��� ������⥫�
	for(int qz=0;qz<kolPok;qz++)
	 {
	  Pokaz[qz]=(float*)calloc(fl,sizeof(float));
	  if(Pokaz[qz]==NULL){printf("Out memory4\n");return;}
	 }
          for(k=0;k<fl;k++)     //��宦����� ����ﭭ�� ��⠢���饩 �� ������� ������
          { for(int i=0;i<Number;i++)
	    Pokaz[0][k]=(aRes[k][i]*aKof[Nom_kol[0][k]])+Pokaz[0][k];
	    Pokaz[0][k]=(Pokaz[0][k]/Number)/(aKof[Nom_kol[0][k]]); 
	  }
      if(stricmp(flagUdpost,"Y")==0)
	 {for(k=0;k<fl;k++)     //�������� ����ﭭ�� ��⠢���饩 �� ������� ������
	   { for(int i=0;i<Number;i++)
	    aRes[k][i]=aRes[k][i]-Pokaz[0][k];
	   }
	 }
	   for( k=0;k<fl;k++)
	   { Pokaz[0][k]=0;
	     for(int i=1;i<Number-1;i++)
		 { if(aRes[k][i] > Pokaz[0][k]) Pokaz[1][k]=i;//����� ����樨 ���ᨬ��쭮�� ���祭��
		   Pokaz[0][k]=(aRes[k][i] > Pokaz[0][k]) ? aRes[k][i]:Pokaz[0][k];//���ᨬ.���祭�� ����樨
		   if(aRes[k][i] < Pokaz[2][k]) Pokaz[3][k]=i;//����� ����樨 �������쭮�� ���祭��
		   Pokaz[2][k]=(aRes[k][i] < Pokaz[2][k]) ? aRes[k][i]:Pokaz[2][k]; //�����.���祭�� ����樨
                 }
	   }
	  if(stricmp(flagDat_vr,"Y")==0)
	       {// fprintf(basefile," ���/�६�: %s",asctime(gmt));
		 fprintf(basefile," �६�: %.2d:%.2d:%.2d\t",gmt->tm_hour,gmt->tm_min,gmt->tm_sec);
		 fprintf(basefile," ���: %.2d.%.2d.%.4d\n",gmt->tm_mday,(gmt->tm_mon)+1,(gmt->tm_year)+1900);
	       }
	if(stricmp(flagPok,"Y")==0)
	 {  fprintf(basefile," ����.\t");//����� ���ᨬ. ���祭�� ����樨
	      for(int oo=0;oo<fl;oo++)
	       fprintf(basefile,"%.2f\t",(Pokaz[0][oo])*aKof[Nom_kol[0][oo]]);
	       fprintf(basefile,"\n");
	     fprintf(basefile," �६�\t");//����� �६��� ���ᨬ. ���祭�� ����樨
	      for(int o=0;o<fl;o++)           
	       {//vrem_fun(Pokaz[1][o],vremia);
		fprintf(basefile,"%7.4f\t",Pokaz[1][o]/Chastota);}
	        fprintf(basefile,"\n");
             fprintf(basefile," ���.\t");//����� �����. ���祭�� ����樨
	      for(oo=0;oo<fl;oo++)          
	       fprintf(basefile,"%.2f\t",(Pokaz[2][oo])*aKof[Nom_kol[0][oo]]);
		fprintf(basefile,"\n");
	     fprintf(basefile," �६�\t");//����� �६��� �����. ���祭�� �����
	      for(o=0;o<fl;o++)
	       {//vrem_fun(Pokaz[3][o],vremia);
               fprintf(basefile,"%7.4f\t",Pokaz[3][o]);}
		 fprintf(basefile,"\n");
	 }
       if(stricmp(flagVib,"Y")==0)
	{ fprintf(basefile," �६�\t"); //���������� ��������
	  for(int o=0;o<fl;o++)           //������� ��� ����묨
	    fprintf(basefile,"���.N%d\t",Nom_kol[0][o]);
	    fprintf(basefile,"\n");
	   for(k=0;k<Number;k++)
	  {  //vrem_fun(k,vremia);      //�८�ࠧ������ ����� ����樨 � ⥪�饥 �६�
	    fprintf(basefile,"%7.4f\t",k/Chastota);
	   for(int i=0;i<fl;i++)
	    {
	     if(i==fl-1) fprintf(basefile,"% .2f\n",((float)(aRes[i][k]))*aKof[Nom_kol[0][i]]);
	     else  fprintf(basefile,"% .2f\t",((float)(aRes[i][k]))*aKof[Nom_kol[0][i]]);
	    }
	  }
	}
	    tc_poz=Number+tc_poz; //�����祭�� ⥪�饩 ����樨 � 䠩��
	 for(qq=0;qq<fl;qq++)
          farfree (aRes[qq]);
	  farfree (aRes);
	 for(qz=0;qz<kolPok;qz++)
          free (Pokaz[qz]);
          free (Pokaz);   
	 fclose(basefile);
    } while(konec_f==0&&(stricmp(flagBl_f,"Y")!=0));//������ ���� �� �����
	 //䠩�� ��� �� ��⠭����� 䫠� �ନ஢���� ������ ����-䠩��
	 fclose(myfile);
   
/**********************************************************************/   
/************************�८�ࠧ������ ����**************************/
/**********************************************************************/
     if(stricmp(flagFour,"Y")==0)
     {  if(stricmp(flagFiltr,"Y")==0)
	{  printf("\t�८�ࠧ������ ���� � 䨫�����\n");
	   f_done=atof(Nig_f);
	   f_up=atof(Verh_f);
	   printf("������ �࠭�� 䨫��� %6.2f ��\n",f_done);
	   printf("������ �࠭�� 䨫��� %6.2f ��\n",f_up);
	}
	else printf("\t�८�ࠧ������ ����\n");
	 int riad=log(Number)*1.442695;//��室�� ���ᨬ��쭮 ��������� ���-�� 
	 Number=pow(2,riad);             //����⮢ ��� ࠧ������� � �� ����

          otmPoz=(int*)farcalloc(k_met,sizeof(int));
	   if(otmPoz==NULL){printf("Out memory\n");return;}
                                  //������ ���-�� ��⮪ �⬥�稪� �� 䠩��
	 k_met=kol_metok(otmPoz); //� ���������� ���ᨢ� �����ﬨ ��⮪ 

           
             
           
       for(int yy=0;yy<fl;yy++)
       {    
	     basefile=fopen(str2,"rt");
	     if(basefile==NULL){printf("�� ���� ������ 䠩�%35s\n",str2);}
	     vib_dan=(float*)farcalloc(Number,sizeof(float));
	   if(vib_dan==NULL){printf("Out memory\n");return;}	
	     
	     fgets(varus,250,basefile); //�⥭�� �� 䠩�� ��ࢮ� ��ப�
	 for(int ii=0;ii<Number;ii++)              //���������� ���ᨢ�
	  {  float vsper;                           //������ �� ����樨
	       for(int iii=0;iii<fl+1;iii++)
	       { fscanf(basefile,"%f",&vsper);
		 if((iii-1)==yy) vib_dan[ii]=vsper;
	       }                              
	  }
		if(yy==Nom_Otm){ fclose(basefile);farfree(vib_dan); break;}

		 nGbm=(float*)farcalloc(Number>>1,sizeof(float));
		if(nGbm==NULL){printf("Out memory\n");return;}
	     nGbv=(float*)farcalloc(Number>>1,sizeof(float));
	     if(nGbv==NULL){printf("Out memory\n");return;}
		argum=(float*)farcalloc(Number>>1,sizeof(float));
		if(argum==NULL){printf("Out memory\n");return;}

                sprintf(str1,"c:%sFURIE\\%s.f%02d",path,Name_f,Nom_kol[0][yy]);
		myfile=fopen(str1,"wt");
		if(myfile==NULL){printf("�� ���� ������ 䠩�%35s\n",str1);}
		printf("%35s\n",str1);
                  aGv=(float*)farcalloc(Number,sizeof(float));
	     if(aGv==NULL){printf("Out memory\n");return;}
	         aGm=(float*)farcalloc(Number,sizeof(float));
	         if(aGm==NULL){printf("Out memory\n");return;}
	       //����ணࠬ�� "�८�ࠧ������ ����"
	       S_Furie(Number,vib_dan,nGbm,nGbv,argum,aGv,aGm);

	       for(int d=0;d<(Number>>1);d++)
	       { fprintf(myfile,"%9.4f \t",nGbm[d]);
	         fprintf(myfile,"%9.4f \t",nGbv[d]);
		 fprintf(myfile,"%9.4f \n",argum[d]*(180/M_PI));
               }
	 fclose(myfile);
	 fclose(basefile); 
	 farfree(aGv);
	 farfree(aGm);
       if(stricmp(flagFiltr,"Y")==0)
	 { 
/*******************************************************************/
/****************���⭮� �८�ࠧ������ ���� � 䨫�����*********/
/*******************************************************************/
              
	      for(int flt=0;flt<(Number>>1);flt++)  //��������(���㫥���
	      {                                     //���祭�� ������� ��
	       if(nGbm[flt]<f_done||nGbm[flt]>f_up) //����, ����� �� �㦭�)
	       {nGbv[flt]=0;}
	      }
	  sprintf(str1,"c:%sFILTR\\%s.t%02d",path,Name_f,Nom_kol[0][yy]);
	  fltfile=fopen(str1,"wt");
	  if(fltfile==NULL){printf("�� ���� ������ 䠩�%35s\n",str1);}
	    printf("%35s\n",str1);
            int tec_otm=0;
	  for(int dd=0;dd<Number;dd++)
	   {
	     double summary=0;//���祭�� �������� ����樨 ��� ⥪�饣� �����
	      for(int ddd=1;ddd<(Number>>1);ddd++)
	      {
	       if(nGbv[ddd]!=0)
	       {summary=nGbv[ddd]*cos(2*M_PI*nGbm[ddd]*(dd/Chastota)+argum[ddd])+summary;}
	      }
	       fprintf(fltfile,"%9.4f \t",(float)dd/Chastota);
	       fprintf(fltfile,"%9.4f \t",(float)summary+(-nGbv[0]/2));
	       if(dd==otmPoz[tec_otm])
	       { tec_otm=tec_otm+1;
		 fprintf(fltfile,"%2d \n",(int)(((float)summary+(-nGbv[0]/2))*2));
	       }
	       else {fprintf(fltfile,"%2d \n",-1);}
	       vib_dan[dd]=(float)summary+(-nGbv[0]/2);
	   }
	   fclose(fltfile);
	  
	   farfree(nGbm);
	   farfree(nGbv);
	   farfree(argum);

	     a_p_f(yy,vib_dan,otmPoz);//�㭪�� ������㤠, �����, ���� ��饭��

	 } //����� "���⭮� �८�ࠧ������ ���� � 䨫�����"
	 else{ farfree(nGbm);
	       farfree(nGbv);
	       farfree(argum);
	     }
           farfree(vib_dan);
       } //����� 横�� "�⥭�� �� 䠩�� �� �������"
	farfree(otmPoz); 
 
     }//����� "�८�ࠧ������ ����" 
    // farfree(vib_dan);        
     for(q=0;q<2;q++)
     free (Nom_kol[q]);
     free (Nom_kol);
}
//-------------------------------------------------------------------------------

       int obr_f_dan(void)
       {
       sprintf(fdan,"%s",f_ust);       //  C:\\TC\\WORK2\\
      datfile=fopen(fdan,"rt");  //����⨥ ��⠭���筮�� 䠩��
      if(datfile==NULL)
	    {printf("�� ���� ������ 䠩�%35s\n",fdan);return 0;}
      char* df=NULL;

      c=0;
      for(int tf=0;tf<100;tf++)
     {
   do{
	 strcpy(pred,"");
         fldv=0;    //䫠� �� ������� ��襭
      do {
          df=fgets(t,2,datfile);
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
	 
	case 1: strcpy(Name_f,prom);		 break;
	case 2: strcpy(chans,prom);	 	 break;
	case 3: strcpy(frec,prom);               break;
	case 4: strcpy(size,prom);	         break;
	case 5: strcpy(path,prom); 	 	 break;
	case 6: strcpy(flagVib,prom); 	 	 break;
	case 7: strcpy(flagPok,prom); 	 	 break;
	case 8: strcpy(flagOtm,prom); 	 	 break;
	case 9: strcpy(flagUdpost,prom); 	 break;
	case 10: strcpy(flagBl_f,prom); 	 break;
	case 11: strcpy(flagDat_vr,prom); 	 break;
	case 12: strcpy(Nom_nab,prom); 	         break;
	case 13: strcpy(flagFour,prom); 	 break;
	case 14: strcpy(flagFiltr,prom); 	 break;
	case 15: strcpy(Nig_f,prom); 	         break;
	case 16: strcpy(Verh_f,prom); 	         break;
	default: break;
	     }
	   
        if (df==NULL) break;
	
      }  while (on!=0);
        if (df==NULL) break; 
     }
      if(stricmp(flagFour,"Y")==0) //�᫨ ��⠭����� 䫠� �८�ࠧ������
	    {                            //���� � ᮮ⢥�����騥 䫠�� 
		strcpy(flagVib,"Y"); 	 //��⠭���������� ��� ���뢠����	
		strcpy(flagPok,"N"); 	 	  
	        strcpy(flagUdpost,"Y"); 	 
	        strcpy(flagBl_f,"Y"); 	         
		strcpy(flagDat_vr,"N"); 	 
            }	
     fclose(datfile);
      return 1;
       } //����� ������� obr_f_dan
//-------------------------------------------------------------------------

     void kol_kan(char *strok, int **mas1)
     {
     char odsim[2];
     odsim[0]='\0';
     odsim[1]='\0';
     for(int tf=0;tf<fl;tf++)
     {
      memcpy(odsim,&strok[tf],1);      //�८�ࠧ������ ��ப� � ����ࠬ�
      sscanf(odsim,"%x",&mas1[0][tf]); //������� � ������� �᫠ � 
      strset(odsim,'\0');              //��࠭���� �� � ���ᨢ� mas1
     }
        int vrNom=0;
         for(tf=0;tf<fl;tf++)             //���������� ��ண� �⮫�� ���ᨢ�
         {                                //�⢥��饣� �� ������⢮ ����७��  
          vrNom=mas1[0][tf];               //������� ������
           int schNom=0;
           for(int ff=tf;ff<fl;ff++)
	    {if (vrNom==mas1[0][ff]&&mas1[1][ff]==0)
		 {schNom=schNom+1;mas1[1][ff]=-1;}
            }
          mas1[1][tf]=schNom;
         }     
     //	 for(tf=0;tf<fl;tf++)
     //	 printf("%+d\n",mas1[1][tf]);
     }   //����� ������� kol_kan
//---------------------------------------------------------------------
/*    void vrem_fun(int pozic, char* vnut_vrem)
    {    unsigned long b_time=gmt->tm_sec+(gmt->tm_min*60)+((unsigned long)gmt->tm_hour*(unsigned long)3600);
	 //b_time -�� �६� ᮧ����� 䠩�� ������
          strset(vnut_vrem,NULL);
          unsigned long t_time=0; unsigned long hour=0; unsigned long mn=0; float sec=0;
	  t_time=(tc_poz+pozic)/Chastota;  //��ॢ�� �६��� �� ᥪ㭤 � ���, ������, ᥪ㭤�
	  int izb_pozic=(tc_poz+pozic)-(t_time*Chastota); //��宦����� �஡��� ���
	  float dol_sec=izb_pozic/Chastota;      //ᥪ㭤
          if(stricmp(flagVrem,"Y")!=0) t_time=t_time+b_time;//����� ���-�� ᥪ㭤(����� � ⥪��. �६����)
	  hour=t_time/3600;   //���-�� �ᮢ
	  mn=(t_time-hour*3600)/60;  //���-�� �����
	  unsigned long c_sec=(t_time-3600*hour)-60*mn;//楫�� �᫮ ᥪ㭤
	  sec=c_sec+dol_sec;       //��饥 �᫮ ᥪ㭤
          float ob_time=t_time+dol_sec;// ��饥 ⥪�饥 �६�
	  float ob_vrem=0;    //����筮� �६� �����
	   if(stricmp(flagVrem,"Y")!=0) ob_vrem=(Number/Chastota)+b_time;
           else ob_vrem=Number/Chastota;
          if(stricmp(flagVrem,"Y")!=0&&hour>24) hour%=hour/24;//��᢮���� ���⪠ �⤥�����
	  if(ob_vrem>60)
	       { sprintf(vnut_vrem,"%2ld:%2ld:%7.4f",hour,mn,sec);}
	  else {sprintf(vnut_vrem,"%7.4f",ob_time);}
    }   */

//--------------------------------------------------------
//   ����஥ �८�ࠧ������ ����
//--------------------------------------------------------
void S_Furie(int Num,float far *vn_vib,float far *vn_nGbm,float far *vn_nGbv,float far *vn_argum,float far *vn_aGv,float far *vn_aGm)
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
	float dT =(float)1/Chastota;
	double SumKv;
        double arctan;
		Koeff=1.0;
     //	float huge aGv[Num];
     //	float huge aGm[Num];
	 
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
				vn_aGv[ip]=Koeff*(vn_vib[ni]+vn_vib[ni+nSt]);
				vn_aGv[ip+nSt]=Koeff*(vn_vib[ni]-vn_vib[ni+nSt]);
				vn_aGm[ip]=0;
				vn_aGm[ip+nSt]=0;
				}
				else
				{
				nGtv=vn_aGv[ni]+nV*vn_aGv[ni+nSt]+nM*vn_aGm[ni+nSt];
				vn_nGbv[ip]=vn_aGv[ni]-nV*vn_aGv[ni+nSt]-nM*vn_aGm[ni+nSt];
				nGtm=vn_aGm[ni]+nV*vn_aGm[ni+nSt]-nM*vn_aGv[ni+nSt];
				vn_nGbm[ip]=vn_aGm[ni]-nV*vn_aGm[ni+nSt]+nM*vn_aGv[ni+nSt];
				vn_aGv[ip]=nGtv;
				vn_aGm[ip]=nGtm;
				}
				ip++;
			}
		}
		if (i!=1)
		{
			for(m=0;m<(Num>>1);m++)
			{
				vn_aGv[m+(Num>>1)]=vn_nGbv[m];
				vn_aGm[m+(Num>>1)]=vn_nGbm[m];
			}
		}
	}
	i=0;
	vn_nGbv[i]=vn_aGv[i]/Num;
	vn_nGbm[i]=0;
	for (i=1;i<(Num>>1);i++)
	{
		SumKv=sqrt((vn_aGv[i]/Num)*(vn_aGv[i]/Num)+(vn_aGm[i]/Num)*(vn_aGm[i]/Num));
		vn_nGbv[i]=2.0*(float)SumKv;
		vn_nGbm[i]=i/(dT*Num);
		arctan=atan(vn_aGm[i]/vn_aGv[i]);
		vn_argum[i]=(float)arctan;//*(180/M_PI);
		if(vn_aGv[i]<0){vn_argum[i]=vn_argum[i]+M_PI;}
		if(vn_argum[i]<0){vn_argum[i]=vn_argum[i]+(2*M_PI);}
	}

	return;
}
//---------------------------------------------------------------------------
     int kol_metok(int far *massiv1)
     {  basefile=fopen(str2,"rt");
	   if(basefile==NULL){printf("�� ���� ������ 䠩�%35s\n",str2);}
	   fgets(varus,250,basefile);  //�⥭�� ��ࢮ� ��ப� 䠩��
             int vn_k_met=0;          //���-�� ��⮪
	     int mark=0;
	   for(int ii=0;ii<Number;ii++)              
	  {  float vsper;                           
	       for(int iii=0;iii<fl+1;iii++)
	       { fscanf(basefile,"%f",&vsper);//�⥭�� ⥪�饩 ����樨 � 䠩��
		 if((iii-1)==Nom_Otm) //�᫨ ᮮ⢥����� ������ �⬥�稪� �믮����� ������
		  {  //������ ���-�� ��⮪ �⬥�稪� � ���������� ���ᨢ� �����ﬨ ��⮪
		     mark=mark-1;
		     if (fabs(vsper)>kof_zap*aKof[Nom_Otm] && mark<5)
		     {massiv1[vn_k_met]=ii; vn_k_met=vn_k_met+1; mark=20;}
		  }
	       }                              
	  }
         fclose(basefile);
         return vn_k_met;
     }     
//-------------------------------------------------------------------------
    void a_p_f(int chanel,float far *vn_vib,int far *mass)
      {
       sprintf(str1,"c:%sBALANSIR\\%s.b%02d",path,Name_f,Nom_kol[0][chanel]);
       balfile=fopen(str1,"wt");
        if(balfile==NULL){printf("�� ���� ������ 䠩�%35s\n",str1);}
	 printf("%40s\n",str1);
	 int tec_otm=0;
	 float maxil=0;
	 int n_max=0;
	 double n_pol;
         double polus_d;
         int polus;
	 for(int tt=0;tt<Number;tt++) 
	  {   tec_otm=tec_otm+1;
	       maxil=0;
	       n_max=0;
	    do{
                 n_max=(vn_vib[tt] > maxil) ? (tt+1):n_max;
		 maxil=(vn_vib[tt] > maxil) ? vn_vib[tt]:maxil;
	        tt=tt+1;	 
	      } while(tt!=mass[tec_otm]);
	       n_pol=((double)(n_max-mass[tec_otm-1])/(double)(mass[tec_otm]-mass[tec_otm-1]))*96;//����� ����� �� 1 �� 96
	       n_pol=96-(n_pol-1);
	       polus_d=floor(n_pol+(double)0.5);
	       polus=polus_d;
	       if(polus>96) polus=1;
	       if(polus==96) polus=1;
	       else polus=polus+1;
	       fprintf(balfile,"%2d \t",polus);
	       fprintf(balfile,"%9.4f \t",maxil);
	       fprintf(balfile,"%9.4f \n",1/(((float)(mass[tec_otm]-mass[tec_otm-1]))/Chastota));
             tt=tt-1;
	     if(tec_otm==k_met-1)  break;
	  }

       fclose(balfile);
       return;
      }