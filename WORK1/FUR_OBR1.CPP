//-------------------------------------------------------------------------
//�ணࠬ�� ���筮� ��ࠡ�⪨ ����樨 �� ����(�����।������ 䠩��� (⥯��� �
//����� 䠩�� ���� 6 ���०���� ������ ������))
//-------------------------------------------------------------------------
#include <stdio.h>
#include <conio.h>
#include <dir.h>
#include <string.h>
#include <alloc.h>
#include <stdlib.h>
const int ch_reg=6;
const int CN=16;
const int Number=4096;
const int Num2=2048;
char kk;
float sum;
float koof;
float stepp;
float* aRes;
float* Chast;
FILE *myfile;
	FILE *basefile;
	char str[40];
	char rty[40];
	char unic_name [8];
	char O_name [5];
	char fales[15];
	char vrvs[15];
	char fls[4];
	char vosk[5];
	char GA_num[5];
	char* buffer;
	int fl;
void main(void)
{  //1
 clrscr();

  mkdir( "fur_obr" );
       aRes=(float*)calloc(Number,sizeof(float));
        if(aRes==NULL){printf("Out memory\n");return;}
       Chast=(float*)calloc(Number,sizeof(float));
        if(Chast==NULL){printf("Out memory\n");return;}
      do
       {  //3
	clrscr();
	printf("              ���筠� ��ࠡ�⪠ ������ �� ����\n");
	printf("\n");
	 printf(" ��室 �� ESC ��� �த������\n");
         int esc=getch();
	 if(esc==27) return;

         printf(" ������ ����� �� (1-10)\n");
	 GA_num[0]=3;
	 cgets(GA_num);
	 strcpy(GA_num,&GA_num[2]);
	 int ga=atoi(GA_num);

          printf("\n������: 1)������ 㧫�� 2)������ ����\n");
	 printf(" ������ ���� 1 ��� 2 �� 㬮�砭�� 1\n");
	  fls[0]=2;
	 cgets(fls);
	 strcpy(fls,&fls[2]);
	 fl=atoi(fls);
	 if(fl!=2){fl=1;}
         else{fl=2;}

     printf("\n������ 㭨���쭮�  ������������ ��ࠡ��뢠���� 䠩��� [xxxxx]\n");
    unic_name[0]=6;
    buffer=cgets(unic_name);
    strcpy(unic_name,buffer);

   printf("\n ������ ������������ 䠩�� ��᫥ ��ࠡ�⪨  [xx]\n");
     printf("��������㥬� �����: G[x]-�������� ०��,V[x]-����㦤����,\n");
     printf("   X[x]-宫��⮩ 室,B[x]-�롥�,S[x]-०�� ��,��� [x]-�� ᨬ���\n");
      char* buf;
      O_name[0]=3;
      buf=cgets(O_name);
      strcpy(O_name,buf);

        printf("\n � ������ ������ ��ࠡ��뢠��?(�� 1 �� 16) [xx]\n");
   printf(" �� 㬮�砭�� ࠢ�� 1\n");
	 vosk[0]=3;
	 cgets(vosk);
	 strcpy(vosk,&vosk[2]);
	 int cn2=atoi(vosk);
	 if(cn2<17&&cn2>0) cn2=cn2;
	 else cn2=1;

       printf("\n");
	 for(int i=cn2-1;i<CN;i++)   //CN-�᫮ �������
	  {   //4
	    if (kbhit())  { if (getch()==27) break;} //��室 �� ESC
	   sprintf(rty,"c:\\DIAGR\\fur_obr\\%2s_%5s.v%02i",O_name,unic_name,i+1);
           printf("%35s\n",rty);
	   basefile=fopen(rty,"wt");
	    if(basefile==NULL)  printf("�� ���� ������ 䠩�%35s\n",rty);

	      fprintf(basefile,"�� %d\t",ga);
              switch(fl)
                {
	  case 1:  fprintf(basefile,"������ ������ 㧫��\t");
		   break;
	  case 2:  fprintf(basefile,"������ ᯨ��� ����\t");
		   break;
	  default:  fprintf(basefile,"             \t");
		}

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
			  
	   sprintf(fales,"%2s_%5sv%02i",O_name,unic_name,i+1); //��ନ஢���� ����� 䠩�� ��� ��।�� ��� � Excel
	   fprintf(basefile,"%- 12s \t",fales);
	   fprintf(basefile,"����� N\t%d\n",i+1);

	  for(int j=0;j<ch_reg;j++)    //ch_reg-�᫮ ���०����
	   {  //5
	    sprintf(str,"c:\\DIAGNOST\\furie\\%5s_%02i.f%02i",unic_name,j+1,i+1);
	    myfile=fopen(str,"rt");
	    if(myfile==NULL)
	     {
	      printf("�� ���� ������ 䠩�%35s\n",str);
	     }
	      else
	      {
	       float vspom=0;
	       fgets(vrvs,15,myfile);
	       for(int k=0;k<Number;k++)
	       {
		fscanf(myfile,"%f",&vspom);
//		fscanf(myfile,"%f.5",&Chast[k]);
		Chast[k]=vspom;
		fscanf(myfile,"%f",&aRes[k]);
		 }
                 if(j>0) fprintf(basefile,"���०�� N%-d\n",j+1);
		   else {
		         fprintf(basefile,"���०�� N%-d\t",j+1);
			 fprintf(basefile,"%9s",vrvs);
			}
		  for(int d=0;d<(Num2+32);d++)
		   {
		    fprintf(basefile,"% .4f \t",Chast[d]);
		     fprintf(basefile,"% .4f \n",aRes[d]);
		   }
		}
                fclose(myfile);
	
           }  //5
          fclose(basefile);
       	  }    //4
       printf("�த������ ? [y/n]\n");
       kk='';
   scanf("%c",&kk);
  }  //3
 while ((kk!='n')&&(kk!='N'));
 free(aRes);
 free(Chast);
}  //1
