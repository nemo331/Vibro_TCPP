//-------------------------------------------------------------------------
//�ணࠬ�� ���筮� ��ࠡ�⪨ ����樨( �������� ����ﭭ�� ��⠢���饩,
//�������� 誠�� �६��� � �⬥�稪�, �����।������ 䠩��� (⥯��� �
//����� 䠩�� ���� 6 ���०���� ������ ������))
//-------------------------------------------------------------------------
#include <stdio.h>
#include <conio.h>
#include <dir.h>
#include <string.h>
#include <alloc.h>
#include <stdlib.h>
#include <math.h>

const int ch_reg=6;
const int CN=16;
const int Number=6144;
const int Num2=2048;
char ocenk[6];
float kof_st=1.0;//�����樥�� ��� �������� 誠�� �६��� � �⬥�稪�
float sum;
float sum_skl;
float koof;
float stepp;
int kof_zap=3;
int skl_sr=31;
float* aRes;
float* Otm;
float* r_Sksr;
        FILE *myfile;
	FILE *basefile;
        FILE *otmfile;
	char str[40];
	char rty[40];
	char otme[40];
	char fale[13];
	char vrvs[15];
	char vrnen[15];

	char unic_name [8];
	char O_name [5];

void main(int argc,char* argv[])
{  //1
 clrscr();
 
  mkdir( "obr_rg" );
       aRes=(float*)calloc(Number,sizeof(float));
       if(aRes==NULL){printf("Out memory\n");return;}

	Otm=(float*)calloc(Num2,sizeof(float));
	if(Otm==NULL){printf("Out memory\n");return;}

        r_Sksr=(float*)calloc(Num2,sizeof(float));
         if(r_Sksr==NULL){printf("Out memory\n");return;}

	clrscr();
	printf("\n\t\t���筠� ��ࠡ�⪠ ������ �� ����樨\n");
   /*	 printf(" ��室 �� ESC ��� �த������\n");
         int esc=getch();
	 if(esc==27) return;  */

	  printf(" ����� �� (1-10)\n");
	 int ga=atoi(argv[1]);
         printf("%d\n",ga);

 //	  printf("\n�業�� ����樨 ��: 1)����� 㧫�� 2)������ ����\n");

         printf("\n �業�� ����樨 ��: 16)����� 㧫�� 0)������ ����\n");
	 printf("��᫮ �� 0 �� 16 ����砥� �� ������ ������(�����⥫쭮) \n");
	 printf("������ �業������� ��� ��� ������ 㧫��, �� ��⠢���� ������� -  \n");
	 printf("��� ��� ᯨ��� ���� (�� 㬮�砭�� 0)\n");
	 int fl=atoi(argv[2]);
	 if(fl<=0||fl>16){fl=0;}
         printf("%d\n",fl);

	    


  printf("������쭮�  ������������ ��ࠡ��뢠���� 䠩��� \n");
    strcpy(unic_name,argv[3]);
    printf("%s\n",unic_name);

   printf("������������ 䠩�� ��᫥ ��ࠡ�⪨ \n");
//   printf("��������㥬� �����: G[x]-�������� ०��,V[x]-����㦤����,\n");
//   printf("   X[x]-宫��⮩ 室,B[x]-�롥�,S[x]-०�� ��,��� [x]-�� ᨬ���\n");
      strcpy(O_name,argv[4]);
      printf("%s\n",O_name);

           printf("\n");

	 for(int i=0;i<CN;i++)   //CN-�᫮ �������
	  {   //4
	   if (kbhit())  { if (getch()==27) break;} //��室 �� ESC
           sprintf(rty,"c:\\DIAGR\\obr_rg\\%2s_%5s.r%02i",O_name,unic_name,i+1);
	 
           printf("%35s\n",rty);
	   basefile=fopen(rty,"wt");
	    if(basefile==NULL) printf("�� ���� ������ 䠩�%35s\n",rty);

	    fprintf(basefile,"�� %d\t",ga);
      /*        switch(fl)
                {
	  case 1:  fprintf(basefile,"������ ������ 㧫��\t");
		   break;
	  case 2:  fprintf(basefile,"������ ᯨ��� ����\t");
		   break;
	  default:  fprintf(basefile,"             \t");
		}              */

          if(i+1<=fl) fprintf(basefile,"������ ������ 㧫��\t");
	  else  fprintf(basefile,"������ ᯨ��� ����\t");

          switch(O_name[0])
		{
	  case 'G':
	  case 'g':  fprintf(basefile,"�������� ०��\n");
		   break;
	  case 'V':
	  case 'v':  fprintf(basefile,"�����⮩ 室 � ����㦤�����\n");
		   break;
	  case 'X':
	  case 'x':  fprintf(basefile,"�����⮩ 室 ��� ����㦤����\n");
		   break;
	  case 'B':
	  case 'b':  fprintf(basefile,"�롥�\n");
		    break;
	  case 'S':
	  case 's':  fprintf(basefile,"����� ��\n");
		   break;
	  default:  fprintf(basefile,"             \n");
		}


	   sprintf(fale,"%2s_%5sr%02i",O_name,unic_name,i+1); //��ନ஢���� ����� 䠩�� ��� ��।�� ��� � Excel
	   fprintf(basefile,"%- 12s \t",fale);
	   fprintf(basefile,"����� N\t%d\n",i+1);

	  for(int j=0;j<ch_reg;j++)    //ch_reg-�᫮ ���०����
	   {  //5
	    sprintf(str,"c:\\DIAGNOST\\oscill\\%5s_%02i.o%02i",unic_name,j+1,i+1);
	    myfile=fopen(str,"rt");
	    if(myfile==NULL)
	     {
	      printf("�� ���� ������ 䠩�%35s\n",str);
	     }
	    else
	      {//10
	    sprintf(otme,"c:\\DIAGNOST\\oscill\\%5s_%02i.o16",unic_name,j+1);
	     if(str!=otme)
	     {
	    otmfile=fopen(otme,"rt");
	    if(otmfile==NULL){printf("�� ���� ������ 䠩�%35s\n",otme);}
             }
	       float vspom;
	       float sum=0;
	       float vr;
	       fgets(vrvs,15,myfile);
//	       fscanf(myfile,"%s",&vrvs);
	       for(int k=0;k<Number;k++)    //Number-�᫮ 蠣�� ���� ���
		{//6
		 fscanf(myfile,"%f",&vspom);
		 fscanf(myfile,"%f",&aRes[k]);
		 if(k==1) {stepp=vspom*kof_st;}
		 sum=aRes[k]+sum;
		} //6
		if(str!=otme){ fscanf(otmfile,"%s",&vrnen);}
	       if(str==otme){ fscanf(myfile,"%s",&vrnen);}
		for(int z=0;z<Num2;z++)
		 {
	       if(str!=otme){ fscanf(otmfile,"%f %f",&vr,&Otm[z]);}
	       if(str==otme){ fscanf(myfile,"%f %f",&vr,&Otm[z]);}
		}
		 koof=sum/Number;
		 float maxil=0;
		 float minil=0;
		 for(int r=0;r<Number;r++)
		 aRes[r]=aRes[r]-koof;
		 for(r=0;r<Num2;r++)
                   {
		   maxil=(aRes[r] > maxil) ? aRes[r]:maxil;
		   minil=(aRes[r] < minil) ? aRes[r]:minil;
                   }

		int flag=0;               //��ࠡ�⪠ �⬥�稪�
		float sumotm=0;
                float otmkof=0;
		 for(int h=0;h<Num2;h++)
		   sumotm=sumotm+fabs(Otm[h]);
		 otmkof=sumotm/Num2;
		  for(h=0;h<Num2;h++)
		     if ((fabs(Otm[h])-otmkof)>kof_zap){flag=h;h=Num2;}
		   for(h=0;h<Num2;h++)
                      {
		      if ((Otm[h]-otmkof)>kof_zap) Otm[h]=maxil/1.42;
		      if ((Otm[h]+otmkof)<(kof_zap*(-1))) Otm[h]=minil/1.42;
		      }
		   for(h=0;h<Num2;h++)
		       if ((fabs(Otm[h])-otmkof)<=kof_zap) Otm[h]=-1;

     //�業����� ������
                 float maxi=0;
		float mini=0;
                float max_vibr=0;
		for( k=0;k<Num2;k++)
		  {maxi=(aRes[k] > maxi) ? aRes[k]:maxi;} //��宦����� ���祭�� ᠬ��� ��᮪��� ����
		for( k=0;k<Num2;k++)
		  {mini=(aRes[k] < mini) ? aRes[k]:mini;} //��宦����� ���祭�� ᠬ�� ��㡮��� �������
		  max_vibr=fabs(maxi)+fabs(mini); //��।������ ����. ���祭�� ����樨

                int w;
		for( k=0;k<Num2;k++)   //��宦����� ᪮���饣� �।���� �� 31-���
		{
		 sum_skl=0;
		 for(w=k;w<(skl_sr+k);w++)
		  {sum_skl=aRes[w]+sum_skl;}
		   r_Sksr[k]=sum_skl/skl_sr;
		}

                float maxim=0;
		float minim=0;
                float maxsr_vibr=0;
		for( k=0;k<Num2;k++)
		  {maxim=(r_Sksr[k] > maxim) ? r_Sksr[k]:maxim;} //��宦����� ���祭�� ᠬ��� ��᮪��� ����
                for( k=0;k<Num2;k++)
		  {minim=(r_Sksr[k] < minim) ? r_Sksr[k]:minim;} //��宦����� ���祭�� ᠬ�� ��㡮��� �������
		  maxsr_vibr=fabs(maxim)+fabs(minim); //��।������ ����.�।���� ���祭�� ����樨

		for(h=0;h<Num2;h++)        //����஭����� � �⬥�稪��
		 {
		  aRes[h]=aRes[h+flag];
		  if((h+flag)<Num2){Otm[h]=Otm[h+flag];}
		  else{Otm[h]=-1;}
		  }

                 if(i+1<=fl)
		 {
	          if(maxsr_vibr<180){strcpy(ocenk,"���");}//�業�� �।��� ����樨
                  else{strcpy(ocenk,"�����");}
		  if(maxsr_vibr<140){strcpy(ocenk,"�");}
                  if(maxsr_vibr<100){strcpy(ocenk,"��");} 
		  if(maxsr_vibr<50){strcpy(ocenk,"��");}
                  }
	     else { if(maxsr_vibr<80){strcpy(ocenk,"�");}//�業�� �।��� ����樨
		  else{strcpy(ocenk,"���");}
		  }

		 if(j>0) fprintf(basefile,"���०�� N%-d\n",j+1);
		   else {
		         fprintf(basefile,"���०�� N%-d\t",j+1);
			 fprintf(basefile,"%9s",vrvs);
			}

                   fprintf(basefile," ��.���.  \t");
		      fprintf(basefile," ����.���.  \t");
		       fprintf(basefile," �業��  \n");
			fprintf(basefile,"%- .1f ��� \t",maxsr_vibr);
			 fprintf(basefile,"%- .1f ��� \t",max_vibr);
			  fprintf(basefile,"%- 6s \n",ocenk);
		  for(int d=0;d<1157;d++)
		   {
		    fprintf(basefile,"%- 4.4f \t",stepp*d);
		     fprintf(basefile,"%- .4f \t",aRes[d]);
		      fprintf(basefile,"%- .4f \n",Otm[d]);
		   }
                   
               } //10 
	        fclose(myfile);
		fclose(otmfile);
           }  //5
          fclose(basefile);
       	  }    //4
     
 free(aRes);
 free(r_Sksr);
 free(Otm);
}  //1