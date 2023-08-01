//-------------------------------------------------------------------------
//Программа вторичной обработки вибрации по Фурье(перераспределение файлов (теперь в
//одном файле будут 6 подрежимов одного канала))
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
	char unic_name [6];
	char O_name [3];
	char* buffer;

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
	printf("              Вторичная обработка данных по Фурье\n");
	printf("\n");
	 printf(" Выход по ESC или продолжать\n");
         int esc=getch();
	 if(esc==27) return;
     printf(" Введите уникальное  наименование обрабатываемых файлов [xxxxx]\n");
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
	 for(int i=0;i<CN;i++)   //CN-число каналов
	  {   //4
	   sprintf(rty,"c:\\TC\\WORK\\fur_obr\\%2s_%5s.v%02i",O_name,unic_name,i+1);
           printf("%35s\n",rty);
	   basefile=fopen(rty,"wt");
	    if(basefile==NULL)
	     { //11
	      printf("Не могу открыть файл%35s\n",rty);

	     }//11
	  for(int j=0;j<ch_reg;j++)    //ch_reg-число подрежимов
	   {  //5
	    sprintf(str,"c:\\TC\\WORK\\furie\\%5s_%02i.f%02i",unic_name,j+1,i+1);
	    myfile=fopen(str,"rt");
	    if(myfile==NULL)
	     {
	      printf("Не могу открыть файл%35s\n",str);
	     }
	      else
	      {
	       float vspom=0;
	       float stepp=0;
	       for(int k=0;k<Number;k++)
	       {
		fscanf(myfile,"%f",&vspom);
//		fscanf(myfile,"%f.5",&Chast[k]);
		Chast[k]=vspom;
		fscanf(myfile,"%f",&aRes[k]);
		 if(k==1) {stepp=vspom;}
		 }
                 fprintf(basefile,"Подрежим N%-d\n",j+1);
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
       printf("Продолжать ? [y/n]\n");
   scanf("%c",&kk);
  }  //3
 while ((kk!='n')&&(kk!='N'));
 free(aRes);
 free(Chast);
}  //1
