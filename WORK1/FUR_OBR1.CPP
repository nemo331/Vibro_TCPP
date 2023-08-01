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
	printf("              Вторичная обработка данных по Фурье\n");
	printf("\n");
	 printf(" Выход по ESC или продолжать\n");
         int esc=getch();
	 if(esc==27) return;

         printf(" Введите номер ГА (1-10)\n");
	 GA_num[0]=3;
	 cgets(GA_num);
	 strcpy(GA_num,&GA_num[2]);
	 int ga=atoi(GA_num);

          printf("\nВибрация: 1)Опорных узлов 2)Спинки статора\n");
	 printf(" Введите цифру 1 или 2 по умолчанию 1\n");
	  fls[0]=2;
	 cgets(fls);
	 strcpy(fls,&fls[2]);
	 fl=atoi(fls);
	 if(fl!=2){fl=1;}
         else{fl=2;}

     printf("\nВведите уникальное  наименование обрабатываемых файлов [xxxxx]\n");
    unic_name[0]=6;
    buffer=cgets(unic_name);
    strcpy(unic_name,buffer);

   printf("\n Введите наименование файла после обработки  [xx]\n");
     printf("Рекомендуемые имена: G[x]-генераторный режим,V[x]-возбуждение,\n");
     printf("   X[x]-холостой ход,B[x]-выбег,S[x]-режим СК,где [x]-любой символ\n");
      char* buf;
      O_name[0]=3;
      buf=cgets(O_name);
      strcpy(O_name,buf);

        printf("\n С какого канала обрабатывать?(от 1 до 16) [xx]\n");
   printf(" По умолчанию равно 1\n");
	 vosk[0]=3;
	 cgets(vosk);
	 strcpy(vosk,&vosk[2]);
	 int cn2=atoi(vosk);
	 if(cn2<17&&cn2>0) cn2=cn2;
	 else cn2=1;

       printf("\n");
	 for(int i=cn2-1;i<CN;i++)   //CN-число каналов
	  {   //4
	    if (kbhit())  { if (getch()==27) break;} //выход по ESC
	   sprintf(rty,"c:\\DIAGR\\fur_obr\\%2s_%5s.v%02i",O_name,unic_name,i+1);
           printf("%35s\n",rty);
	   basefile=fopen(rty,"wt");
	    if(basefile==NULL)  printf("Не могу открыть файл%35s\n",rty);

	      fprintf(basefile,"ГА %d\t",ga);
              switch(fl)
                {
	  case 1:  fprintf(basefile,"Вибрация опорных узлов\t");
		   break;
	  case 2:  fprintf(basefile,"Вибрация спинки статора\t");
		   break;
	  default:  fprintf(basefile,"             \t");
		}

          switch(O_name[0])
		{
	  case 'G':
	  case 'g':  fprintf(basefile,"Генераторный режим\n");
		   break;
	  case 'V':
	  case 'v':  fprintf(basefile,"Холостой ход с возбуждением\n");
		   break;
	  case 'X':
	  case 'x':  fprintf(basefile,"Холостой ход без возбуждения\n");
		   break;
	  case 'B':
	  case 'b':  fprintf(basefile,"Выбег\n");
		    break;
	  case 'S':
	  case 's':  fprintf(basefile,"Режим СК\n");
		   break;
	  default:  fprintf(basefile,"             \n");
		}
			  
	   sprintf(fales,"%2s_%5sv%02i",O_name,unic_name,i+1); //Формирование имени файла для передачи его в Excel
	   fprintf(basefile,"%- 12s \t",fales);
	   fprintf(basefile,"Канал N\t%d\n",i+1);

	  for(int j=0;j<ch_reg;j++)    //ch_reg-число подрежимов
	   {  //5
	    sprintf(str,"c:\\DIAGNOST\\furie\\%5s_%02i.f%02i",unic_name,j+1,i+1);
	    myfile=fopen(str,"rt");
	    if(myfile==NULL)
	     {
	      printf("Не могу открыть файл%35s\n",str);
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
                 if(j>0) fprintf(basefile,"Подрежим N%-d\n",j+1);
		   else {
		         fprintf(basefile,"Подрежим N%-d\t",j+1);
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
       printf("Продолжать ? [y/n]\n");
       kk='';
   scanf("%c",&kk);
  }  //3
 while ((kk!='n')&&(kk!='N'));
 free(aRes);
 free(Chast);
}  //1
