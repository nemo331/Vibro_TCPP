//-------------------------------------------------------------------------
//Программа оценки вибрации по Фурье(по основным гармоникам и частотам ,
//перераспределение файлов (теперь в одном файле будут 6 подрежимов одного канала))
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
char kk;
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
	char unic_name [6];
	char O_name [3];
	char* buffer;
	char ls[2];
	char fols[3];
	int los;
        int cn1;
	float garm[]={1.04,2.08,3.12,4.16,8.32,12.48,16.64,20.8,24.96,33.28,49.92,99.84};
	float sch[12];
        int gr[]={1,2,3,4,8,12,16,20,24,32,50,100};
	int ok[12];
	char* c_buf;
	char* bf;


void main(void)
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

      do
       {  //3
	clrscr();
	printf("              Оценка данных по Фурье\n");
	printf("\n");
	 printf(" Выход по ESC или продолжать\n");
         int esc=getch();
	 if(esc==27) return;

         printf("Оценивать вибрацию: 1)Опорные узлы 2)Спинка статора\n");
	 printf(" Введите цифру 1 или 2 по умолчанию 1\n");
	 ls[0]=2;
	 bf=cgets(ls);
	 strcpy(ls,bf);

    printf("\n Введите уникальное  наименование обрабатываемых файлов [xxxxx]\n");
    unic_name[0]=6;
    buffer=cgets(unic_name);
    strcpy(unic_name,buffer);

    printf("\n Введите наименование файла после обработки  [xx]\n");
     do
      {char* buf;
      O_name[0]=3;
      buf=cgets(O_name);
      strcpy(O_name,buf);}
      while(O_name[0]==0);
       printf("\n");

   printf(" С какого канала обрабатывать?(от 1 до 16) [xx]\n");
   printf(" По умолчанию равно 1\n");
         fols[0]=3;
	 c_buf=cgets(fols);
	 strcpy(fols,c_buf);
	 cn1=atoi(fols);
	 if(cn1<17&&cn1>0) cn1=cn1;
	 else cn1=1;

         los=atoi(ls);
	 if(los!=2){los=1;}
	 else{los=2;}
	   printf("\n");
	     
	 for(int i=cn1-1;i<CN;i++)   //CN-число каналов
	  {   //4
	  if (kbhit())  { if (getch()==27) break;} //выход по ESC
	   sprintf(rty,"c:\\TC\\WORK\\fur_ock\\%2s_%5s.c%02i",O_name,unic_name,i+1);
           printf("%35s\n",rty);
	   basefile=fopen(rty,"wt");
	    if(basefile==NULL) printf("Не могу открыть файл%35s\n",rty);

               switch(los)
                {
	  case 1:  fprintf(basefile,"Опорные узлы\n");
		   break;
	  case 2:  fprintf(basefile,"Спинка статора\n");
		   break;
	  default:  fprintf(basefile,"             \n");
		}
	  for(int j=0;j<ch_reg;j++)    //ch_reg-число подрежимов
	   {  //5
	    sprintf(str,"c:\\TC\\WORK\\furie\\%5s_%02i.f%02i",unic_name,j+1,i+1);
	    myfile=fopen(str,"rt");
	    if(myfile==NULL) printf("Не могу открыть файл%35s\n",str);
	      else
	      {
	       float vspom=0;
	       for(int k=0;k<Number;k++)
	         {
		fscanf(myfile,"%f",&vspom);
		Chast[k]=vspom;
		fscanf(myfile,"%f",&aRes[k]);
		 }
	       for(k=0;k<12;k++)
	       {
	       for(int gf=0;gf<Number;gf++)
		{ if(Chast[gf]==garm[k]) sch[k]=aRes[gf];
		  else
                  {
		 if(Chast[gf]<garm[k]&&Chast[gf+1]>garm[k])
		   {
		 if(fabs(Chast[gf]-garm[k])>fabs(Chast[gf+1]-garm[k])) sch[k]=aRes[gf+1];
		 else sch[k]=aRes[gf];
                   }
		  }
		}
	       }
                   int d;
		   switch (los)
		    {
	   case 1: for(d=0;d<10;d++)
		   {
		   if(sch[d]<(-45.65*log(garm[d])+195)) sprintf(occh[d],"неуд.");//Оценка вибрации на данной частоте
		  else
		   {
		   if ((-45.65*log(garm[d])+195)<0)  sprintf(occh[d],"отл.");
                   else sprintf(occh[d],"недоп.");
		   }
	  if(sch[d]<(-34.00*log(garm[d])+150)) sprintf(occh[d],"уд.");
	   if(sch[d]<(-23.129*log(garm[d])+100)) sprintf(occh[d],"хор.");
	     if(sch[d]<(-16.403*log(garm[d])+53)) sprintf(occh[d],"отл.");
		   }
                   break;
	   case 2: for(int d=0;d<3;d++)
		   {
		    if(sch[d]>80) sprintf(occh[d],"неуд.");
		    else  sprintf(occh[d],"уд.");
		   }
		   for(d=3;d<10;d++)
		   sprintf(occh[d],"     ");
		   break;
	  default: for(d=0;d<10;d++)
		   sprintf(occh[d],"     ");
                   }
		   for(d=10;d<12;d++)
		   {
		   if(sch[d]<30) sprintf(occh[d],"уд.");
		   else sprintf(occh[d],"неуд.");
		   }

		  fprintf(basefile,"Подрежим N%-d\n",j+1);
		  fprintf(basefile,"N гарм.\t");
		  fprintf(basefile,"Вибрация\t");
                  fprintf(basefile,"Оценка\n");
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
       printf("Продолжать ? [y/n]\n");
   scanf("%c",&kk);
  }  //3
 while ((kk!='n')&&(kk!='N'));
 free(aRes);
 free(Chast);
  for(q=0;q<12;q++)
 free (occh[q]);
 free (occh);
}  //1



