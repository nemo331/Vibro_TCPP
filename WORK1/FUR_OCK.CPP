//-------------------------------------------------------------------------
//�ணࠬ�� �業�� ����樨 �� ����(�� �᭮��� ��ମ����� � ���⠬ ,
//�����।������ 䠩��� (⥯��� � ����� 䠩�� ���� 6 ���०���� ������ ������))
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
const int Number=4096;
const int Num2=2048;
float sum;
float koof;
float stepp;
float* aRes;
float* Chast;
char** occh;
FILE *myfile;
	FILE *basefile;
	char str[40];
	char rty[40];
	char unic_name [8];
	char O_name [5];
	char falek[15];
        char vrvd[15];
	int los;
	float garm[]={1.04,2.08,3.12,4.16,8.32,12.48,16.64,20.8,24.96,33.28,49.92,99.84};
	float sch[12];
	int gr[]={1,2,3,4,8,12,16,20,24,32,50,100};
	int diap[]={16,16,16,16,30,40,60,60,60,60,60,120};

void main(int argc,char* argv[])
{  //1
 clrscr();
  mkdir( "fur_ock" );
       aRes=(float*)calloc(Number,sizeof(float));
        if(aRes==NULL){printf("Out memory\n");return;}
       Chast=(float*)calloc(Number,sizeof(float));
	if(Chast==NULL){printf("Out memory\n");return;}
       occh=(char**)calloc(12,sizeof(char*));
	 if(occh==NULL){printf("Out memory\n");return;}
	   for(int q=0;q<12;q++)
	    {
	     occh[q]=(char*)calloc(7,sizeof(char));
	     if(occh[q]==NULL){printf("Out memory\n");return;}
	    }

	clrscr();
	printf("\n\t\t�業�� ������ �� ����\n");
	printf("\n");
 /*	 printf(" ��室 �� ESC ��� �த������\n");
         int esc=getch();
	 if(esc==27) return;   */

          printf(" ����� �� (1-10)\n");
	 int ga=atoi(argv[1]);
         printf("%d\n",ga);


          printf("\n �業�� ����樨 ��: 16)����� 㧫�� 0)������ ����\n");
	 printf("��᫮ �� 0 �� 16 ����砥� �� ������ ������(�����⥫쭮) \n");
	 printf("������ �業������� ��� ��� ������ 㧫��, �� ��⠢���� ������� -  \n");
	 printf("��� ��� ᯨ��� ���� (�� 㬮�砭�� 0)\n");
   /*	 fls[0]=3;
	 cgets(fls);
	 strcpy(fls,&fls[2]);  */
	 los=atoi(argv[2]);
	 if(los<=0||los>16){los=0;}
	 printf("%d\n",los);

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
	   sprintf(rty,"c:\\DIAGR\\fur_ock\\%2s_%5s.c%02i",O_name,unic_name,i+1);
             printf("%35s\n",rty);
	   basefile=fopen(rty,"wt");
	    if(basefile==NULL) printf("�� ���� ������ 䠩�%35s\n",rty);
             fprintf(basefile,"�� %d\t",ga);
   /*            switch(los)
                {
	  case 1:  fprintf(basefile,"������ ������ 㧫��\t");
		   break;
	  case 2:  fprintf(basefile,"������ ᯨ��� ����\t");
		   break;
	  default:  fprintf(basefile,"             \t");
		} */

	  if(i+1<=los) fprintf(basefile,"������ ������ 㧫��\t");
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

	     sprintf(falek,"%2s_%5sc%02i",O_name,unic_name,i+1); //��ନ஢���� ����� 䠩�� ��� ��।�� ��� � Excel
	   fprintf(basefile,"%- 12s \t",falek);
	   fprintf(basefile,"����� N\t%d\n",i+1);

	  for(int j=0;j<ch_reg;j++)    //ch_reg-�᫮ ���०����
	   {  //5
	    sprintf(str,"c:\\DIAGNOST\\furie\\%5s_%02i.f%02i",unic_name,j+1,i+1);
	    myfile=fopen(str,"rt");
	    if(myfile==NULL) printf("�� ���� ������ 䠩�%35s\n",str);
	      else
	      {
	       float vspom=0;
	       fgets(vrvd,15,myfile);
	       for(int k=0;k<Number;k++)
	         {
		fscanf(myfile,"%f",&vspom);
		Chast[k]=vspom;
		fscanf(myfile,"%f",&aRes[k]);
		 }
                 float maxi;
	       for(k=0;k<12;k++)
	       { maxi=0;
	       for(int gf=0;gf<Number;gf++)
		{  if(Chast[gf]<=garm[k]&&Chast[gf+1]>garm[k])
		      {  for(int zz=gf-(diap[k]/2);zz<gf+(diap[k]/2);zz++)
			 maxi=(aRes[zz] > maxi) ? aRes[zz]:maxi;
			 sch[k]=maxi;} 
		 }
		}

                   int d;
               if(i+1<=los)
		    {
	           for(d=0;d<10;d++)
		   {
		   if(sch[d]<(-45.65*log(garm[d])+195)) sprintf(occh[d],"���.");//�業�� ����樨 �� ������ ����
		  else
		   {
		   if ((-45.65*log(garm[d])+195)<0)  sprintf(occh[d],"��.");
                   else sprintf(occh[d],"�����.");
		   }
	  if(sch[d]<(-34.00*log(garm[d])+150)) sprintf(occh[d],"�.");
	   if(sch[d]<(-23.129*log(garm[d])+100)) sprintf(occh[d],"��.");
	     if(sch[d]<(-16.403*log(garm[d])+53)) sprintf(occh[d],"��.");
		   }
                   }
		else
	       {  for(int d=0;d<3;d++)
		   {
		    if(sch[d]>80) sprintf(occh[d],"���.");
		    else  sprintf(occh[d],"�.");
		   }
		   for(d=3;d<10;d++)
		   sprintf(occh[d],"     ");

                   }
		   for(d=10;d<12;d++)
		   {
		   if(sch[d]<30) sprintf(occh[d],"�.");
		   else sprintf(occh[d],"���.");
		   }

                    if(j>0) fprintf(basefile,"���०�� N%-d\n",j+1);
		   else {
		         fprintf(basefile,"���०�� N%-d\t",j+1);
			 fprintf(basefile,"%9s",vrvd);
			}

		  fprintf(basefile,"N ���.\t");
		  fprintf(basefile,"������\t");
                  fprintf(basefile,"�業��\n");
		  for( d=0;d<12;d++)
		     {
		     fprintf(basefile,"%d \t",gr[d]);
		     fprintf(basefile,"% .4f \t",sch[d]);
                     fprintf(basefile,"% 6s\n",occh[d]);
		     }
	      }
		fclose(myfile);

	   }  //5
          fclose(basefile);
       	  }    //4

 free(aRes);
 free(Chast);
  for(q=0;q<12;q++)
 free (occh[q]);
 free (occh);
}  //1



